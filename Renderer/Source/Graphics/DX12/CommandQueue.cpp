#include "Graphics/DX12/CommandQueue.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "Math/Mathf.h"

CommandQueue::CommandQueue()
{
	m_NextFence = 1;
	m_LastFence = 0;
}

CommandQueue::~CommandQueue()
{
	Dispose();
}

void CommandQueue::Dispose()
{
	CloseHandle(m_FenceEvent);
	if (m_Fence)
	{
		m_Fence->Release();
		m_Fence = nullptr;
	}

	if (m_Queue)
	{
		m_Queue->Release();
		m_Queue = nullptr;
	}
}

void CommandQueue::Create(GraphicsDevice* device, QueueType type)
{
	m_Device = device;
	
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type		=  ConversionTypes::ConvertCommandListType(m_Type);
	desc.NodeMask	= 0; // For multi-GPU 0 ignores

	// Create queue
	m_Device->NativeDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_Queue));

	// Create Fence, every queue has a fence
	m_Device->NativeDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));

	// Fence event for CPU waiting.
	m_FenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
}

u64 CommandQueue::PollCurrentFenceValue()
{
	m_LastFence = Mathf::Max(m_LastFence, m_Fence->GetCompletedValue());
	return m_LastFence;
}

bool CommandQueue::IsFenceCompleted(u64 fence)
{
	if (fence > m_LastFence) { PollCurrentFenceValue(); }
	return fence <= m_LastFence;
}

void CommandQueue::GPUWait(u64 value)
{
	m_Queue->Wait(m_Fence, value);
}

void CommandQueue::GPUWaitForQueue(CommandQueue* queue)
{
	m_Queue->Wait(queue->m_Fence, queue->NextFenceValue() - 1);
}

void CommandQueue::GPUWaitforQueueFence(CommandQueue* queue, u64 value)
{
	m_Queue->Wait(queue->GetFence(), value);
}

void CommandQueue::CPUWaitForFence(u64 value)
{
	if (IsFenceCompleted(value)) { return; }

	// Lock the function and spin CPU untill GPU signals event
	std::lock_guard<std::mutex> lock(m_EventMutex);
	m_Fence->SetEventOnCompletion(value, m_FenceEvent);
	WaitForSingleObjectEx(m_FenceEvent, INFINITE, false);
	m_LastFence = value;
}

u64 CommandQueue::Signal()
{
	// Prevent other Threads from messing with fence till we are done signaling
	std::lock_guard<std::mutex> lock(m_FenceMutex);
	m_Queue->Signal(m_Fence, m_NextFence);
	return m_NextFence++;
}

u64 CommandQueue::ExecuteCommandLists(ID3D12CommandList* commandLists)
{
	m_Queue->ExecuteCommandLists(1, &commandLists);
	return Signal();
}

u64 CommandQueue::ExecuteCommandLists(ID3D12CommandList** commandLists, u32 count)
{
	m_Queue->ExecuteCommandLists(count, commandLists);
	return Signal();
}