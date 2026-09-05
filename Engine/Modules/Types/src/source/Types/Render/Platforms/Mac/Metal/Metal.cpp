//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Mac/Metal
// File: Metal.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Types/Render/Platforms/Mac/Metal/Metal.hpp"

#include <Metal/Metal.hpp>


static_assert(std::is_same_v<std::underlying_type_t<MTL::PixelFormat>, std::uintptr_t>, "MetalCpp changed the underlying type of MTL enums; the forward declarations in Metal.hpp must follow.");

namespace CE::Types {

MTL::PixelFormat ToMetal(const PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case PixelFormat::R8Unorm: return MTL::PixelFormatR8Unorm;
		case PixelFormat::RG8Unorm: return MTL::PixelFormatRG8Unorm;
		case PixelFormat::RGBA8Unorm: return MTL::PixelFormatRGBA8Unorm;
		case PixelFormat::RGBA8UnormSrgb: return MTL::PixelFormatRGBA8Unorm_sRGB;
		case PixelFormat::BGRA8Unorm: return MTL::PixelFormatBGRA8Unorm;
		case PixelFormat::BGRA8UnormSrgb: return MTL::PixelFormatBGRA8Unorm_sRGB;

		case PixelFormat::R16Float: return MTL::PixelFormatR16Float;
		case PixelFormat::RG16Float: return MTL::PixelFormatRG16Float;
		case PixelFormat::RGBA16Float: return MTL::PixelFormatRGBA16Float;
		case PixelFormat::R32Float: return MTL::PixelFormatR32Float;
		case PixelFormat::RG32Float: return MTL::PixelFormatRG32Float;
		case PixelFormat::RGBA32Float: return MTL::PixelFormatRGBA32Float;

		case PixelFormat::RGB10A2Unorm: return MTL::PixelFormatRGB10A2Unorm;
		case PixelFormat::RG11B10Float: return MTL::PixelFormatRG11B10Float;

		case PixelFormat::Depth16Unorm: return MTL::PixelFormatDepth16Unorm;
		case PixelFormat::Depth32Float: return MTL::PixelFormatDepth32Float;
			// Not present on Apple silicon: the device reports it unsupported and pipeline creation fails, which is the
			// right place for that to be caught - the conversion's job is only to name the format.
		case PixelFormat::Depth24UnormStencil8: return MTL::PixelFormatDepth24Unorm_Stencil8;
		case PixelFormat::Depth32FloatStencil8: return MTL::PixelFormatDepth32Float_Stencil8;

		case PixelFormat::None:
		default:
			return MTL::PixelFormatInvalid;
	}
}

PixelFormat FromMetal(MTL::PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case MTL::PixelFormatR8Unorm: return PixelFormat::R8Unorm;
		case MTL::PixelFormatRG8Unorm: return PixelFormat::RG8Unorm;
		case MTL::PixelFormatRGBA8Unorm: return PixelFormat::RGBA8Unorm;
		case MTL::PixelFormatRGBA8Unorm_sRGB: return PixelFormat::RGBA8UnormSrgb;
		case MTL::PixelFormatBGRA8Unorm: return PixelFormat::BGRA8Unorm;
		case MTL::PixelFormatBGRA8Unorm_sRGB: return PixelFormat::BGRA8UnormSrgb;

		case MTL::PixelFormatR16Float: return PixelFormat::R16Float;
		case MTL::PixelFormatRG16Float: return PixelFormat::RG16Float;
		case MTL::PixelFormatRGBA16Float: return PixelFormat::RGBA16Float;
		case MTL::PixelFormatR32Float: return PixelFormat::R32Float;
		case MTL::PixelFormatRG32Float: return PixelFormat::RG32Float;
		case MTL::PixelFormatRGBA32Float: return PixelFormat::RGBA32Float;

		case MTL::PixelFormatRGB10A2Unorm: return PixelFormat::RGB10A2Unorm;
		case MTL::PixelFormatRG11B10Float: return PixelFormat::RG11B10Float;

		case MTL::PixelFormatDepth16Unorm: return PixelFormat::Depth16Unorm;
		case MTL::PixelFormatDepth32Float: return PixelFormat::Depth32Float;
			// Not present on Apple silicon: the device reports it unsupported and pipeline creation fails, which is the
			// right place for that to be caught - the conversion's job is only to name the format.
		case MTL::PixelFormatDepth24Unorm_Stencil8: return PixelFormat::Depth24UnormStencil8;
		case MTL::PixelFormatDepth32Float_Stencil8: return PixelFormat::Depth32FloatStencil8;

		case MTL::PixelFormatInvalid:
		default:
			return PixelFormat::None;
	}
}

MTL::CompareFunction ToMetal(const CompareFunc compareFunc) {
	switch (compareFunc) {
		case CompareFunc::Never: return MTL::CompareFunctionNever;
		case CompareFunc::Less: return MTL::CompareFunctionLess;
		case CompareFunc::Equal: return MTL::CompareFunctionEqual;
		case CompareFunc::LessEqual: return MTL::CompareFunctionLessEqual;
		case CompareFunc::Greater: return MTL::CompareFunctionGreater;
		case CompareFunc::NotEqual: return MTL::CompareFunctionNotEqual;
		case CompareFunc::GreaterEqual: return MTL::CompareFunctionGreaterEqual;
		default:
		case CompareFunc::Always: return MTL::CompareFunctionAlways;
	}
}

MTL::BlendFactor ToMetal(const BlendFactor blendFactor) {
	switch (blendFactor) {
		case BlendFactor::Zero: return MTL::BlendFactorZero;
		case BlendFactor::One: return MTL::BlendFactorOne;

		case BlendFactor::SrcColor: return MTL::BlendFactorSourceColor;
		case BlendFactor::OneMinusSrcColor: return MTL::BlendFactorOneMinusSourceColor;
		case BlendFactor::DstColor: return MTL::BlendFactorDestinationColor;
		case BlendFactor::OneMinusDstColor: return MTL::BlendFactorOneMinusDestinationColor;

		case BlendFactor::SrcAlpha: return MTL::BlendFactorSourceAlpha;
		case BlendFactor::OneMinusSrcAlpha: return MTL::BlendFactorOneMinusSourceAlpha;
		case BlendFactor::DstAlpha: return MTL::BlendFactorDestinationAlpha;
		case BlendFactor::OneMinusDstAlpha: return MTL::BlendFactorOneMinusDestinationAlpha;

		case BlendFactor::ConstantColor: return MTL::BlendFactorBlendColor;
		case BlendFactor::OneMinusConstantColor: return MTL::BlendFactorOneMinusBlendColor;
		case BlendFactor::ConstantAlpha: return MTL::BlendFactorBlendAlpha;
		case BlendFactor::OneMinusConstantAlpha: return MTL::BlendFactorOneMinusBlendAlpha;

		case BlendFactor::SrcAlphaSaturated: return MTL::BlendFactorSourceAlphaSaturated;
		default:
			return MTL::BlendFactorOne;
	}
}

MTL::CullMode ToMetal(const CullMode cullMode) {
	switch (cullMode) {
		case CullMode::None: return MTL::CullModeNone;
		case CullMode::Front: return MTL::CullModeFront;
		case CullMode::Back: return MTL::CullModeBack;
		default:
			return MTL::CullModeNone;
	}
}

MTL::Winding ToMetal(const FrontFace frontFace) {
	switch (frontFace) {
		case FrontFace::Clockwise: return MTL::WindingClockwise;
		case FrontFace::CounterClockwise:
		default:
			return MTL::WindingCounterClockwise;
	}
}

MTL::LoadAction ToMetal(const LoadAction loadAction) {
	switch (loadAction) {
		case LoadAction::Load: return MTL::LoadActionLoad;
		case LoadAction::Clear: return MTL::LoadActionClear;
		case LoadAction::DontCare:
		default:
			return MTL::LoadActionDontCare;
	}
}

MTL::StoreAction ToMetal(const StoreAction storeAction) {
	switch (storeAction) {
		case StoreAction::Store: return MTL::StoreActionStore;
		case StoreAction::DontCare:
		default:
			return MTL::StoreActionDontCare;
	}
}

MTL::PrimitiveType ToMetalPrimitiveType(const PrimitiveTopology topology) {
	switch (topology) {
		case PrimitiveTopology::PointList: return MTL::PrimitiveTypePoint;
		case PrimitiveTopology::LineList: return MTL::PrimitiveTypeLine;
		case PrimitiveTopology::LineStrip: return MTL::PrimitiveTypeLineStrip;
		case PrimitiveTopology::TriangleList: return MTL::PrimitiveTypeTriangle;
		case PrimitiveTopology::TriangleStrip: return MTL::PrimitiveTypeTriangleStrip;
		case PrimitiveTopology::None:
		default:
			return MTL::PrimitiveTypeTriangle;
	}
}

MTL::PrimitiveTopologyClass ToMetalTopologyClass(const PrimitiveTopology topology) {
	switch (topology) {
		case PrimitiveTopology::PointList: return MTL::PrimitiveTopologyClassPoint;
		case PrimitiveTopology::LineList:
		case PrimitiveTopology::LineStrip: return MTL::PrimitiveTopologyClassLine;
		case PrimitiveTopology::TriangleList:
		case PrimitiveTopology::TriangleStrip: return MTL::PrimitiveTopologyClassTriangle;
		case PrimitiveTopology::None:
		default:
			return MTL::PrimitiveTopologyClassUnspecified;
	}
}

MTL::VertexFormat ToMetal(const ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float: return MTL::VertexFormatFloat;
		case ShaderDataType::Float2: return MTL::VertexFormatFloat2;
		case ShaderDataType::Float3: return MTL::VertexFormatFloat3;
		case ShaderDataType::Float4: return MTL::VertexFormatFloat4;

		case ShaderDataType::Int: return MTL::VertexFormatInt;
		case ShaderDataType::Int2: return MTL::VertexFormatInt2;
		case ShaderDataType::Int3: return MTL::VertexFormatInt3;
		case ShaderDataType::Int4: return MTL::VertexFormatInt4;

		// One byte, read as an integer in the shader. GLSL's bool attribute has no direct Metal twin.
		case ShaderDataType::Bool: return MTL::VertexFormatUChar;

		// A matrix attribute occupies several slots; see the note in the header.
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		case ShaderDataType::None:
		default:
			return MTL::VertexFormatInvalid;
	}
}

}