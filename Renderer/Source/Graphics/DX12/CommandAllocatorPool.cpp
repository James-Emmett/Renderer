#include "Graphics/DX12/CommandAllocatorPool.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "System/Assert.h"

CommandAllocatorPool::~CommandAllocatorPool()
{
	ShutDown();
}

void CommandAllocatorPool::Create(GraphicsDevice* device, QueueType type)
{
	m_Device = device;
	m_Type = type;
}

void CommandAllocatorPool::ShutDown()
{
	for (size_t i = 0; i < m_Allocators.size(); ++i)
	{
		m_Allocators[i]->Release();
	}

	m_Allocators.clear();
}

ID3D12CommandAllocator* CommandAllocatorPool::Allocate(u64 completedFence)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	ID3D12CommandAllocator* allocator = nullptr;

	// Try get a free command list
	if (m_FreeAllocators.empty() == false)
	{
		std::pair<u64, ID3D12CommandAllocator*>& pair = m_FreeAllocators.front();

		// Make sure the work is completed
		if (pair.first <= completedFence)
		{
			allocator = pair.second;
			assert(SUCCEEDED(pair.second->Reset()));
			m_FreeAllocators.pop();
		}
	}

	// Create a new Allocator as we ran out
	if (allocator == nullptr)
	{
		assert(SUCCEEDED(m_Device->NativeDevice()->CreateCommandAllocator(ConversionTypes::ConvertCommandListType(m_Type), IID_PPV_ARGS(&allocator))));
		wchar_t name[32];
		swprintf(name, 32, L"Command Allocator %u", (u32)m_Allocators.size());
		allocator->SetName(name);
		m_Allocators.push_back(allocator);
	}

	return allocator;
}

void CommandAllocatorPool::FreeAllocator(u64 fenceValue, ID3D12CommandAllocator* allocator)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_FreeAllocators.push(std::make_pair(fenceValue, allocator));
}
