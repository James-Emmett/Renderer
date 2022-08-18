#pragma once
#include "System/Types.h"

enum class DescHeapType
{
	Resource,
	Sampler,
	RenderTargetView,
	DepthStencilView,
	Unkown
};

struct DescriptorHandle
{
	DescHeapType m_Type;
	u64 m_Cpu = 0;
	u64 m_Gpu = 0;

	DescriptorHandle(u64 cpuHandle = 0, u64 gpuHandle = 0, DescHeapType type = DescHeapType::Unkown)
	{
		m_Type = type;
		m_Cpu = cpuHandle;
		m_Gpu = gpuHandle;
	}

	bool IsValid()const
	{
		return m_Cpu != 0;
	}

	bool IsShaderVisible()const
	{
		return m_Gpu != 0;
	}

	void operator=(const DescriptorHandle& handle)
	{
		m_Type	= handle.m_Type;
		m_Cpu	= handle.m_Cpu;
		m_Gpu	= handle.m_Gpu;
	}
};