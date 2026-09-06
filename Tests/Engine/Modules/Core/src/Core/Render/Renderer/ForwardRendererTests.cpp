//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Render/Renderer
// File: ForwardRendererTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include <Core/Render/Buffer/I_Buffer.hpp>
#include <Core/Render/Command/I_CommandEncoder.hpp>
#include <Core/Render/Command/RenderPassDescriptor.hpp>
#include <Core/Render/Command/Viewport.hpp>
#include <Core/Render/Device/I_GraphicDevice.hpp>
#include <Core/Render/Pipeline/I_PipelineState.hpp>
#include <Core/Render/Pipeline/PipelineDescriptor.hpp>
#include <Core/Render/Renderer/DrawCommand.hpp>
#include <Core/Render/Renderer/ForwardRenderer.hpp>
#include <Core/Render/Shader/I_ShaderModule.hpp>
#include <Core/Render/Shader/ShaderModuleDescriptor.hpp>
#include <Core/Render/Swapchain/I_Swapchain.hpp>
#include <Core/Render/Texture/I_Texture.hpp>
#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace CE;
using CE::Core::BufferLayout;
using CE::Core::DrawCommand;
using CE::Core::ForwardRenderer;
using CE::Core::I_CommandEncoder;
using CE::Core::I_GraphicDevice;
using CE::Core::I_IndexBuffer;
using CE::Core::I_PipelineState;
using CE::Core::I_ShaderModule;
using CE::Core::I_Swapchain;
using CE::Core::I_Texture;
using CE::Core::I_VertexBuffer;
using CE::Core::PipelineDescriptor;
using CE::Core::RenderPassDescriptor;
using CE::Core::ShaderModuleDescriptor;
using CE::Core::TextureDescriptor;
using CE::Core::Viewport;

namespace {

/// Vulkan on purpose, and it is not a hint about a future backend: the engine has no Vulkan implementation, so no
/// assertion anywhere can mistake one of these fakes for a real object, and ForwardRenderer's OpenGL branch - the one
/// that loads GLSL off disk - stays out of a test that has no bundle to load it from.
constexpr auto FakeApi = Types::GraphicsApi::Vulkan;

constexpr u32 DefaultWidth = 1280;
constexpr u32 DefaultHeight = 720;
constexpr auto DefaultColorFormat = Types::PixelFormat::BGRA8Unorm;

/**
 * @struct TextureRequest
 * @brief A copy of what the renderer asked CreateTexture for
 * @details The descriptor's debugName is a string_view borrowed for the call, so it is copied rather than kept.
 */
struct TextureRequest {
	u32 width = 0;
	u32 height = 0;
	Types::PixelFormat format = Types::PixelFormat::None;
	Types::TextureUsage usage = Types::TextureUsage::None;
	std::string debugName;
};

/**
 * @struct PassRecord
 * @brief One render pass the renderer opened, and what it encoded into it
 */
struct PassRecord {
	RenderPassDescriptor descriptor{};

	std::vector<std::pair<u32, const I_Texture*>> fragmentTextures;
	std::vector<Viewport> viewports;

	u32 drawIndexedCalls = 0;
	u32 indices = 0;

	bool ended = false;
};

/**
 * @struct Recorder
 * @brief What the fakes saw, kept alive independently of them
 * @details The renderer takes ownership of the device and the swapchain, so a test cannot hold either. It holds this
 *			instead, and the fakes only borrow it.
 */
struct Recorder {
	std::vector<TextureRequest> textures;
	std::vector<PassRecord> passes;

	std::vector<PipelineDescriptor> pipelines;
	u32 shaderModules = 0;
	u32 vertexBuffers = 0;
	u32 indexBuffers = 0;

	u32 acquires = 0;
	u32 presents = 0;
	u32 resizes = 0;

	/// Set before the renderer is built, to drive the failure paths.
	bool failTextureCreation = false;
	bool failPipelineCreation = false;

	u32 width = DefaultWidth;
	u32 height = DefaultHeight;

	/// Answered by AcquireNextTarget, so a test can play a frame the swapchain refuses.
	bool acquireSucceeds = true;
};

class FakeTexture final: public I_Texture {
public:
	explicit FakeTexture(const TextureDescriptor& descriptor):
		_width(descriptor.width), _height(descriptor.height), _format(descriptor.format), _usage(descriptor.usage) {}

public:
	[[nodiscard]] u32 GetWidth() const override { return _width; }
	[[nodiscard]] u32 GetHeight() const override { return _height; }
	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_width, _height}; }
	[[nodiscard]] Types::PixelFormat GetFormat() const override { return _format; }
	[[nodiscard]] Types::TextureUsage GetUsage() const override { return _usage; }
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

private:
	u32 _width;
	u32 _height;
	Types::PixelFormat _format;
	Types::TextureUsage _usage;
};

class FakeShaderModule final: public I_ShaderModule {
public:
	explicit FakeShaderModule(const ShaderModuleDescriptor& descriptor):
		_stage(descriptor.stage), _entryPoint(descriptor.entryPoint) {}

public:
	[[nodiscard]] Types::ShaderType GetStage() const override { return _stage; }
	[[nodiscard]] std::string_view GetEntryPoint() const override { return _entryPoint; }
	[[nodiscard]] bool IsValid() const override { return true; }
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

private:
	Types::ShaderType _stage;
	std::string _entryPoint;
};

class FakePipelineState final: public I_PipelineState {
public:
	explicit FakePipelineState(PipelineDescriptor descriptor): _descriptor(std::move(descriptor)) {}

public:
	[[nodiscard]] const PipelineDescriptor& GetDescriptor() const override { return _descriptor; }
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

private:
	PipelineDescriptor _descriptor;
};

class FakeVertexBuffer final: public I_VertexBuffer {
public:
	explicit FakeVertexBuffer(BufferLayout layout): _layout(std::move(layout)) {}

public:
	void SetLayout(const BufferLayout& layout) override { _layout = layout; }
	void SetLayout(BufferLayout&& layout) override { _layout = std::move(layout); }

	[[nodiscard]] const BufferLayout& GetLayout() const override { return _layout; }
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

	[[nodiscard]] u32 BindLayout(const u32 firstAttributeIndex) const override { return firstAttributeIndex; }

private:
	BufferLayout _layout;
};

class FakeIndexBuffer final: public I_IndexBuffer {
public:
	explicit FakeIndexBuffer(const size_t count): _count(count) {}

public:
	[[nodiscard]] size_t GetCount() const override { return _count; }
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

private:
	size_t _count;
};

/// Writes into the pass its device opened. The index rather than a reference, because the recorder's vector grows.
class FakeCommandEncoder final: public I_CommandEncoder {
public:
	FakeCommandEncoder(Recorder& recorder, const size_t passIndex): _recorder(recorder), _passIndex(passIndex) {}

public:
	void DrawIndexed(const u32 indexCount, u32, u32) override {
		auto& pass = _recorder.passes[_passIndex];
		++pass.drawIndexedCalls;
		pass.indices += indexCount;
	}

	void End() override { _recorder.passes[_passIndex].ended = true; }

	void SetPipelineState(const I_PipelineState&) override {}
	void SetIndexBuffer(const I_IndexBuffer&) override {}
	void SetVertexBuffer(const I_VertexBuffer&) override {}

	void SetViewport(const Viewport& viewport) override { _recorder.passes[_passIndex].viewports.push_back(viewport); }

	void SetFragmentTexture(const u32 slot, const I_Texture& texture) override {
		_recorder.passes[_passIndex].fragmentTextures.emplace_back(slot, &texture);
	}

	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

private:
	Recorder& _recorder;
	size_t _passIndex;
};

class FakeGraphicDevice final: public I_GraphicDevice {
public:
	explicit FakeGraphicDevice(Recorder& recorder): _recorder(recorder) {}

public:
	[[nodiscard]] std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) override {
		++_recorder.shaderModules;
		return std::make_shared<FakeShaderModule>(descriptor);
	}

	[[nodiscard]] std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) override {
		_recorder.pipelines.push_back(descriptor);
		if (_recorder.failPipelineCreation)
			return nullptr;

		return std::make_shared<FakePipelineState>(descriptor);
	}

	[[nodiscard]] std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(const std::span<const u32> indices) override {
		++_recorder.indexBuffers;
		return std::make_shared<FakeIndexBuffer>(indices.size());
	}

	[[nodiscard]] std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32>, const BufferLayout& layout) override {
		++_recorder.vertexBuffers;
		return std::make_shared<FakeVertexBuffer>(layout);
	}

	[[nodiscard]] std::shared_ptr<I_Texture> CreateTexture(const TextureDescriptor& descriptor) override {
		_recorder.textures.push_back({
			descriptor.width, descriptor.height, descriptor.format, descriptor.usage, std::string(descriptor.debugName)
		});

		// Thrown, not returned null: it is what MetalTexture does when the allocation fails, and _EnsureSceneTarget
		// is written around catching it.
		if (_recorder.failTextureCreation)
			throw std::runtime_error("FakeGraphicDevice::CreateTexture: refused by the test.");

		return std::make_shared<FakeTexture>(descriptor);
	}

	[[nodiscard]] std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) override {
		PassRecord record{};
		record.descriptor = descriptor;
		_recorder.passes.push_back(std::move(record));
		return std::make_unique<FakeCommandEncoder>(_recorder, _recorder.passes.size() - 1);
	}

	[[nodiscard]] Types::GraphicsApi GetGraphicApi() override { return FakeApi; }

private:
	Recorder& _recorder;
};

class FakeSwapchain final: public I_Swapchain {
public:
	explicit FakeSwapchain(Recorder& recorder): _recorder(recorder) {}

public:
	[[nodiscard]] bool AcquireNextTarget() override {
		++_recorder.acquires;
		return _recorder.acquireSucceeds;
	}

	void Present() override { ++_recorder.presents; }

	void Resize(const u32 width, const u32 height) override {
		++_recorder.resizes;
		_recorder.width = width;
		_recorder.height = height;
	}

	void SetVSync(const bool enabled) override { _vsync = enabled; }
	[[nodiscard]] bool IsVSyncEnabled() const override { return _vsync; }

	[[nodiscard]] Types::PixelFormat GetColorFormat() const override { return DefaultColorFormat; }
	[[nodiscard]] u32 GetBufferCount() const override { return 3; }
	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_recorder.width, _recorder.height}; }
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return FakeApi; }

private:
	Recorder& _recorder;
	bool _vsync = false;
};

/**
 * @brief Test fixture for the renderer's frame structure
 * @details The recorder outlives the renderer, which is what lets a test read back what the fakes saw after the
 *			renderer has taken ownership of them.
 */
class ForwardRendererTest: public ::testing::Test {
protected:
	// The renderer reports its failure paths through CE_CORE_ERROR, which asserts the logger exists. Init is
	// idempotent, so a fixture that brings it up per test costs nothing and keeps the pairing symmetric.
	void SetUp() override { Tools::Log::Init(); }
	void TearDown() override { Tools::Log::Shutdown(); }

	[[nodiscard]] std::unique_ptr<ForwardRenderer> MakeRenderer() {
		return std::make_unique<ForwardRenderer>(
			std::make_unique<FakeGraphicDevice>(recorder),
			std::make_unique<FakeSwapchain>(recorder)
		);
	}

	/// The pass every frame ends with: the composite. Named so the assertions read as what they mean.
	[[nodiscard]] const PassRecord& CompositePass() const { return recorder.passes.back(); }

	Recorder recorder{};
};

}

/**
 * @brief Test that the composite pipeline is built before the first frame rather than inside it
 * @details It is built once, from shaders in the bundle and a format known at construction, so it either works for
 *			the whole run or for none of it. Building it lazily turned a permanent failure into a warning per frame.
 */
TEST_F(ForwardRendererTest, Construction_BuildsTheCompositeUpFront) {
	const auto renderer = MakeRenderer();

	EXPECT_EQ(recorder.pipelines.size(), 1u);
	EXPECT_EQ(recorder.vertexBuffers, 1u);
	EXPECT_EQ(recorder.indexBuffers, 1u);

	// Nothing has been drawn: the resources exist before any frame does.
	EXPECT_TRUE(recorder.passes.empty());
	EXPECT_EQ(recorder.acquires, 0u);
}

/**
 * @brief Test that a renderer that cannot composite refuses to exist
 * @details The composite is the only pass that writes the back buffer, so there is no degraded mode to fall back to.
 *			Failing in the constructor reports it once, at startup, instead of once per frame forever.
 */
TEST_F(ForwardRendererTest, Construction_ThrowsWhenTheCompositePipelineCannotBeBuilt) {
	recorder.failPipelineCreation = true;

	EXPECT_THROW(static_cast<void>(MakeRenderer()), std::runtime_error);
}

/**
 * @brief Test that the composite pipeline declares the swapchain's format, not the scene's
 * @details It is the one pipeline that draws into the back buffer, so it is the one that has to match the swapchain
 *			the day the scene target becomes RGBA16Float for HDR.
 */
TEST_F(ForwardRendererTest, Construction_CompositeTargetsTheSwapchainFormat) {
	const auto renderer = MakeRenderer();

	ASSERT_EQ(recorder.pipelines.size(), 1u);
	const auto& composite = recorder.pipelines.front();

	EXPECT_EQ(composite.formats.colorCount, 1u);
	EXPECT_EQ(composite.formats.colors[0], DefaultColorFormat);

	// No depth anywhere: a full-screen quad has nothing to test against, and asking for one would make the swapchain
	// own a depth buffer again.
	EXPECT_EQ(composite.formats.depth, Types::PixelFormat::None);
	EXPECT_FALSE(composite.depthState.testEnabled);
	EXPECT_FALSE(composite.depthState.writeEnabled);
}

/**
 * @brief Test that the scene colour is sized from the swapchain and reports the format pipelines must declare
 */
TEST_F(ForwardRendererTest, BeginFrame_SizesTheSceneTargetToTheSwapchain) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());

	const auto* const sceneColor = renderer->GetSceneColorTarget();
	ASSERT_NE(sceneColor, nullptr);

	EXPECT_EQ(sceneColor->GetWidth(), DefaultWidth);
	EXPECT_EQ(sceneColor->GetHeight(), DefaultHeight);
	EXPECT_EQ(sceneColor->GetFormat(), renderer->GetSceneColorFormat());
	EXPECT_EQ(renderer->GetSceneColorFormat(), DefaultColorFormat);
}

/**
 * @brief Test that the scene colour is readable by the composite and the depth buffer deliberately is not
 * @details ShaderRead on the colour is what the composite's texture fetch needs. Leaving it off the depth buffer is
 *			what lets a tile-based GPU keep it in tile memory and never write a byte of it to RAM.
 */
TEST_F(ForwardRendererTest, BeginFrame_AsksForASceneColourTheCompositeCanRead) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	ASSERT_EQ(recorder.textures.size(), 2u);

	const auto& color = recorder.textures[0];
	EXPECT_EQ(color.debugName, "SceneColor");
	EXPECT_NE(color.usage & Types::TextureUsage::RenderTarget, Types::TextureUsage::None);
	EXPECT_NE(color.usage & Types::TextureUsage::ShaderRead, Types::TextureUsage::None);

	const auto& depth = recorder.textures[1];
	EXPECT_EQ(depth.debugName, "SceneDepth");
	EXPECT_NE(depth.usage & Types::TextureUsage::RenderTarget, Types::TextureUsage::None);
	EXPECT_EQ(depth.usage & Types::TextureUsage::ShaderRead, Types::TextureUsage::None);
}

/**
 * @brief Test that a frame that cannot allocate its scene target still gives the drawable back
 * @details AcquireNextTarget has already taken a drawable by this point, and Present is what releases it. Returning
 *			without presenting held one buffer per failed frame until the layer had none left and the next acquire
 *			blocked for good.
 */
TEST_F(ForwardRendererTest, BeginFrame_ReturnsTheDrawableWhenTheSceneTargetCannotBeAllocated) {
	const auto renderer = MakeRenderer();
	recorder.failTextureCreation = true;

	EXPECT_FALSE(renderer->BeginFrame());

	EXPECT_EQ(recorder.acquires, 1u);
	EXPECT_EQ(recorder.presents, 1u);
	EXPECT_EQ(renderer->GetSceneColorTarget(), nullptr);
}

/**
 * @brief Test that a frame the swapchain refused presents nothing
 * @details The mirror of the case above: no drawable was taken, so there is none to give back.
 */
TEST_F(ForwardRendererTest, BeginFrame_PresentsNothingWhenTheAcquireFailed) {
	const auto renderer = MakeRenderer();
	recorder.acquireSucceeds = false;

	EXPECT_FALSE(renderer->BeginFrame());

	EXPECT_EQ(recorder.acquires, 1u);
	EXPECT_EQ(recorder.presents, 0u);
}

/**
 * @brief Test that the scene target survives frames of an unchanged size
 * @details It is allocated in BeginFrame, which runs every frame; reallocating a full-screen colour and depth pair
 *			120 times a second is the failure mode this guards.
 */
TEST_F(ForwardRendererTest, BeginFrame_KeepsTheSceneTargetAcrossFramesOfTheSameSize) {
	const auto renderer = MakeRenderer();

	for (auto frame = 0; frame < 3; ++frame) {
		ASSERT_TRUE(renderer->BeginFrame());
		renderer->EndFrame();
	}

	EXPECT_EQ(recorder.textures.size(), 2u);
	EXPECT_EQ(recorder.presents, 3u);
}

/**
 * @brief Test that a resize replaces both scene textures, not one of them
 * @details A half-replaced pair leaves the next pass drawing colour at the new size against depth at the old one,
 *			which every backend rejects.
 */
TEST_F(ForwardRendererTest, BeginFrame_ReplacesTheSceneTargetWhenTheSwapchainResizes) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->EndFrame();

	renderer->OnResize(800, 600);

	ASSERT_TRUE(renderer->BeginFrame());

	ASSERT_EQ(recorder.textures.size(), 4u);
	EXPECT_EQ(recorder.textures[2].width, 800u);
	EXPECT_EQ(recorder.textures[2].height, 600u);
	EXPECT_EQ(recorder.textures[3].width, 800u);
	EXPECT_EQ(recorder.textures[3].height, 600u);

	const auto* const sceneColor = renderer->GetSceneColorTarget();
	ASSERT_NE(sceneColor, nullptr);
	EXPECT_EQ(sceneColor->GetWidth(), 800u);
	EXPECT_EQ(sceneColor->GetHeight(), 600u);
}

/**
 * @brief Test that the renderer's own pass draws into the scene target and never into the back buffer
 * @details The whole point of the offscreen target. A backend that ignores the descriptor's target and renders into
 *			the drawable anyway leaves the scene texture untouched - which is what this pins.
 */
TEST_F(ForwardRendererTest, DefaultPass_RendersIntoTheSceneTargetNotTheBackBuffer) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->BeginPass();

	ASSERT_EQ(recorder.passes.size(), 1u);
	const auto& descriptor = recorder.passes.front().descriptor;

	EXPECT_EQ(descriptor.colors[0].target, renderer->GetSceneColorTarget());
	EXPECT_EQ(descriptor.colors[0].loadAction, Types::LoadAction::Clear);
	EXPECT_EQ(descriptor.colors[0].storeAction, Types::StoreAction::Store);

	EXPECT_TRUE(descriptor.depth.enabled);
	EXPECT_NE(descriptor.depth.target, nullptr);
	EXPECT_NE(descriptor.depth.target, descriptor.colors[0].target);

	EXPECT_EQ(descriptor.width, DefaultWidth);
	EXPECT_EQ(descriptor.height, DefaultHeight);
}

/**
 * @brief Test that the composite is the last pass of the frame and the only one aimed at the back buffer
 * @details A null target means the swapchain's drawable. Exactly one pass may carry it, and it has to be the one
 *			that runs after everything else has been drawn.
 */
TEST_F(ForwardRendererTest, EndFrame_CompositesIntoTheBackBufferExactlyOnce) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->BeginPass();
	renderer->EndFrame();

	ASSERT_EQ(recorder.passes.size(), 2u);

	auto backBufferPasses = 0;
	for (const auto& pass: recorder.passes)
		if (pass.descriptor.colors[0].target == nullptr)
			++backBufferPasses;

	EXPECT_EQ(backBufferPasses, 1);
	EXPECT_EQ(CompositePass().descriptor.colors[0].target, nullptr);
}

/**
 * @brief Test that the composite loads nothing and tests no depth
 * @details The quad covers every pixel, so reading the previous contents into tile memory would be bandwidth spent
 *			on something about to be overwritten - and a Clear would cost a write for the same reason.
 */
TEST_F(ForwardRendererTest, EndFrame_CompositeLoadsNothingAndHasNoDepth) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->EndFrame();

	const auto& composite = CompositePass().descriptor;

	EXPECT_EQ(composite.colors[0].loadAction, Types::LoadAction::DontCare);
	EXPECT_EQ(composite.colors[0].storeAction, Types::StoreAction::Store);

	EXPECT_FALSE(composite.depth.enabled);
	EXPECT_EQ(composite.depth.target, nullptr);
}

/**
 * @brief Test that the composite reads the scene colour from slot 0 and draws the quad
 * @details Slot 0 is what both composite shaders declare - texture(0) on Metal, the sceneColor sampler on OpenGL -
 *			and six indices are the two triangles of the full-screen quad.
 */
TEST_F(ForwardRendererTest, EndFrame_CompositeReadsTheSceneColour) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	const auto* const sceneColor = renderer->GetSceneColorTarget();
	renderer->EndFrame();

	const auto& composite = CompositePass();

	ASSERT_EQ(composite.fragmentTextures.size(), 1u);
	EXPECT_EQ(composite.fragmentTextures.front().first, 0u);
	EXPECT_EQ(composite.fragmentTextures.front().second, sceneColor);

	EXPECT_EQ(composite.drawIndexedCalls, 1u);
	EXPECT_EQ(composite.indices, 6u);
}

/**
 * @brief Test that the composite pass is closed before the frame is presented
 * @details On Metal the present is added to the frame's own command buffer, and an encoder still open when that
 *			buffer is committed is a validation error rather than a late frame.
 */
TEST_F(ForwardRendererTest, EndFrame_ClosesTheCompositePassBeforePresenting) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->BeginPass();
	renderer->EndFrame();

	for (const auto& pass: recorder.passes)
		EXPECT_TRUE(pass.ended);

	EXPECT_EQ(recorder.presents, 1u);
}

/**
 * @brief Test that the composite appears in none of the numbers the application is shown
 * @details It is the renderer's own cost, not the scene's: a frame that opened one pass and issued one draw has to
 *			report exactly that, however many passes the renderer needed to put it on screen.
 */
TEST_F(ForwardRendererTest, EndFrame_DoesNotCountTheCompositeInTheFrameStats) {
	const auto renderer = MakeRenderer();

	const FakePipelineState pipeline{PipelineDescriptor{}};
	const FakeVertexBuffer vertexBuffer{BufferLayout{}};
	const FakeIndexBuffer indexBuffer{36};

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->BeginPass();
	renderer->Submit(DrawCommand{
		.pipelineState = &pipeline,
		.vertexBuffer = &vertexBuffer,
		.indexBuffer = &indexBuffer
	});
	renderer->EndFrame();

	const auto& stats = renderer->GetRenderStats();

	EXPECT_EQ(stats.drawCalls, 1u);
	EXPECT_EQ(stats.indices, 36u);
	EXPECT_EQ(stats.passes, 1u);

	// The composite did run: it is simply not in the totals above.
	EXPECT_EQ(recorder.passes.size(), 2u);
	EXPECT_EQ(CompositePass().drawIndexedCalls, 1u);
}

/**
 * @brief Test that EndFrame composites even when the application opened no pass of its own
 * @details A frame that drew nothing still has to write the back buffer: skipping the composite would leave the
 *			drawable holding whatever the recycled buffer happened to contain.
 */
TEST_F(ForwardRendererTest, EndFrame_CompositesAFrameThatDrewNothing) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->EndFrame();

	ASSERT_EQ(recorder.passes.size(), 1u);
	EXPECT_EQ(CompositePass().descriptor.colors[0].target, nullptr);
	EXPECT_EQ(CompositePass().drawIndexedCalls, 1u);
}

/**
 * @brief Test that EndFrame outside a frame presents nothing and composites nothing
 * @details It is what a caller does after a BeginFrame that answered false, and presenting there would put a frame
 *			nobody drew on screen.
 */
TEST_F(ForwardRendererTest, EndFrame_OutsideAFrameDoesNothing) {
	const auto renderer = MakeRenderer();

	renderer->EndFrame();

	EXPECT_TRUE(recorder.passes.empty());
	EXPECT_EQ(recorder.presents, 0u);
}

/**
 * @brief Test that the overlay is handed the same texture the scene was drawn into
 * @details GetSceneColorTarget is what the ImGui layer opens its pass on. If it answered anything but the target of
 *			the renderer's own pass, the overlay would be drawn onto an image the composite never reads.
 */
TEST_F(ForwardRendererTest, GetSceneColorTarget_IsWhatTheDefaultPassDrawsInto) {
	const auto renderer = MakeRenderer();

	ASSERT_TRUE(renderer->BeginFrame());
	renderer->BeginPass();

	ASSERT_FALSE(recorder.passes.empty());
	EXPECT_EQ(renderer->GetSceneColorTarget(), recorder.passes.front().descriptor.colors[0].target);
}
