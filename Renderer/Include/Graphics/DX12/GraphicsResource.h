#pragma once
#include "../Common/ResourceDesc.h"
#include <d3d12.h>
#include <memory>

class GraphicsDevice;
class GraphicsResource
{
protected:
	GraphicsDevice*		m_Device = nullptr;
	ID3D12Resource*		m_Resource = nullptr;
	ResourceDimension	m_ResourceDimension = ResourceDimension::Buffer;
	ResourceState		m_ResourceState = ResourceState::Common;
	HeapType			m_HeapType = HeapType::Default;
	u64					m_Size	 = 0;
	u32					m_Stride = 0;
	u64					m_Offset = 0;
	u32					m_Count  = 0;
	bool				m_IsSuballocation = false;

	ResourceDesc		m_Desc;
	D3D12_RESOURCE_DESC m_InternalDesc;
	
public:
	GraphicsResource(GraphicsDevice* device, ResourceDesc description);
	GraphicsResource(GraphicsDevice* device, ID3D12Resource* resource, HeapType type = HeapType::Upload, u64 offset = 0, u64 size = 0, bool m_IsSuballocation = false);
	~GraphicsResource() { Release(); }

public:
	GraphicsDevice*	GetGraphicsDevice()const { return m_Device; }
	ResourceState   GetResourceState()const { return m_ResourceState; }
	HeapType		Heap()const { return m_HeapType; }
	u64				ByteSize()const { return m_Size; }
	u32				Stride()const { return m_Stride; }
	u32				Count()const { return m_Count; }
	u32				Width()const { return m_Desc.Width; }
	Byte*			Map(int subresource = 0);
	void			Unmap(int subresource = 0);
	// CommandList only used if Default Resource heap
	void			SetData(Byte* data, u64 size, u64 offset, CommandList cmd);
	// Overide this to manage when /how certain resources get released
	virtual void	Release();

	ResourceDesc&	GetResourceDesc();
	ID3D12Resource* NativeResource() { return m_Resource; }

	static std::shared_ptr<GraphicsResource> CreateBuffer(GraphicsDevice* device, HeapType heapType, u32 size, u32 stride, u32 flags);
};