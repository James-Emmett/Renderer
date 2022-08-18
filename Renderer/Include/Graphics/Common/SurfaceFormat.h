#pragma once
#include "System/Types.h"

enum class SurfaceFormat
{
	Unkown,
	R32G32B32A32_Float,
	R32G32B32A32_Uint,
	R32G32B32A32_Sint,

	R32G32B32_Float,
	R32G32B32_Uint,
	R32G32B32_Sint,

	R16G16B16A16_Float,
	R16G16B16A16_Unorm,
	R16G16B16A16_Uint,
	R16G16B16A16_Snorm,
	R16G16B16A16_Sint,

	R32G32_Float,
	R32G32_Uint,
	R32G32_Sint,

	R32G8X24_Typeless,
	D32_Float_S8X24_Uint,

	R10G10B10A2_Unorm,
	R10G10B10A2_Uint,
	R11G11B10_Float,

	R8G8B8A8_Typeless,
	R8G8B8A8_Unorm,
	R8G8B8A8_Unorm_SRGB,
	R8G8B8A8_Uint,
	R8G8B8A8_Snorm,
	R8G8B8A8_Sint,

	R16G16_Typeless,
	R16G16_Float,
	R16G16_Unorm,
	R16G16_Uint,
	R16G16_Snorm,
	R16G16_Sint,

	R32_Typeless,
	D32_Float,
	R32_Float,
	R32_Uint,
	R32_Sint,

	R24G8_Typeless,
	D24_Unorm_S8_Uint,

	R8G8_Typeless,
	R8G8_Unorm,
	R8G8_Uint,
	R8G8_Snorm,
	R8G8_Sint,
	R16_Typeless,
	R16_Float,
	D16_Unorm,
	R16_Unorm,
	R16_Uint,
	R16_Snorm,
	R16_Sint,

	R8_Typeless,
	R8_Unorm,
	R8_Uint,
	R8_Snorm,
	R8_Sint,

	BC1_Unorm,
	BC1_Unorm_SRGB,

	BC2_Unorm,
	BC2_Unorm_SRGB,

	BC3_Unorm,
	BC3_Unorm_SRGB,

	BC4_Typeless,
	BC4_Unorm,
	BC4_Snorm,

	BC5_Unorm,
	BC5_Snorm,

	B8G8R8A8_Unorm,
	B8G8R8A8_Unorm_SRGB,

	BC6H_UF16,
	BC6H_SF16,

	BC7_Unorm,
	BC7_Unorm_SRGB,
};

enum class RenderFormat
{
	R8G8B8A8_Unorm		= (u32)SurfaceFormat::B8G8R8A8_Unorm,
	R10G10A2_Unorm		= (u32)SurfaceFormat::R10G10B10A2_Unorm,
	R16G16B16A16_Float	= (u32)SurfaceFormat::R16G16B16A16_Float,
	R32G32B32A32_Float	= (u32)SurfaceFormat::R32G32B32A32_Float,
};

enum class DepthFormat
{
	Depth16				= (u32)SurfaceFormat::R16_Typeless,
	Depth24Stencil8		= (u32)SurfaceFormat::R24G8_Typeless,
	Depth32				= (u32)SurfaceFormat::R32_Typeless,
	Depth24Stencil8x24	= (u32)SurfaceFormat::R32G8X24_Typeless
};

enum class IndexFormat
{
	I16 = (u32)SurfaceFormat::R16_Uint,
	I32 = (u32)SurfaceFormat::R32_Uint
};

namespace TextureHelper
{
	SurfaceFormat	ToSrgb(SurfaceFormat format);
	SurfaceFormat	ToLinear(SurfaceFormat format);
	bool			IsDepthFormat(SurfaceFormat format);
	bool			IsSRGBFormat(SurfaceFormat format);
	bool			IsCompressed(SurfaceFormat format);
	u32				BytesPerBlock(SurfaceFormat format);
	u32				PitchSize(SurfaceFormat format, u32 width);
	u32				CalculateSurfaceSize(SurfaceFormat format, u32 width, u32 height = 1);
	u32				CalculateMipCount(u32 width, u32 height);
	u32				CalculateTotalBytes(SurfaceFormat format, u32 width, u32 height, u32 depth = 1, u32 mips = 1);
};