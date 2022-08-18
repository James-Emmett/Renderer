#pragma once
#include "GraphicsResource.h"
#include "Graphics/Common/DescriptorHandle.h"

class TextureResource : public GraphicsResource
{
	DescriptorHandle m_ShaderResourceView;
	DescriptorHandle m_RenderTargetView;
	DescriptorHandle m_DepthStencilView;

	// Is this the right way to handle texture shader resource views?
	DescriptorHandle m_ShaderVisibleSRV;

public:
	TextureResource(GraphicsDevice* device, ResourceDesc desc);
	// Special type for already created
	TextureResource(GraphicsDevice* device, ID3D12Resource* resource);

public:
	DescriptorHandle&	ShaderResourceView();
	// This will create/copy your SRV to Shader visible heap if it isnt already!
	DescriptorHandle&	ShaderResourceViewGPUVisible();
	DescriptorHandle&	RenderTargetView();
	DescriptorHandle&	DepthStencilView();
	u32					Height()const { return m_Desc.Height; }
	u32					Depth()const { return m_Desc.DepthOrArraySize; }
	u32					MipCount()const { return m_Desc.MipCount; }
	SurfaceFormat		Format()const { return m_Desc.Format; }
	ResourceDimension	Dimension() { return m_Desc.Dimension; }
	virtual void		Release();
};