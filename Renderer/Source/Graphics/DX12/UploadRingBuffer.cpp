#include "Graphics/DX12/UploadRingBuffer.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "System/Assert.h"
#include "Math/Mathf.h"

UploadRingBuffer::UploadRingBuffer() {}

void UploadRingBuffer::Initialise(GraphicsDevice* device, u32 maxSize, bool cpuAccess)
{
	m_Device = device;
	m_MaxSize = maxSize;

	// Create a resource heap of the requested size with either CPU access i.e constant buffer
	// or no cpu access for stuff like UAV
	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask		= 1;
	heapProp.VisibleNodeMask		= 1;

	D3D12_RESOURCE_DESC resDesc		= {};
	resDesc.Dimension				= D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Alignment				= 0;
	resDesc.Width					= m_MaxSize;
	resDesc.Height					= 1;
	resDesc.DepthOrArraySize		= 1;
	resDesc.MipLevels				= 1;
	resDesc.Format					= DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count		= 1;
	resDesc.SampleDesc.Quality		= 0;
	resDesc.Layout					= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_STATES resState;
	if (cpuAccess)
	{
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		// Change this eventually its not optimised according to Nvidia
		// Need a more specific state than concatination of a bunch.
		resState = D3D12_RESOURCE_STATE_GENERIC_READ;
	}
	else
	{
		heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	}

	// Create the heap to store our dynamic allocations
	m_Device->NativeDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, resState, nullptr, IID_PPV_ARGS(&m_Resource));

	// Grab the GPU address
	m_GpuVirtualAddress = m_Resource->GetGPUVirtualAddress();

	// Map the Cpu for writing into buffer
	if (cpuAccess)
	{
		m_Resource->Map(0, nullptr, &m_CpuVirtualAddress);
	}
}

void UploadRingBuffer::FinishFrame(u64 fence)
{
	m_FrameTailQueue.emplace(fence, m_Tail, m_CurrentFramesize);
	m_CurrentFramesize = 0;
}

std::shared_ptr<GraphicsResource> UploadRingBuffer::Allocate(u64 byteSize, u64 alignment)
{
	u64 offset = 0;

	if (IsFull())
	{
		return nullptr;
	}

	u64 byteAlignedTail = Mathf::AlignSize(m_Tail, alignment);
	u64 byteSizeWithPadding = (byteAlignedTail - m_Tail) + byteSize;

	if (byteAlignedTail >= m_Head)
	{
		if (byteAlignedTail + byteSize <= m_MaxSize)
		{
			offset = byteAlignedTail;
			m_Used += byteSizeWithPadding;
			m_Tail += byteSizeWithPadding;
			m_CurrentFramesize += byteSizeWithPadding;
		}
		else if (byteSizeWithPadding <= m_Head)
		{
			// Allocate from start as it overlaps end otherwise
			// But only if enough space before head
			u64 AddSize = (m_MaxSize - m_Tail) + byteSize;
			m_Used += AddSize;
			m_CurrentFramesize += AddSize;
			m_Tail = byteSize;
			offset = 0;
		}
	}
	else if (byteAlignedTail + byteSize <= m_Head)
	{
		// Tail wrapped, check less than head
		offset = byteAlignedTail;
		m_Tail += byteSizeWithPadding;
		m_Used += byteSizeWithPadding;
		m_CurrentFramesize += byteSizeWithPadding;
	}
	else
	{
		assert(false && "Ring Buffer Overflow");
		return nullptr;
	}

	return std::make_shared<GraphicsResource>(m_Device, m_Resource, HeapType::Upload, offset, byteSize, true);
}

std::shared_ptr<GraphicsResource> UploadRingBuffer::AllocateTextureMemory(u64 byteSize)
{
	return Allocate(byteSize, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
}

void UploadRingBuffer::ReleaseCompletedFrames(u64 compeletedFenceValue)
{
	// Free memory untill we are at current fence value.
	while (m_FrameTailQueue.empty() == false && m_FrameTailQueue.front().m_FenceValue <= compeletedFenceValue)
	{
		FrameAttribute& tail = m_FrameTailQueue.front();
		m_Used -= tail.m_Size; // Decrement the tails size
		m_Head = tail.m_StartIndex; // Move head to this tails value
		m_FrameTailQueue.pop();
	}
}

void UploadRingBuffer::Dispose()
{
	if (m_Resource)
	{
		m_Device->MemoryHandler()->Release(m_Resource);
	}
}
