#pragma once
#include <d3d12.h>
#include "Graphics/Graphics.h"

namespace ConversionTypes
{
	// Conversion Types
	D3D12_HEAP_TYPE					ConvertHeapType(HeapType type);
	D3D12_DESCRIPTOR_HEAP_TYPE		ConvertDescHeapType(DescHeapType type);
	DXGI_FORMAT						ConvertFormat(SurfaceFormat format);
	D3D12_FILTER					ConvertFilter(FilterMode filter);
	D3D12_TEXTURE_ADDRESS_MODE		ConvertAddressMode(WrapMode mode);
	D3D12_COMPARISON_FUNC			ConvertComparison(ComparisonFunc comparisonFunc);
	D3D12_STENCIL_OP				ConvertStencilOp(StencilOp stencilOp);
	D3D12_BLEND						ConvertBlend(Blend blend);
	D3D12_BLEND_OP					ConvertBlendOp(BlendOp blendOp);
	D3D12_FILL_MODE					ConvertFillMode(FillMode fillMode);
	D3D12_CULL_MODE					ConvertCullMode(CullMode cullMode);
	D3D12_DEPTH_WRITE_MASK			ConvertDepthWrite(DepthWrite depthWrite);
	D3D12_COLOR_WRITE_ENABLE		ConvertBlendWrite(ColorWrite colorWrite);
	D3D12_INPUT_CLASSIFICATION		ConvertInputClass(InputType inputType);
	D3D12_PRIMITIVE_TOPOLOGY_TYPE	ConvertPipeTopology(PrimitiveTopology primitiveTopology);
	D3D12_PRIMITIVE_TOPOLOGY		ConvertTopology(PrimitiveTopology primitiveTopology);
	D3D12_RESOURCE_STATES			ConvertResourceState(ResourceState resourceState);
	D3D12_RESOURCE_DIMENSION		ConvertResourceDimension(ResourceDimension dimension);
	D3D12_TEXTURE_LAYOUT			ConvertTextureLayout(TextureLayout layout);
	D3D12_COMMAND_LIST_TYPE			ConvertCommandListType(QueueType type);
}