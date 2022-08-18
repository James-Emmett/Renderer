#pragma once
#include "System/Types.h"
#include "Graphics/Common/QueueType.h"
#include <d3d12.h>
#include <mutex>

class GraphicsDevice;
class CommandQueue
{
private:
	GraphicsDevice*		m_Device = nullptr;
	QueueType			m_Type;
	ID3D12CommandQueue* m_Queue = nullptr; // Queue object to execute command lists
	ID3D12Fence*		m_Fence = nullptr; // Each queue has a fence to sync finished execution
	u64					m_LastFence = 0;
	u64					m_NextFence = 0;
	HANDLE				m_FenceEvent = nullptr; // Even triggered when we wait CPU side for GPU to flag Finished

	// Mutex to lock fence and event when accessed by a thread
	std::mutex m_FenceMutex;
	std::mutex m_EventMutex;

public:
	CommandQueue();
	~CommandQueue();

public:
	void	Dispose();
	void	Create(GraphicsDevice* device, QueueType type);
	u64		PollCurrentFenceValue();
	bool	IsFenceCompleted(u64 fence);
	void	GPUWait(u64 value);
	void	GPUWaitForQueue(CommandQueue* queue);
	void	GPUWaitforQueueFence(CommandQueue* queue, u64 value);
	void	CPUWaitForFence(u64 value);
	u64		Signal();
	u64		LastCompletedFence()const { return m_LastFence; }
	u64		NextFenceValue()const { return m_NextFence; }
	// Executes a command list on this queue
	u64 ExecuteCommandLists(ID3D12CommandList* commandLists);
	// Execute multiple command lists
	u64 ExecuteCommandLists(ID3D12CommandList** commandLists, u32 count = 1);
	// Get Native Ptr's
	ID3D12Fence* GetFence() { return m_Fence; }
	ID3D12CommandQueue* GetCommandQueue()const { return m_Queue; }
};