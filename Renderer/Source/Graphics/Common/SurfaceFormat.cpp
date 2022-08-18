#pragma once
#include "Graphics/Common/SurfaceFormat.h"
#include "Math/Mathf.h"

namespace TextureHelper
{
	SurfaceFormat	ToSrgb(SurfaceFormat format)
	{
		switch (format)
		{
		case SurfaceFormat::R8G8B8A8_Unorm:
			return SurfaceFormat::R8G8B8A8_Unorm_SRGB;
			break;

		case SurfaceFormat::BC1_Unorm:
			return SurfaceFormat::BC1_Unorm_SRGB;
			break;

		case SurfaceFormat::BC2_Unorm:
			return  SurfaceFormat::BC2_Unorm_SRGB;
			break;

		case SurfaceFormat::BC3_Unorm:
			return SurfaceFormat::BC3_Unorm_SRGB;
			break;

		case SurfaceFormat::B8G8R8A8_Unorm:
			return SurfaceFormat::B8G8R8A8_Unorm_SRGB;
			break;

		case SurfaceFormat::BC7_Unorm:
			return SurfaceFormat::BC7_Unorm_SRGB;
			break;

		default:
			return SurfaceFormat::Unkown;
			break;
		}
	}

	SurfaceFormat	ToLinear(SurfaceFormat format)
	{
		switch (format)
		{
		case SurfaceFormat::R8G8B8A8_Unorm_SRGB:
			return SurfaceFormat::R8G8B8A8_Unorm;
			break;

		case SurfaceFormat::BC1_Unorm_SRGB:
			return SurfaceFormat::BC1_Unorm;
			break;

		case SurfaceFormat::BC2_Unorm_SRGB:
			return  SurfaceFormat::BC2_Unorm;
			break;

		case SurfaceFormat::BC3_Unorm_SRGB:
			return SurfaceFormat::BC3_Unorm;
			break;

		case SurfaceFormat::B8G8R8A8_Unorm_SRGB:
			return SurfaceFormat::B8G8R8A8_Unorm;
			break;

		case SurfaceFormat::BC7_Unorm_SRGB:
			return SurfaceFormat::BC7_Unorm;
			break;

		default:
			return SurfaceFormat::Unkown;
			break;
		}
	}

	bool IsDepthFormat(SurfaceFormat format)
	{
		switch (format)
		{
		case SurfaceFormat::R32G8X24_Typeless:
		case SurfaceFormat::D32_Float_S8X24_Uint:
		case SurfaceFormat::D32_Float:
		case SurfaceFormat::R24G8_Typeless:
		case SurfaceFormat::D24_Unorm_S8_Uint:
			return true;

		case SurfaceFormat::Unkown:
		default:
			return false;
			break;
		}
	}

	bool IsSRGBFormat(SurfaceFormat format)
	{
		switch (format)
		{
			case SurfaceFormat::R8G8B8A8_Unorm_SRGB:
			case SurfaceFormat::BC1_Unorm_SRGB:
			case SurfaceFormat::BC2_Unorm_SRGB:
			case SurfaceFormat::BC3_Unorm_SRGB:
			case SurfaceFormat::B8G8R8A8_Unorm_SRGB:
			case SurfaceFormat::BC7_Unorm_SRGB:
				return true;
			break;

		default:
			return false;
			break;
		}
	}

	bool IsCompressed(SurfaceFormat format)
	{
		switch (format)
		{
		case SurfaceFormat::BC1_Unorm:
		case SurfaceFormat::BC1_Unorm_SRGB:
		case SurfaceFormat::BC2_Unorm:
		case SurfaceFormat::BC2_Unorm_SRGB:
		case SurfaceFormat::BC3_Unorm:
		case SurfaceFormat::BC3_Unorm_SRGB:
		case SurfaceFormat::BC4_Typeless:
		case SurfaceFormat::BC4_Unorm:
		case SurfaceFormat::BC4_Snorm:
		case SurfaceFormat::BC5_Unorm:
		case SurfaceFormat::BC5_Snorm:
		case SurfaceFormat::B8G8R8A8_Unorm:
		case SurfaceFormat::B8G8R8A8_Unorm_SRGB:
		case SurfaceFormat::BC6H_UF16:
		case SurfaceFormat::BC6H_SF16:
		case SurfaceFormat::BC7_Unorm:
		case SurfaceFormat::BC7_Unorm_SRGB:
			return true;

		case SurfaceFormat::Unkown:
		default:
			return false;
			break;
		}
	}

	u32	BytesPerBlock(SurfaceFormat format)
	{
		switch (format)
		{
		case SurfaceFormat::Unkown:
			return 0;
			break;

		case SurfaceFormat::R32G32B32A32_Float:
		case SurfaceFormat::R32G32B32A32_Uint:
		case SurfaceFormat::R32G32B32A32_Sint:
		case SurfaceFormat::BC2_Unorm:
		case SurfaceFormat::BC2_Unorm_SRGB:
		case SurfaceFormat::BC3_Unorm:
		case SurfaceFormat::BC3_Unorm_SRGB:
		case SurfaceFormat::BC5_Unorm:
		case SurfaceFormat::BC5_Snorm:
		case SurfaceFormat::BC6H_UF16:
		case SurfaceFormat::BC6H_SF16:
		case SurfaceFormat::BC7_Unorm:
		case SurfaceFormat::BC7_Unorm_SRGB:
			return 16;
			break;

		case SurfaceFormat::R32G32B32_Float:
		case SurfaceFormat::R32G32B32_Uint:
		case SurfaceFormat::R32G32B32_Sint:
			return 12;
			break;

		case SurfaceFormat::R16G16B16A16_Float:
		case SurfaceFormat::R16G16B16A16_Unorm:
		case SurfaceFormat::R16G16B16A16_Uint:
		case SurfaceFormat::R16G16B16A16_Snorm:
		case SurfaceFormat::R16G16B16A16_Sint:
		case SurfaceFormat::R32G32_Float:
		case SurfaceFormat::R32G32_Uint:
		case SurfaceFormat::R32G32_Sint:
		case SurfaceFormat::R32G8X24_Typeless:
		case SurfaceFormat::BC1_Unorm:
		case SurfaceFormat::BC1_Unorm_SRGB:
			return 8;
			break;

		case SurfaceFormat::D32_Float_S8X24_Uint:
		case SurfaceFormat::R10G10B10A2_Unorm:
		case SurfaceFormat::R10G10B10A2_Uint:
		case SurfaceFormat::R11G11B10_Float:
		case SurfaceFormat::R8G8B8A8_Unorm:
		case SurfaceFormat::R8G8B8A8_Unorm_SRGB:
		case SurfaceFormat::R8G8B8A8_Uint:
		case SurfaceFormat::R8G8B8A8_Snorm:
		case SurfaceFormat::R8G8B8A8_Typeless:
		case SurfaceFormat::R8G8B8A8_Sint:
		case SurfaceFormat::R16G16_Float:
		case SurfaceFormat::R16G16_Unorm:
		case SurfaceFormat::R16G16_Uint:
		case SurfaceFormat::R16G16_Snorm:
		case SurfaceFormat::R16G16_Sint:
		case SurfaceFormat::R32_Typeless:
		case SurfaceFormat::D32_Float:
		case SurfaceFormat::R32_Float:
		case SurfaceFormat::R32_Uint:
		case SurfaceFormat::R32_Sint:
		case SurfaceFormat::R24G8_Typeless:
		case SurfaceFormat::D24_Unorm_S8_Uint:
		case SurfaceFormat::BC4_Unorm:
		case SurfaceFormat::BC4_Snorm:
		case SurfaceFormat::B8G8R8A8_Unorm:
		case SurfaceFormat::B8G8R8A8_Unorm_SRGB:
			return 4;
			break;

		case SurfaceFormat::R8G8_Unorm:
		case SurfaceFormat::R8G8_Uint:
		case SurfaceFormat::R8G8_Snorm:
		case SurfaceFormat::R8G8_Sint:
		case SurfaceFormat::R16_Typeless:
		case SurfaceFormat::R16_Float:
		case SurfaceFormat::D16_Unorm:
		case SurfaceFormat::R16_Unorm:
		case SurfaceFormat::R16_Uint:
		case SurfaceFormat::R16_Snorm:
		case SurfaceFormat::R16_Sint:
			return 2;
			break;

		case SurfaceFormat::R8_Typeless:
		case SurfaceFormat::R8G8_Typeless:
		case SurfaceFormat::R8_Unorm:
		case SurfaceFormat::R8_Uint:
		case SurfaceFormat::R8_Snorm:
		case SurfaceFormat::R8_Sint:
			return 1;
			break;

		default:
			return 0;
			break;
		}
	}

	u32	PitchSize(SurfaceFormat format, u32 width)
	{
		switch (format)
		{
		case SurfaceFormat::BC1_Unorm:
		case SurfaceFormat::BC1_Unorm_SRGB:
		case SurfaceFormat::BC2_Unorm:
		case SurfaceFormat::BC2_Unorm_SRGB:
		case SurfaceFormat::BC3_Unorm:
		case SurfaceFormat::BC3_Unorm_SRGB:
			// Block aligned pitch
			return (width + 3 / 4) * BytesPerBlock(format);
			break;

		default:
			return BytesPerBlock(format) * width;
			break;
		}
	}

	u32	CalculateSurfaceSize(SurfaceFormat format, u32 width, u32 height)
	{
		return (BytesPerBlock(format) * width) * height;
	}

	u32	CalculateMipCount(u32 width, u32 height)
	{
		// Ceil to keep the partial mip? pad somehow?
		return (u32)Mathf::Ceil(Mathf::Log2((float)Mathf::Max(width, height)));
	}

	u32	CalculateTotalBytes(SurfaceFormat format, u32 width, u32 height, u32 depth, u32 mips)
	{
		u32 totalBytes = 0;

		// This could be improved? mips are 1.33 times the size, but can we guarentee that?
		for (u32 i = 0; i < mips; ++i)
		{
			totalBytes += CalculateSurfaceSize(format, width >> i, height >> i) * (depth >> i);
		}

		return totalBytes;
	}
};