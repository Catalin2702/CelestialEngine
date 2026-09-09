//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: ForwardRenderer.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-09
//

#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Core/Render/Command/I_CommandEncoder.hpp"
#include "Core/Render/Command/RenderPassDescriptor.hpp"
#include "Core/Render/Command/Viewport.hpp"
#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"
#include "Core/Render/Renderer/ForwardRenderer.hpp"
#include "Core/Render/Renderer/DrawCommand.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Core/Render/Texture/I_Texture.hpp"
#include "Tools/Tools.hpp"
#include "Utility/Utility.hpp"

#include <cassert>
#include <stdexcept>


namespace CE::Core {

namespace {
constexpr auto OpenGlShadersDirectory = CE_PLATFORM_MACOS ? "../Resources/Shaders/OpenGL/" : "Resources/Shaders/OpenGL/";
}

ForwardRenderer::ForwardRenderer(std::unique_ptr<I_GraphicDevice> graphicDevice, std::unique_ptr<I_Swapchain> swapchain):
	_graphicDevice(std::move(graphicDevice)), _swapchain(std::move(swapchain))
{
	assert(_graphicDevice != nullptr && "ForwardRenderer::ForwardRenderer: The renderer was given no graphic device.");
	assert(_swapchain != nullptr && "ForwardRenderer::ForwardRenderer: The renderer was given no swapchain.");
	assert(_graphicDevice->GetGraphicApi() == _swapchain->GetGraphicApi() && "ForwardRenderer::ForwardRenderer: The device and the swapchain were created for different graphics APIs.");

	// The scene is rendered at the swapchain's own format, so the composite is a straight copy and every pipeline
	// written against the back buffer keeps working unchanged. It is the format to change for HDR, and the only one.
	_sceneColorFormat = _swapchain->GetColorFormat();

	// Up front, not on the first composite: it is built once, from shaders shipped in the bundle and a format already
	// known here, so it either works for the whole run or for none of it. Building it inside the frame would turn a
	// permanent failure into a warning on every frame, retrying something that cannot start working. Both reasons it
	// used to be deferred are already met at this point: the format is read just above, and the application has the
	// device ready to accept resources before it builds the renderer.
	_CreateCompositeResources();
}

ForwardRenderer::~ForwardRenderer() {
	// An open pass holds live device state, whatever that state is for the backend underneath, so a renderer destroyed
	// mid-frame has to close it while the device is still alive.
	ForwardRenderer::EndPass();
}

bool ForwardRenderer::BeginFrame() {
	assert(not _inFrame && "ForwardRenderer::BeginFrame: A frame is already open; every BeginFrame needs its EndFrame.");
	if (_inFrame) [[unlikely]]
		return false;

	// The geometry, and only the geometry. The back buffer is taken at the far end of the frame, in EndFrame, because
	// nothing before the composite draws into it: the scene pass and the ImGui overlay both target _sceneColor. Taking
	// it here instead made every frame hold a back buffer for its whole length - all the encoding, all the overlay -
	// and a held buffer is one the display system cannot start recycling, which is time the next frame waits out. The
	// heavier the frame, the more that costs.
	if (not _swapchain->PrepareFrame())
		return false;

	const auto [width, height] = _swapchain->GetSize();
	_EnsureSceneTarget(width, height);

	// Nothing has been acquired at this point, so there is nothing to hand back: the frame simply does not start.
	if (not _sceneColor) [[unlikely]]
		return false;

	_frameStats.Reset();

	_inFrame = true;

	return _inFrame;
}

void ForwardRenderer::EndFrame() {
	// Not an error: it is what a caller does after a BeginFrame that answered false, and presenting a frame that was
	// never drawn would put whatever the back buffer happened to hold on screen.
	if (not _inFrame)
		return;

	// The scene pass and the overlay's are both closed before the back buffer is touched at all.
	EndPass();

	// As late as the frame allows, and this is that point: the composite is the only pass that writes the back buffer,
	// so everything between here and Present is the entire time the swapchain is held. A false is the acquire the
	// backend could not satisfy - a minimised window, or no buffer free - and it has already released whatever the
	// frame encoded, so there is nothing to compose into and nothing to present.
	if (_swapchain->AcquireNextTarget()) [[likely]] {
		_Composite();
		EndPass();

		_swapchain->Present();
	}

	_stats = _frameStats;
	_inFrame = false;
}

void ForwardRenderer::BeginPass() {
	using namespace Types;

	// Backing pixels, which is what a render area and a viewport are measured in - not the window's screen coordinates.
	const auto [width, height] = _swapchain->GetSize();

	RenderPassDescriptor descriptor{};
	descriptor.width = width;
	descriptor.height = height;

	auto& [target_0, loadAction_0, storeAction_0, clearColor_0] = descriptor.colors[0];
	target_0 = _sceneColor.get();
	loadAction_0 = LoadAction::Clear;
	storeAction_0 = StoreAction::Store;
	clearColor_0 = _clearColor;

	descriptor.depth.target = _sceneDepth.get();
	descriptor.depth.enabled = true;
	descriptor.depth.loadAction = LoadAction::Clear;
	descriptor.depth.storeAction = StoreAction::DontCare;

	BeginPass(descriptor);
}

void ForwardRenderer::BeginPass(const RenderPassDescriptor& descriptor) {
	_OpenPass(descriptor);

	// Counted here rather than in _OpenPass, which is what makes the composite free: the stats describe what the
	// application asked for, and the composite is a pass the application never opened.
	if (_commandEncoder)
		++_frameStats.passes;
}

void ForwardRenderer::_OpenPass(const RenderPassDescriptor& descriptor) {
	assert(_inFrame && "ForwardRenderer::_OpenPass: A pass outside a frame draws into a target nobody will present.");
	if (not _inFrame) [[unlikely]]
		return;

	// One pass at a time: a tile-based backend cannot interleave two encoders on one command buffer, so opening a
	// second has to close the first rather than nest inside it.
	EndPass();

	_commandEncoder = _graphicDevice->BeginRenderPass(descriptor);

	// A null encoder is a pass the device skipped - a zero-sized render area - and not a failure to report twice.
	if (not _commandEncoder) [[unlikely]]
		return;

	// The whole render area, stated top-left as the Viewport constructor expects. It converts into whatever convention
	// the backend uses, and the target height is what it needs to do so.
	_commandEncoder->SetViewport(Viewport{
		GetGraphicApi(),
		0.0_f32, 0.0_f32,
		static_cast<f32>(descriptor.width),
		static_cast<f32>(descriptor.height),
		static_cast<f32>(descriptor.height)
	});
}

void ForwardRenderer::EndPass() {
	// Idempotent on purpose: EndFrame, the next BeginPass and the destructor all call it, and only one of them is the
	// one that actually closes the pass.
	if (not _commandEncoder)
		return;

	// Ended before it is dropped, not instead of: closing the pass is what releases the state it bound, so whatever
	// draws next - the ImGui overlay, the next pass - does not inherit it.
	_commandEncoder->End();
	_commandEncoder.reset();
}

void ForwardRenderer::Submit(const DrawCommand& command) {
	if (not _commandEncoder) [[unlikely]] {
		CE_CORE_WARN("ForwardRenderer::Submit: No render pass is open, so the command has nowhere to go.");
		return;
	}

	if (not command.pipelineState or not command.vertexBuffer or not command.indexBuffer) [[unlikely]] {
		CE_CORE_WARN("ForwardRenderer::Submit: The command is missing its pipeline, vertex buffer or index buffer.");
		return;
	}

	const auto availableIndices = static_cast<u32>(command.indexBuffer->GetCount());
	if (command.firstIndex >= availableIndices) [[unlikely]] {
		CE_CORE_WARN("ForwardRenderer::Submit: firstIndex {} is past the end of a {}-index buffer.", command.firstIndex, availableIndices);
		return;
	}

	const auto indexCount = command.indexCount != 0 ? command.indexCount : availableIndices - command.firstIndex;

	_commandEncoder->SetPipelineState(*command.pipelineState);
	_commandEncoder->SetVertexBuffer(*command.vertexBuffer);
	_commandEncoder->SetIndexBuffer(*command.indexBuffer);

	_commandEncoder->DrawIndexed(indexCount, command.firstIndex, command.baseVertex);

	++_frameStats.drawCalls;
	_frameStats.indices += indexCount;
}

void ForwardRenderer::OnResize(const u32 width, const u32 height) {
	assert(not _inFrame && "ForwardRenderer::OnResize: The swapchain cannot be resized inside a frame - the target the frame acquired would be invalidated.");

	_swapchain->Resize(width, height);
}

void ForwardRenderer::SetVSync(const bool enabled) {
	_swapchain->SetVSync(enabled);
}

void ForwardRenderer::SetClearColor(const glm::vec4 color) {
	_clearColor = color;
}

void ForwardRenderer::SetCameraData(const Types::CameraData& cameraData) {
	_cameraData = cameraData;
}

void ForwardRenderer::_EnsureSceneTarget(const u32 width, const u32 height) {
	using namespace Types;

	if (width == 0 or height == 0)
		return;

	if (_sceneColor and _sceneColor->GetWidth() == width and _sceneColor->GetHeight() == height)
		return;

	const TextureDescriptor colorDescriptor{
		.width = width,
		.height = height,
		.format = _sceneColorFormat,
		.usage = TextureUsage::RenderTarget | TextureUsage::ShaderRead,
		.debugName = "SceneColor"
	};

	const TextureDescriptor depthDescriptor {
		.width = width,
		.height = height,
		.format = _sceneDepthFormat,
		// Render target only: nothing reads the depth buffer back, and saying so is what lets a tile-based GPU keep it in
		// tile memory for the whole pass and never write a byte of it to RAM.
		.usage = TextureUsage::RenderTarget,
		.debugName = "SceneDepth"
	};

	try {
		auto color = _graphicDevice->CreateTexture(colorDescriptor);
		auto depth = _graphicDevice->CreateTexture(depthDescriptor);

		// Assigned only once both exist: a half-replaced pair would leave the next pass drawing colour at the new size
		// against depth at the old one, which every backend rejects.
		_sceneColor = std::move(color);
		_sceneDepth = std::move(depth);
	}
	catch (const std::exception& exception) {
		CE_CORE_ERROR("ForwardRenderer::_EnsureSceneTarget: Could not size the scene target to {}x{}: {}", width, height, exception.what());
	}
}

void ForwardRenderer::_Composite() {
	using namespace Types;

	// Nothing is checked here on purpose. The scene target is what BeginFrame refuses to open a frame without, the
	// composite resources are what the constructor refuses to exist without, and EndFrame has already returned if no
	// frame is open - so by this point all three are guaranteed.
	const auto [width, height] = _swapchain->GetSize();

	RenderPassDescriptor descriptor{};
	descriptor.width = width;
	descriptor.height = height;

	auto& color_0 = descriptor.colors[0];

	// Null target: this is the pass that writes the swapchain, and the only one.
	color_0.target = nullptr;

	// DontCare, not Load and not Clear: the quad covers every pixel, so reading the previous contents into tile
	// memory would be bandwidth spent on something about to be overwritten. Clear would cost a write for the same
	// reason.
	color_0.loadAction = LoadAction::DontCare;
	color_0.storeAction = StoreAction::Store;

	// No depth at all. A full-screen quad has nothing to test against, and asking for one would make the swapchain
	// own a depth buffer again for no reason.
	descriptor.depth.enabled = false;

	// _OpenPass, not BeginPass: this pass is the renderer's own cost and stays out of the frame stats entirely.
	_OpenPass(descriptor);
	if (not _commandEncoder) [[unlikely]]
		return;

	_commandEncoder->SetPipelineState(*_compositePipeline);
	_commandEncoder->SetVertexBuffer(*_compositeVertexBuffer);
	_commandEncoder->SetIndexBuffer(*_compositeIndexBuffer);

	// After the pipeline, not before: a texture binding belongs to the pipeline it is read through, and there is no
	// pipeline to bind it against until one is set.
	_commandEncoder->SetFragmentTexture(0, *_sceneColor);

	_commandEncoder->DrawIndexed(6, 0, 0);

	// DrawIndexed on the encoder rather than Submit, for the same reason the pass was opened with _OpenPass: nothing
	// here is counted. Folding the composite in would make every frame report one draw call and one pass more than
	// the application issued.
}

void ForwardRenderer::_CreateCompositeResources() {
	using namespace Types;

	// Clip-space quad, two triangles. The colour column is there only so the layout matches the scene's, which is
	// what lets the composite shaders share VertexInput and the engine share one vertex descriptor.
	constexpr std::array vertices{
		-1.0_f32, -1.0_f32, 0.0_f32,	1.0_f32, 1.0_f32, 1.0_f32, 1.0_f32,
		 1.0_f32, -1.0_f32, 0.0_f32,	1.0_f32, 1.0_f32, 1.0_f32, 1.0_f32,
		 1.0_f32,  1.0_f32, 0.0_f32,	1.0_f32, 1.0_f32, 1.0_f32, 1.0_f32,
		-1.0_f32,  1.0_f32, 0.0_f32,	1.0_f32, 1.0_f32, 1.0_f32, 1.0_f32,
	};

	constexpr std::array indices{0_u32, 1_u32, 2_u32, 2_u32, 3_u32, 0_u32};

	const BufferLayout vertexLayout {
		{ShaderDataType::Float3, "inputPosition"},
		{ShaderDataType::Float4, "inputColor"}
	};

	_compositeVertexBuffer = _graphicDevice->CreateVertexBuffer(vertices, vertexLayout);
	_compositeIndexBuffer = _graphicDevice->CreateIndexBuffer(indices);

	// TODO: a backend leak to remove, the twin of the one in Application::_CreateRenderResources. The descriptor
	// carries both a source and an entry point, so this has to know which half its backend reads and where the
	// artifact lives. The device should resolve that itself, and this branch - with the shader directory above it -
	// go away.
	const auto isOpenGl = _graphicDevice->GetGraphicApi() == GraphicsApi::OpenGL;

	const auto vertexSource = isOpenGl ? Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "CompositeVertex.glsl").GetContentString() : std::string{};
	const auto fragmentSource = isOpenGl ? Utility::FileSystem::StLoad(std::string(OpenGlShadersDirectory) + "CompositeFragment.glsl").GetContentString() : std::string{};

	const PipelineDescriptor pipelineDescriptor{
		.vertexShader = _graphicDevice->CreateShaderModule({
			.stage = ShaderType::Vertex,
			.source = vertexSource,
			.entryPoint = isOpenGl ? "main": "compositeVertexMain",
			.debugName = "CompositeVertex"
		}),
		.fragmentShader = _graphicDevice->CreateShaderModule({
			.stage = ShaderType::Fragment,
			.source = fragmentSource,
			.entryPoint = isOpenGl ? "main": "compositeFragmentMain",
			.debugName = "CompositeFragment"
		}),
		.vertexLayout = vertexLayout,
		.cullMode = CullMode::None,
		.depthState = {
			.testEnabled = false,
			.writeEnabled = false
		},
		.blendState = {
			.enabled = false,
		},
		.formats = {
			.colors = {_swapchain->GetColorFormat()},
			.colorCount = 1,
			.depth = PixelFormat::None
		}
	};

	_compositePipeline = _graphicDevice->CreatePipelineState(pipelineDescriptor);

	// Fatal, and deliberately so: the composite is the only pass that writes the back buffer, so a renderer without it
	// cannot put a single frame on screen. There is no degraded mode to fall back to, and failing here reports it once,
	// at startup, where it can be acted on.
	if (not _compositePipeline or not _compositeVertexBuffer or not _compositeIndexBuffer) [[unlikely]] {
		constexpr auto error = "ForwardRenderer::_CreateCompositeResources: The composite pipeline could not be built!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

}
