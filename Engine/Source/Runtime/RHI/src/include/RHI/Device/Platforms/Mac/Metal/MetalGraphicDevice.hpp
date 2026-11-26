//
// Module: CelestialEngine/Engine/Source/Runtime/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_RHI_DEVICE_METALGRAPHICDEVICE_HPP
#define CE_RHI_DEVICE_METALGRAPHICDEVICE_HPP

#include "RHI/Device/I_GraphicDevice.hpp"
#include "RHI/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <semaphore>


namespace CA {
class MetalDrawable;
}

namespace MTL {
class Buffer;
class CommandBuffer;
class CommandQueue;
class Device;
class Texture;
}

namespace CE::RHI {

/**
 * @struct MetalConstantAllocation
 * @brief Where a block of constants landed in the frame's ring: the buffer to bind, and the offset to bind it at
 * @details A null buffer means the frame's slice is full, and the caller has to carry the block some other way.
 */
struct MetalConstantAllocation {
	MTL::Buffer* buffer = nullptr; ///< Borrowed; owned by the device's ring
	size_t offset = 0; ///< In bytes, a multiple of MetalGraphicDevice::CONSTANT_OFFSET_ALIGNMENT
};

/**
 * @class MetalGraphicDevice
 * @brief The Metal device, its queue and its shader library - everything a resource is created from
 * @details Owns what outlives a frame; owns nothing that belongs to one. The drawable a frame renders into comes from
 *			the swapchain, and is published here through SetFrameTarget for the duration of that frame.
 */
class CE_RHI_API MetalGraphicDevice final: public I_GraphicDeviceBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief How many frames the CPU may hand to the GPU before it has to wait for the oldest one to finish
	 * @details Three, the same count CAMetalLayer rotates its drawables through by default. One fewer and the GPU
	 *			goes idle whenever a frame runs long on the CPU; one more buys no throughput once the GPU is the
	 *			bottleneck, only another frame of latency between an input and the pixels that answer it.
	 */
	static constexpr u32 MAX_FRAMES_IN_FLIGHT = 3;

	/**
	 * @brief Bytes of constants one frame can write into its slice of the ring
	 * @details 256 KB is about a thousand blocks at one aligned block each. A frame that needs more falls back to
	 *			inline bytes instead of failing, so this is a ceiling on performance, not on correctness.
	 */
	static constexpr size_t FRAME_CONSTANTS_CAPACITY = 256 * 1024;

	/**
	 * @brief The alignment Metal asks of an offset into a buffer bound to the constant address space
	 * @details 256 bytes on macOS, against 4 on iOS. Every block costs at least this much of the slice, which is
	 *			why a 64-byte matrix takes 256: the rest is padding Metal requires, not waste we chose.
	 */
	static constexpr size_t CONSTANT_OFFSET_ALIGNMENT = 256;

public:
	/**
	 * @brief Creates the system default device, its command queue and loads the bundled .metallib
	 * @details Throws std::runtime_error if any of the three fails: without them nothing can be rendered, and a
	 *			half-built device would only move the failure to the first draw.
	 */
	MetalGraphicDevice();

	MetalGraphicDevice(const MetalGraphicDevice&) = delete;

	MetalGraphicDevice(MetalGraphicDevice&& other) noexcept = default;

	/**
	 * @brief Waits until the GPU has finished every frame in flight, then releases the device
	 * @details In the .cpp file for two reasons. The old one: `NS::SharedPtr` destroys the forward declared classes,
	 *			and only the .cpp sees them complete. The new one is the wait: every command buffer in flight has a
	 *			completion handler holding the address of _framesInFlight, and a handler that runs after the semaphore
	 *			is gone writes into freed memory.
	 */
	~MetalGraphicDevice() override;

public:
	MetalGraphicDevice& operator = (const MetalGraphicDevice&) = delete;
	MetalGraphicDevice& operator = (MetalGraphicDevice&& other) noexcept = default;

public:
	[[nodiscard]] std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) override;
	[[nodiscard]] std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> vertices, const BufferLayout& layout) override;

	[[nodiscard]] std::shared_ptr<I_Texture> CreateTexture(const TextureDescriptor& descriptor) override;

	/**
	 * @brief Builds the Metal render pass descriptor and opens an encoder on it
	 * @return A MetalCommandEncoder, or null when the pass has nothing to draw into
	 * @details Returns null - the same "skip this pass" answer the OpenGL device gives for a zero-sized render area -
	 *			when no frame target has been published, which is what a frame whose drawable was not acquired looks
	 *			like. That is normal under memory pressure, not an error to escalate.
	 */
	[[nodiscard]] std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) override;

public:
	/**
	 * @brief Publishes the textures the current frame renders into
	 * @param drawable The drawable MetalSwapchain acquired for this frame, or null to clear the target
	 * @details Called by MetalSwapchain::AcquireNextTarget, and cleared at Present. It is the seam between "who owns
	 *			the back buffer" (the swapchain) and "who builds render passes" (the device), and it is deliberately
	 *			Metal-only: the portable version of it is a RenderPassDescriptor carrying an I_Texture per attachment,
	 *			which is what this collapses into the day the engine has a texture type.
	 *
	 *			The drawable is borrowed for the frame, not retained: it belongs to the layer's autorelease pool, and
	 *			the swapchain is what holds a reference to it for the length of the frame.
	 */
	void SetFrameTarget(CA::MetalDrawable* drawable);

	/**
	 * @brief Gets the command buffer every pass of this frame encodes into, opening one if the frame has none yet
	 * @return MTL::CommandBuffer* The frame's buffer, or null when the queue refuses one
	 * @details One buffer per frame, several encoders on it - not one buffer per pass. Metal tracks the dependency
	 *			between a pass that writes an attachment and a later pass that loads it *within* a command buffer; it
	 *			does not do so between separate ones, where all that is guaranteed is the order they execute in. On a
	 *			tile-based GPU that difference is visible: the second pass loads tiles the first has not resolved yet,
	 *			and redraws them as it found them.
	 *			Opening the buffer is also where the CPU is held back. With MAX_FRAMES_IN_FLIGHT frames still on the
	 *			GPU this blocks until the oldest completes: it runs before the first byte of the frame's constants is
	 *			written, and waiting any earlier would only stall work that touches nothing the GPU reads.
	 */
	[[nodiscard]] MTL::CommandBuffer* GetFrameCommandBuffer();

	/**
	 * @brief Hands the frame's command buffer over, leaving none behind
	 * @return NS::SharedPtr<MTL::CommandBuffer> The frame's buffer, or null when nothing was encoded
	 * @details The caller owns it, and owes it a commit - it is the one that presents. That ordering is not a
	 *			tidiness: presentDrawable shows the drawable when *its own* command buffer is scheduled, and a buffer
	 *			holding nothing but a present is scheduled at once - possibly while the pass that draws the frame is
	 *			still running on the GPU. Presenting from the buffer that did the drawing is the only arrangement in
	 *			which that cannot happen.
	 */
	[[nodiscard]] NS::SharedPtr<MTL::CommandBuffer> TakeFrameCommandBuffer();

	/**
	 * @brief Copies a block of constants into the current frame's slice of the ring
	 * @return MetalConstantAllocation The buffer and the offset to bind it at, or a null buffer when the slice is full
	 * @details Valid only while the frame's command buffer is open: opening it is what waited, through the semaphore,
	 *			for the GPU to finish the frame that used this slice last. The copy is what lets the caller change its
	 *			own bytes right after the call, the same promise setVertexBytes makes.
	 */
	[[nodiscard]] MetalConstantAllocation AllocateFrameConstants(std::span<const std::byte> data);

public:
	/// Clip space runs from 0 at the near plane to 1 at the far one here; the projection matrix has to be built for
	/// that range, and this is the only object that can say so without naming the API somewhere else.
	[[nodiscard]] Types::ClipConvention GetClipConvention() const override { return Types::ClipConvention::ZeroToOne; }

public:
	[[nodiscard]] MTL::Device* GetDevice() const { return _nativeDevice.get(); }
	[[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return _nativeCommandQueue.get(); }
	[[nodiscard]] const MetalShaderLibrary& GetShaderLibrary() const { return _shaderLibrary; }

private:
	NS::SharedPtr<MTL::Device> _nativeDevice;
	NS::SharedPtr<MTL::CommandQueue> _nativeCommandQueue;
	MetalShaderLibrary _shaderLibrary;

	CA::MetalDrawable* _nativeFrameDrawable = nullptr; ///< Borrowed for the current frame; see SetFrameTarget

	/// The last command buffer encoded into the frame's target, waiting for the present to be added to it. Owned only
	/// until the swapchain takes it, which is once per frame.
	NS::SharedPtr<MTL::CommandBuffer> _nativeFrameCommandBuffer;

	/// One count per frame the GPU has not finished: taken when a frame opens its command buffer, given back by that
	/// buffer's completion handler. Behind a pointer because every pending handler holds its address, and the address
	/// must not change when the device is moved - std::counting_semaphore cannot be moved anyway.
	std::unique_ptr<std::counting_semaphore<MAX_FRAMES_IN_FLIGHT>> _framesInFlight;

	/// The ring: one slice per frame in flight, so the frame being encoded never writes the slice the GPU is reading
	std::array<NS::SharedPtr<MTL::Buffer>, MAX_FRAMES_IN_FLIGHT> _frameConstantBuffers;
	u32 _frameIndex = 0; ///< The slice the open frame writes into
	size_t _frameConstantOffset = 0; ///< First free byte of that slice
};

}

#endif //CE_RHI_DEVICE_METALGRAPHICDEVICE_HPP
