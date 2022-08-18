#pragma once
#include "System/Types.h"
#include "Graphics/Common/QueueType.h"
#include <d3d12.h>
#include <vector>
#include <queue>
#include <mutex>

class GraphicsDevice;
// Simple pool of command allocators, saves having redundant allocators if a commandlist thread
// goes unused in one frame.
class CommandAllocatorPool
{
private:
	QueueType m_Type;

	GraphicsDevice* m_Device;
	std::vector<ID3D12CommandAllocator*> m_Allocators;
	std::queue<std::pair<u64, ID3D12CommandAllocator*>> m_FreeAllocators;
	std::mutex m_Mutex;

public:
	~CommandAllocatorPool();

public:
	void Create(GraphicsDevice* device, QueueType type);
	void ShutDown();

	ID3D12CommandAllocator* Allocate(u64 completedFence);
	void FreeAllocator(u64 fenceValue, ID3D12CommandAllocator* allocator);
	u32 Size() { return (u32)m_Allocators.size(); }
};