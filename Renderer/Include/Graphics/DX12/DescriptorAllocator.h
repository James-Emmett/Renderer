#pragma once
#include "Graphics/Common/DescriptorHandle.h"
#include <d3d12.h>
#include <mutex>
#include <vector>

class GraphicsDevice;
// Create Descriptor heap can be shader visible
class DescriptorHeap
{
private:
	DescHeapType				m_HeapType;
	D3D12_DESCRIPTOR_HEAP_TYPE  m_D3DHeapType;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CpuStart = { 0 };
	D3D12_GPU_DESCRIPTOR_HANDLE m_GpuStart = { 0 };		// Only CBV, SRV, UAV and Sampler are GPU Visible
	ID3D12DescriptorHeap*		m_Heap = nullptr;		// Heap for descriptors
	u32							m_Count = 0;			// How many currently used
	u32							m_Size = 0;				// How many descriptors we support
	u32							m_DescriptorSize = 0;	// Size of a single descriptor
	bool						m_ShaderVisible = false;

	// Multiple threads may try allocate descriptors so we need to lock
	std::mutex m_Lock;

public:
	DescriptorHeap();
	DescriptorHeap(DescriptorHeap&& other)noexcept;
	~DescriptorHeap() { Dispose(); }

public:
	void Initialize(GraphicsDevice* device, DescHeapType type, u32 count, bool shaderVisible = false);
	DescriptorHandle Allocate();
	DescriptorHandle GetHandle(u32 index = 0)const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUStart()const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUStart()const;
	bool IsNull()const;
	bool IsShaderVisible()const;
	u32  DesciptorSize()const;
	void Reset(); // USed for shader visible Descritpros to reset whole heap
	void Dispose();

	ID3D12DescriptorHeap* NativeHeapHandle()const;
	D3D12_DESCRIPTOR_HEAP_TYPE GetHeapType()const;
};

// CPU ONLY: Allocates heaps for cpu resident resourceses in a free list fashion, dynamically grows if out of space.!.
class DescriptorAllocator
{
private:
	GraphicsDevice*								m_Device = nullptr;
	std::vector<DescriptorHeap>					m_Heaps;
	std::vector<DescriptorHandle>				m_FreeList;	// Handles we can use
	DescHeapType								m_Type = DescHeapType::Resource;
	u32											m_CountPerHeap = 0;
	u32											m_DescriptorSize = 0;
	bool										m_ShaderVisible;

	std::mutex m_Lock;

public:
	void Initialize(GraphicsDevice* device, u32 countPerHeap, DescHeapType type, bool shaderVisible = false);
	DescriptorHandle Allocate();
	void Release(DescriptorHandle index);
	// Releases everything, call on shutdown only!!!!
	void ShutDown();

private:
	void AllocateHeap();
};