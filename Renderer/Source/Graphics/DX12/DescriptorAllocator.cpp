#include "Graphics/DX12/DescriptorAllocator.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include <assert.h>

DescriptorHeap::DescriptorHeap()
{

}

DescriptorHeap::DescriptorHeap(DescriptorHeap&& other) noexcept
{
	m_HeapType			= other.m_HeapType;
	m_Size				= other.m_Size;
	m_CpuStart			= other.m_CpuStart;
	m_GpuStart			= other.m_GpuStart;
	m_DescriptorSize	= other.m_DescriptorSize;
	m_Heap				= other.m_Heap;

	// Clear other important bits.
	other.m_Heap = nullptr;
	other.m_CpuStart = D3D12_CPU_DESCRIPTOR_HANDLE();
	other.m_GpuStart = D3D12_GPU_DESCRIPTOR_HANDLE();
}

void DescriptorHeap::Initialize(GraphicsDevice* device, DescHeapType type, u32 count, bool shaderVisible)
{
	m_HeapType = type;
	m_D3DHeapType = ConversionTypes::ConvertDescHeapType(m_HeapType);
	m_ShaderVisible = shaderVisible;

	ID3D12Device* nativeDevice = device->NativeDevice();
	m_Size = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = ConversionTypes::ConvertDescHeapType(type);
	desc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NumDescriptors = count;

	// Create the descriptor heap
	assert(SUCCEEDED(nativeDevice->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), (void**)&m_Heap)));

	m_CpuStart = m_Heap->GetCPUDescriptorHandleForHeapStart();
	m_GpuStart = (shaderVisible)? m_Heap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE();

	m_DescriptorSize = nativeDevice->GetDescriptorHandleIncrementSize(desc.Type);
}

DescriptorHandle DescriptorHeap::Allocate()
{
	// Return handle but increment count
	return GetHandle(m_Count++);
}

DescriptorHandle DescriptorHeap::GetHandle(u32 index) const
{
	DescriptorHandle handle = DescriptorHandle(m_CpuStart.ptr + m_DescriptorSize * index, m_ShaderVisible ? m_GpuStart.ptr + m_DescriptorSize * index : 0, m_HeapType);
	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUStart() const
{
	return m_CpuStart;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUStart() const
{
	return m_GpuStart;
}

bool DescriptorHeap::IsNull() const
{
	return m_CpuStart.ptr == 0;
}

bool DescriptorHeap::IsShaderVisible()const
{
	return m_ShaderVisible;
}

u32 DescriptorHeap::DesciptorSize() const
{
	return m_DescriptorSize;
}

void DescriptorHeap::Reset()
{
	m_Count = 0;
}

void DescriptorHeap::Dispose()
{
	if (m_Heap)
	{
		m_Heap->Release();
	}
}

ID3D12DescriptorHeap* DescriptorHeap::NativeHeapHandle() const
{
	return m_Heap;
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::GetHeapType() const
{
	return m_D3DHeapType;
}

void DescriptorAllocator::Initialize(GraphicsDevice* device, u32 countPerHeap, DescHeapType type, bool shaderVisible)
{
	m_Device = device;
	m_Type = type;
	m_ShaderVisible = shaderVisible;
	m_CountPerHeap = countPerHeap;
	m_DescriptorSize = m_Device->NativeDevice()->GetDescriptorHandleIncrementSize(ConversionTypes::ConvertDescHeapType(m_Type));
}

DescriptorHandle DescriptorAllocator::Allocate()
{
	std::lock_guard<std::mutex> lock(m_Lock);
	if (m_FreeList.empty()) { AllocateHeap(); }
	DescriptorHandle handle = m_FreeList.back();
	m_FreeList.pop_back();

	return handle;
}

void DescriptorAllocator::Release(DescriptorHandle index)
{
	std::lock_guard<std::mutex> lock(m_Lock);
	m_FreeList.push_back(index);
}


void DescriptorAllocator::ShutDown()
{
	// Clear should trigger shudown destructors
	m_Heaps.clear();
}

void DescriptorAllocator::AllocateHeap()
{
	// Create descriptor heap and fill up free list
	m_Heaps.emplace_back();
	DescriptorHeap* heap = &m_Heaps.back();
	heap->Initialize(m_Device, m_Type, m_CountPerHeap, m_ShaderVisible);
	for (u32 i = 0; i < m_CountPerHeap; ++i)
	{
		m_FreeList.push_back(heap->Allocate());
	}
}
