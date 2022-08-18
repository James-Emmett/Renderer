#pragma once
#include <d3d12.h>
#include "CommandAllocatorPool.h"
#include "System/Types.h"

// This seems overly complicated? should find a way to simplify it
class GraphicsDevice;
class CommandListManager
{
private:
	QueueType						m_Type = QueueType::Direct;
	GraphicsDevice*					m_Device = nullptr;
	CommandAllocatorPool			m_AllocatorPool;

	// Command Lists
	std::queue<CommandList>			m_FreeCommandLists;
	std::vector<std::pair<ID3D12CommandList*,ID3D12CommandAllocator*>>	m_CommandLists;

public:
	void Create(GraphicsDevice* device, QueueType type);
	void ShutDown();

	CommandList Allocate(u64 completedFence);		// The Last completed fence value
	void Release(u64 fenceValue, CommandList cmd);	// Current frame fence value
	ID3D12GraphicsCommandList* GetCommandList(CommandList cmd) { return (ID3D12GraphicsCommandList*)m_CommandLists[cmd].first; }
	u32 Size() { return (u32)m_CommandLists.size(); }
};