#include "Graphics/DX12/TextureResource.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "Math/Mathf.h"

TextureResource::TextureResource(GraphicsDevice* device, ResourceDesc desc): GraphicsResource(device, desc)
{
	// Create the views
	if (Mathf::HasBitSet(desc.Flags, (u32)BindFlag::RenderTarget))
	{
		m_RenderTargetView = m_Device->CreateRenderTargetView(desc, m_Resource);
	}
	else if (Mathf::HasBitSet(desc.Flags, (u32)BindFlag::DepthStencil))
	{
		m_DepthStencilView = m_Device->CreateDepthStencilView(desc, m_Resource);
	}
	
	// Creates a CPU visible Shader Resource View Handle, 
	if (Mathf::HasBitSet(desc.Flags, (u32)BindFlag::ShaderResource))
	{
		m_ShaderResourceView = m_Device->CreateShaderResourceView(desc, m_Resource);
	}
}

TextureResource::TextureResource(GraphicsDevice* device, ID3D12Resource* resource): GraphicsResource(device, resource)
{
}

DescriptorHandle& TextureResource::ShaderResourceView()
{
	return m_ShaderResourceView;
}

DescriptorHandle& TextureResource::ShaderResourceViewGPUVisible()
{
	// If its invalid or we are upload (tempoary SRV) then make a new one
	if (m_ShaderVisibleSRV.IsValid() == false || m_HeapType == HeapType::Upload)
	{
		m_ShaderVisibleSRV = m_Device->CopyDescriptors(m_Device->MemoryHandler()->m_ShaderVisibleResourceHeap, m_ShaderResourceView, 1);
	}

	return m_ShaderVisibleSRV;
}

DescriptorHandle& TextureResource::RenderTargetView()
{
	return m_RenderTargetView;
}

DescriptorHandle& TextureResource::DepthStencilView()
{
	return m_DepthStencilView;
}

void TextureResource::Release()
{
	GraphicsResource::Release();

	if (m_RenderTargetView.IsValid())
	{
		m_Device->MemoryHandler()->Release(m_RenderTargetView, DescHeapType::RenderTargetView);
	}

	if (m_DepthStencilView.IsValid())
	{
		m_Device->MemoryHandler()->Release(m_DepthStencilView, DescHeapType::DepthStencilView);
	}
	
}
