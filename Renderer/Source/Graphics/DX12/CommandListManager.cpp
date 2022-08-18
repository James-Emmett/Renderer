#include "Graphics/DX12/CommandListManager.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"

void CommandListManager::Create(GraphicsDevice* device, QueueType type)
{
	m_Device = device;
	m_Type = type;
	m_AllocatorPool.Create(device, type);
}

void CommandListManager::ShutDown()
{
	m_AllocatorPool.ShutDown();

	for (size_t i = 0; i < m_CommandLists.size(); ++i)
	{
		m_CommandLists[i].first->Release();
	}
}

CommandList CommandListManager::Allocate(u64 completedFence)
{
	ID3D12CommandAllocator* allocator = m_AllocatorPool.Allocate(completedFence);

	// Try use a free commandlist
	if (m_FreeCommandLists.empty() == false)
	{
		CommandList index = m_FreeCommandLists.front();
		((ID3D12GraphicsCommandList*)m_CommandLists[index].first)->Reset(allocator, nullptr);
		m_CommandLists[index].second = allocator;
		m_FreeCommandLists.pop();
		return index;
	}
	else // Create a new one
	{
		m_CommandLists.emplace_back(nullptr, allocator);
		m_Device->NativeDevice()->CreateCommandList(0, ConversionTypes::ConvertCommandListType(m_Type), allocator, nullptr, IID_PPV_ARGS(&m_CommandLists.back().first));
		return (CommandList)(m_CommandLists.size() - 1);
	}
}

void CommandListManager::Release(u64 fenceValue, CommandList cmd)
{
	m_FreeCommandLists.push(cmd);
	m_AllocatorPool.FreeAllocator(fenceValue, m_CommandLists[cmd].second);
	m_CommandLists[cmd].second = nullptr;
}
