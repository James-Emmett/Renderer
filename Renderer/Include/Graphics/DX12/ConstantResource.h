#pragma once
#include "GraphicsResource.h"
#include "Graphics/Common/DescriptorHandle.h"

// Add dynamic support via a ring buffer
class ConstantResource : public GraphicsResource
{
	DescriptorHandle m_ConstantBufferView;
	DescriptorHandle m_ShaderVisibleCBV;

public:
	ConstantResource(GraphicsDevice* device, ResourceDesc description);
	ConstantResource(GraphicsDevice* device, ID3D12Resource* resource, u64 offset = 0, u64 size = 0);

public:
	const DescriptorHandle& ConstantBufferView()const { return m_ConstantBufferView; }
	const DescriptorHandle& ShaderVisibleCBV();

	void Release();
};