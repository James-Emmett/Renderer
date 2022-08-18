#include "Graphics/DX12/ConstantResource.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "Math/Mathf.h"

ConstantResource::ConstantResource(GraphicsDevice* device, ResourceDesc description) : GraphicsResource(device, description)
{
	m_ConstantBufferView = m_Device->MemoryHandler()->m_ResourceDescriptorHeap.Allocate();
	// Static constant buffer, please update this irregualry
	D3D12_CPU_DESCRIPTOR_HANDLE handle = {};
	handle.ptr = (SIZE_T)(m_ConstantBufferView.m_Cpu);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = m_Resource->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)m_Size;

	m_Device->NativeDevice()->CreateConstantBufferView(&cbvDesc, handle);
}

ConstantResource::ConstantResource(GraphicsDevice* device, ID3D12Resource* resource, u64 offset, u64 size) : GraphicsResource(device, resource, HeapType::Default, offset, size)
{
	m_ConstantBufferView = m_Device->MemoryHandler()->m_ResourceDescriptorHeap.Allocate();
	// Static constant buffer, please update this irregualry
	D3D12_CPU_DESCRIPTOR_HANDLE handle = {};
	handle.ptr = (SIZE_T)m_ConstantBufferView.m_Cpu;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = m_Resource->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)(Mathf::AlignSize(size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT));

	m_Size = cbvDesc.SizeInBytes;

	m_Device->NativeDevice()->CreateConstantBufferView(&cbvDesc, handle);
}

const DescriptorHandle& ConstantResource::ShaderVisibleCBV()
{
	// If its invalid or we are upload (tempoary CBV) then make a new one
	if (m_ShaderVisibleCBV.IsValid() == false || m_HeapType == HeapType::Upload)
	{
		m_ShaderVisibleCBV = m_Device->CopyDescriptors(m_Device->MemoryHandler()->m_ShaderVisibleResourceHeap, m_ConstantBufferView, 1);
	}

	return m_ShaderVisibleCBV;
}

void ConstantResource::Release()
{
	GraphicsResource::Release();

	if (m_ConstantBufferView.IsValid())
	{
		m_Device->MemoryHandler()->Release(m_ConstantBufferView, DescHeapType::Resource);
	}
}
