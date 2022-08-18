#pragma once
#include "SurfaceFormat.h"
#include "MultiSample.h"

enum class ResourceDimension
{
	Unkown,
	Buffer,
	Texture1D,
	Texture2D,
	Texture3D,
	TextureCube
};

enum class TextureLayout
{
	Unkown,
	RowMajor,
	UndefinedSwizzle,
	StandardSwizzle
};

enum class HeapType
{
	Default,
	Upload,
	ReadBack,
	Custom
};

enum class ResourceState
{
	Common = 0,
	Vertex_And_Constant_Buffer,
	IndexBuffer,
	RenderTarget,
	UnorderedAccess,
	DepthWrite,
	DepthRead,
	NonPixelShaderResource,
	PixelShaderResource,
	StreamOut,
	IndirectArgument,
	CopyDest,
	CopySource,
	ResolveDest,
	ResolveSource,
	RaytracingAcceleration,
	ShadingRateResource,
	GenericRead,
	Present = 0,
	Predication,
};

enum class BindFlag
{
	None = 0,
	VertexBuffer = 1 << 0,
	IndexBuffer = 1 << 1,
	ConstantBuffer = 1 << 2,
	ShaderResource = 1 << 3,
	RenderTarget = 1 << 4,
	DepthStencil = 1 << 5,
	UnorderedAccess = 1 << 6,
};

struct ResourceDesc
{
	HeapType			Heap				= HeapType::Default;
	ResourceDimension	Dimension			= ResourceDimension::Unkown;
	u64					Alignment			= 0;
	u64					Width				= 0;
	u32					Height				= 0;
	u16					DepthOrArraySize	= 1;
	u16					MipCount			= 1;
	SurfaceFormat		Format				= SurfaceFormat::Unkown;
	MultiSampleDesc		MultiSample;
	TextureLayout		Layout				= TextureLayout::Unkown;
	u32					Flags				= 0;
	u32					Stride				= 0;

	ResourceDesc() {}
	ResourceDesc(HeapType heap, ResourceDimension dimension, u64 alignment, u64 width, u32 height, u16 depth, u16 mips, u32 stride, SurfaceFormat format, u32 count, u32 quality, TextureLayout layout, u32 flags)
	{
		Heap = heap;
		Dimension = dimension;
		Alignment = alignment;
		Width = width;
		Height = height;
		DepthOrArraySize = depth;
		MipCount = mips;
		Format = format;
		MultiSample.Count = count;
		MultiSample.Quality = quality;
		Layout = layout;
		Flags = flags;
		Stride = stride;
	}

	static ResourceDesc Buffer(HeapType heap, u64 size, u32 stride, u32 flags = (u32)BindFlag::None)
	{
		return ResourceDesc(heap, ResourceDimension::Buffer, 0, size, 1, 1, 1, stride, SurfaceFormat::Unkown, 1, 0, TextureLayout::RowMajor, flags);
	}

	static ResourceDesc Tex1D(HeapType heap, SurfaceFormat format, u64 width, u16 array = 1, u16 mips = 1, TextureLayout layout = TextureLayout::Unkown, u32 flags = (u32)BindFlag::None)
	{
		return ResourceDesc(heap, ResourceDimension::Texture1D, 0, width, 1, array, mips, TextureHelper::PitchSize(format, (u32)width), format, 1, 0, layout, flags);
	}

	static ResourceDesc Tex2D(HeapType heap, SurfaceFormat format, u64 width, u32 height, u16 array = 1, u16 mips = 1, TextureLayout layout  = TextureLayout::Unkown, u32 flags = (u32)BindFlag::None)
	{
		return ResourceDesc(heap, ResourceDimension::Texture2D, 0, width, height, array, mips, TextureHelper::PitchSize(format, (u32)width), format, 1, 0, layout, flags);
	}

	static ResourceDesc Tex3D(HeapType heap, SurfaceFormat format, u64 width, u32 height, u16 depth, u16 mips = 1, TextureLayout layout = TextureLayout::Unkown, u32 flags = (u32)BindFlag::None)
	{
		return ResourceDesc(heap, ResourceDimension::Texture3D, 0, width, height, depth, mips, TextureHelper::PitchSize(format, (u32)width), format,  1, 0, layout, flags);
	}
};

