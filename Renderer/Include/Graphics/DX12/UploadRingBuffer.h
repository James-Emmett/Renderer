#pragma once
#include "System/Types.h"
#include "GraphicsResource.h"
#include <d3d12.h>
#include <queue>
#include <memory>

// Is there a better way than this?
class UploadRingBuffer
{
public:
	struct FrameAttribute
	{
		u64 m_FenceValue;
		u64 m_StartIndex;
		u64 m_Size;

		FrameAttribute(u64 fenceValue, u64 start, u64 size) : m_FenceValue(fenceValue), m_StartIndex(start), m_Size(size)
		{}
	};

protected:
	std::queue<FrameAttribute> m_FrameTailQueue;
	u64 m_Head = 0;
	u64 m_Tail = 0;
	u64 m_MaxSize = 0;
	u64 m_Used = 0;
	u64 m_CurrentFramesize = 0;

	GraphicsDevice* m_Device = nullptr;
	ID3D12Resource* m_Resource = nullptr;
	void* m_CpuVirtualAddress = nullptr;
	D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress;

public:
	UploadRingBuffer();

public:
	void Initialise(GraphicsDevice* device, u32 maxSize, bool cpuAccess);
	void FinishFrame(u64 fence);
	std::shared_ptr<GraphicsResource> Allocate(u64 byteSize, u64 alignment = 1);
	std::shared_ptr<GraphicsResource> AllocateTextureMemory(u64 byteSize);
	void ReleaseCompletedFrames(u64 compeletedFenceValue);
	bool IsFull()const { return m_Used >= m_MaxSize; }
	void Dispose();
};