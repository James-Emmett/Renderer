#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "System/StringUtil.h"
#include "System/Logger.h"
#include "Math/Mathf.h"
#include <assert.h>
#include <sstream>

void MemoryHandler::Initialize(GraphicsDevice* device)
{
	m_Device = device;
	m_RTVDescriptorHeap.Initialize(device, 128, DescHeapType::RenderTargetView);
	m_DSVDescriptorHeap.Initialize(device, 128, DescHeapType::DepthStencilView);
	m_SamplerDescriptorHeap.Initialize(device, 128, DescHeapType::Sampler);
	m_ResourceDescriptorHeap.Initialize(device, 1024, DescHeapType::Resource);

	m_ShaderVisibleResourceHeap.Initialize(device, DescHeapType::Resource, 4096, true);
	m_ShaderVisibleSamplerHeap.Initialize(device, DescHeapType::Sampler, 128, true);

	// Just make it Huge ?
	m_UploadBuffer.Initialise(m_Device, 12800000000000, true);

}

void MemoryHandler::Dispose()
{
	m_UploadBuffer.Dispose();
	Flush(); // Flush Release all resources
	m_RTVDescriptorHeap.ShutDown();
	m_DSVDescriptorHeap.ShutDown();
	m_SamplerDescriptorHeap.ShutDown();
	m_ResourceDescriptorHeap.ShutDown();

	m_ShaderVisibleResourceHeap.Dispose();
	m_ShaderVisibleSamplerHeap.Dispose();
}

void MemoryHandler::FreeMemory(u64 frame)
{
	//--Free GPU Resources--
	m_UploadBuffer.ReleaseCompletedFrames(frame);

	std::lock_guard<std::mutex> lock(m_MemLock);
	// Basically we deffer the delection by the same number of buffers we have,
	// so double buffer we wait 2 frames to fully ensure GPU has finished with data
	while (m_ResourceDestroyQueue.empty() == false)
	{
		if (m_ResourceDestroyQueue.front().second + BufferCount < frame)
		{
			ID3D12Resource* resource = m_ResourceDestroyQueue.front().first;
			m_ResourceDestroyQueue.pop();
			resource->Release();
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}

	while (m_PipelineDestroyQueue.empty() == false)
	{
		if (m_PipelineDestroyQueue.front().second + BufferCount < frame)
		{
			ID3D12PipelineState* pipeLine = m_PipelineDestroyQueue.front().first;
			m_PipelineDestroyQueue.pop();
			pipeLine->Release();
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}

	while (m_RootSignatureDestoryQueue.empty() == false)
	{
		if (m_RootSignatureDestoryQueue.front().second + BufferCount < frame)
		{
			ID3D12RootSignature* rootSig = m_RootSignatureDestoryQueue.front().first;
			m_RootSignatureDestoryQueue.pop();
			rootSig->Release();
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}

	while (m_RTVDescDestoryQueue.empty() == false)
	{
		if (m_RTVDescDestoryQueue.front().second + BufferCount < frame)
		{
			DescriptorHandle handle = m_RTVDescDestoryQueue.front().first;
			m_RTVDescDestoryQueue.pop();
			m_RTVDescriptorHeap.Release(handle);
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}

	while (m_DSVDescDestoryQueue.empty() == false)
	{
		if (m_DSVDescDestoryQueue.front().second + BufferCount < frame)
		{
			DescriptorHandle handle = m_DSVDescDestoryQueue.front().first;
			m_DSVDescDestoryQueue.pop();
			m_DSVDescriptorHeap.Release(handle);
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}

	while (m_SamplerDescDestoryQueue.empty() == false)
	{
		if (m_SamplerDescDestoryQueue.front().second + BufferCount < frame)
		{
			DescriptorHandle handle = m_SamplerDescDestoryQueue.front().first;
			m_SamplerDescDestoryQueue.pop();
			m_SamplerDescriptorHeap.Release(handle);
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}

	while (m_ResourceDescDestoryQueue.empty() == false)
	{
		if (m_ResourceDescDestoryQueue.front().second + BufferCount < frame)
		{
			DescriptorHandle handle = m_ResourceDescDestoryQueue.front().first;
			m_ResourceDescDestoryQueue.pop();
			m_ResourceDescriptorHeap.Release(handle);
		}
		else
		{
			break; // Wait till we are buffer count frames ahead
		}
	}
}

void MemoryHandler::Release(ID3D12Resource* resource)
{
	m_ResourceDestroyQueue.push(std::make_pair(resource, m_Device->FrameCount()));
}

void MemoryHandler::Release(ID3D12PipelineState* pipeLine)
{
	m_PipelineDestroyQueue.push(std::make_pair(pipeLine, m_Device->FrameCount()));
}

void MemoryHandler::Release(ID3D12RootSignature* rootSig)
{
	m_RootSignatureDestoryQueue.push(std::make_pair(rootSig, m_Device->FrameCount()));
}

void MemoryHandler::Release(DescriptorHandle handle, DescHeapType type)
{
	switch (type)
	{
	case DescHeapType::Resource:
		m_ResourceDescDestoryQueue.push(std::make_pair(handle, m_Device->FrameCount()));
		break;
	case DescHeapType::Sampler:
		m_SamplerDescDestoryQueue.push(std::make_pair(handle, m_Device->FrameCount()));
		break;
	case DescHeapType::RenderTargetView:
		m_RTVDescDestoryQueue.push(std::make_pair(handle, m_Device->FrameCount()));
		break;
	case DescHeapType::DepthStencilView:
		m_DSVDescDestoryQueue.push(std::make_pair(handle, m_Device->FrameCount()));
		break;
	}
}

void MemoryHandler::Flush()
{
	FreeMemory(~0);
}

GraphicsDevice::GraphicsDevice()
{

}

void GraphicsDevice::Initialize(GraphicsParameters& parameters)
{
	HRESULT hr = S_OK;
	m_Parameters = parameters;

	// Debug First to catch errors
#if defined(_DEBUG)
	ID3D12Debug* debug = nullptr;
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug)); // Ask for debug interface
	assert(SUCCEEDED(hr));
	hr = debug->QueryInterface(IID_PPV_ARGS(&m_Debug)); // Try get Debug1
	assert(SUCCEEDED(hr));

	// Enable debugging and validation
	m_Debug->EnableDebugLayer();
	m_Debug->SetEnableGPUBasedValidation(true);

	// Flag are factory for debug mode
	m_FactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

	if (debug)
	{
		debug->Release();
		debug = nullptr;
	}
#endif

	// Create factory
	CreateDXGIFactory2(m_FactoryFlags, IID_PPV_ARGS(&m_Factory));

	// Enumerate adapter till we have DX12 support
	IDXGIAdapter1* adapter;
	for (u32 index = 0; DXGI_ERROR_NOT_FOUND != m_Factory->EnumAdapters1(index, &adapter); ++index)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// No software rendereding
		if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
		{
			continue;
		}

		// Check for feature level 12, dont create just check
		hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			break;
		}

		// Release as not required
		adapter->Release();
	}

	assert(adapter != nullptr);

	// Actually create device;
	hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
	assert(SUCCEEDED(hr));
	adapter->Release();

	// Create a debug device
#if defined(_DEBUG)
	assert(SUCCEEDED(m_Device->QueryInterface(&m_DebugDevice)));
#endif

	// Create Default Queues
	m_DirectQueue.Create(this, QueueType::Direct);
	m_ComputeQueue.Create(this, QueueType::Compute);
	m_CopyQueue.Create(this, QueueType::Copy);

	//--Create Command List Manager--
	m_DirectCommandList.Create(this, QueueType::Direct);
	m_CopyCommandList.Create(this, QueueType::Copy);

	// Create the CPU Resident Descriptors
	m_MemoryHandler.Initialize(this);

	// Create Swap Chain
	CreateSwapChain();
}

void GraphicsDevice::Dispose()
{
	WaitForGPU();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		if (m_RenderTargets[i])
		{
			m_RenderTargets[i]->Release();
		}

		if (m_DepthTargets[i])
		{
			m_DepthTargets[i]->Release();
		}
	}

	m_MemoryHandler.Dispose();

	m_CopyCommandList.ShutDown();
	m_DirectCommandList.ShutDown();

	m_CopyQueue.Dispose();
	m_ComputeQueue.Dispose();
	m_DirectQueue.Dispose();

#ifdef _DEBUG
	m_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
#endif // _DEBUG

	if (m_DebugDevice) { m_DebugDevice->Release(); }
	if (m_Debug) { m_Debug->Release(); }
	if (m_SwapChain) { m_SwapChain->Release(); }
	if (m_Device) { m_Device->Release(); }
	if (m_Factory) { m_Factory->Release(); }
}

u64 GraphicsDevice::FrameCount() const
{
	return m_FrameCount;
}

u32 GraphicsDevice::BufferIndex() const
{
	return m_BufferIndex;
}

ID3D12Device* GraphicsDevice::NativeDevice()
{
	return m_Device;
}

MemoryHandler* GraphicsDevice::MemoryHandler()
{
	return &m_MemoryHandler;
}

void GraphicsDevice::WaitForGPU()
{
	m_DirectQueue.CPUWaitForFence(m_DirectQueue.NextFenceValue() - 1);
	m_CopyQueue.CPUWaitForFence(m_CopyQueue.NextFenceValue() - 1);
}

void GraphicsDevice::WaitForPreviousFrame()
{
	u32 index = BufferIndex();
	m_DirectQueue.CPUWaitForFence(m_FrameFenceValue[index]);
}

const GraphicsParameters& GraphicsDevice::Parameters() const
{
	return m_Parameters;
}

CommandList GraphicsDevice::BeginCommandList(QueueType type)
{
	switch (type)
	{
	case QueueType::Direct:
		return m_DirectCommandList.Allocate(m_DirectQueue.LastCompletedFence());
		break;
	case QueueType::Copy:
		return m_CopyCommandList.Allocate(m_CopyQueue.LastCompletedFence());
		break;
	case QueueType::Compute:
	default:
		assert(false && "Queue type Implemented Yet");
		break;
	}

	return 0;
}

u64 GraphicsDevice::SubmitCommandList(QueueType type, CommandList cmd)
{
	u64 fence = 0;

	switch (type)
	{
	case QueueType::Direct:
	{
		ID3D12GraphicsCommandList * commandList = m_DirectCommandList.GetCommandList(cmd);
		commandList->Close();
		fence = m_DirectQueue.ExecuteCommandLists(commandList);
		m_DirectCommandList.Release(fence, cmd);
	}
		break;
	case QueueType::Copy:
	{
		ID3D12GraphicsCommandList * commandList = m_CopyCommandList.GetCommandList(cmd);
		commandList->Close();
		fence = m_CopyQueue.ExecuteCommandLists(commandList);
		m_CopyCommandList.Release(fence, cmd);
	}
		break;
	case QueueType::Compute:
	default:
		break;
	}

	return 0;
}

void GraphicsDevice::Transition(GraphicsResource* resource, u32 subresource, ResourceState before, ResourceState after, CommandList cmd)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; // I dont think i need to split?
	barrier.Transition.pResource = resource->NativeResource();
	barrier.Transition.StateBefore = ConversionTypes::ConvertResourceState(before);
	barrier.Transition.StateAfter = ConversionTypes::ConvertResourceState(after);
	barrier.Transition.Subresource = subresource; // Hopefully this doesnt error 

	ID3D12GraphicsCommandList* commandList = m_DirectCommandList.GetCommandList(cmd);
	commandList->ResourceBarrier(1, &barrier);
}

void GraphicsDevice::TransitionBackBuffer(ResourceState before, ResourceState after, CommandList cmd)
{
	Transition(m_RenderTargets[m_BufferIndex],0, before, after, cmd);
}

void GraphicsDevice::SetRootConstant32Bit(u32 rootIndex, u32 srcData, u32 offset, CommandList cmd)
{
	GetGraphicsCommandList(cmd)->SetGraphicsRoot32BitConstant(rootIndex, srcData, offset);
}

void GraphicsDevice::SetRootConstant(std::shared_ptr<ConstantResource> buffer, u32 rootIndex, CommandList cmd)
{
	GetGraphicsCommandList(cmd)->SetGraphicsRootConstantBufferView(rootIndex, buffer->NativeResource()->GetGPUVirtualAddress());
}

void GraphicsDevice::SetGraphicsRootDescriptorTable(u32 rootIndex, const DescriptorHandle& handle, CommandList cmd)
{
	GetGraphicsCommandList(cmd)->SetGraphicsRootDescriptorTable(rootIndex, HandleAsGPU(handle));
}

void GraphicsDevice::SetDescriptors(DescriptorHeap* resourceHeap, DescriptorHeap* sampler, CommandList cmd)
{
	u32 count = 0;
	ID3D12DescriptorHeap* heaps[2];
	if (resourceHeap)
	{
		heaps[count++] = resourceHeap->NativeHeapHandle();
	}

	if (sampler)
	{
		heaps[count++] = sampler->NativeHeapHandle();
	}

	GetGraphicsCommandList(cmd)->SetDescriptorHeaps(count, heaps);
}

void GraphicsDevice::CopyBuffer(GraphicsResource* dest, u64 destOffset, GraphicsResource* src, u64 offset, u64 size, CommandList cmd)
{
	ID3D12GraphicsCommandList* commandList = m_DirectCommandList.GetCommandList(cmd);
	commandList->CopyBufferRegion(dest->NativeResource(), destOffset, src->NativeResource(), offset, size);
}

DescriptorHandle GraphicsDevice::CopyDescriptors(DescriptorHeap& destAllocator, const DescriptorHandle& srcHandle, u32 count)
{
	DescriptorHandle destHandle = destAllocator.Allocate();
	m_Device->CopyDescriptorsSimple(count, HandleAsCPU(destHandle), HandleAsCPU(srcHandle), destAllocator.GetHeapType());

	return destHandle;
}

void GraphicsDevice::BindVertexBuffer(std::shared_ptr<GraphicsResource>* vertexBuffer, u32 slot, u32* offsets, u32 count, CommandList cmd)
{
	D3D12_VERTEX_BUFFER_VIEW vbv[8] = {};

	for (size_t i = 0; i < count; ++i)
	{
		if (vertexBuffer[i] != nullptr)
		{
			vbv[i].BufferLocation = vertexBuffer[i]->NativeResource()->GetGPUVirtualAddress();
			vbv[i].SizeInBytes = (UINT)vertexBuffer[i]->ByteSize();

			if (offsets != nullptr)
			{
				vbv[i].BufferLocation += offsets[i];
				vbv[i].SizeInBytes -= offsets[i];
			}

			vbv->StrideInBytes = vertexBuffer[i]->Stride();
		}
	}

	GetGraphicsCommandList(cmd)->IASetVertexBuffers(slot, count, vbv);
}

void GraphicsDevice::BindIndexBuffer(std::shared_ptr<GraphicsResource> indexBuffer, u32 offsets, CommandList cmd)
{
	D3D12_INDEX_BUFFER_VIEW ibv = {};
	ibv.BufferLocation	= indexBuffer->NativeResource()->GetGPUVirtualAddress() + offsets;
	ibv.SizeInBytes		= (UINT)indexBuffer->ByteSize();
	ibv.Format = indexBuffer->Stride() == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	GetGraphicsCommandList(cmd)->IASetIndexBuffer(&ibv);
}

void GraphicsDevice::BindPipeline(std::shared_ptr<PipelineState> pipeLine, CommandList cmd)
{
	ID3D12GraphicsCommandList* commandList = GetGraphicsCommandList(cmd);
	commandList->SetPipelineState(pipeLine->NativePipeline());
	commandList->SetGraphicsRootSignature(pipeLine->NativeRootSignature());
	commandList->IASetPrimitiveTopology(ConversionTypes::ConvertTopology(pipeLine->Topology()));
}

void GraphicsDevice::BindRenderTarget(std::shared_ptr<TextureResource> render, TextureResource* depth, CommandList cmd)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtv, dsv;
	rtv.ptr = SIZE_T(render ? render->RenderTargetView().m_Cpu : m_RenderTargets[m_BufferIndex]->RenderTargetView().m_Cpu);
	dsv.ptr = SIZE_T(depth ? depth->DepthStencilView().m_Cpu : m_DepthTargets[m_BufferIndex]->DepthStencilView().m_Cpu);

	GetGraphicsCommandList(cmd)->OMSetRenderTargets(1, &rtv, false, &dsv);
}

void GraphicsDevice::BindScissorRects(const Rect<u32>* pRects, u32 count, CommandList cmd)
{
	D3D12_RECT d3dRect[8]; // Think max is 8?

	for (u32 i = 0; i < count; ++i)
	{
		// My Rectangle is Y Up, D3d!2 is Y down so reversed bottom and top!
		d3dRect[i].left = (long)pRects[i].Left();
		d3dRect[i].top = (long)pRects[i].Bottom();
		d3dRect[i].right = (long)pRects[i].Right();
		d3dRect[i].bottom = (long)pRects[i].Top();
	}

	// Set the scissors on the selected commandlist
	GetGraphicsCommandList(cmd)->RSSetScissorRects(count, d3dRect);
}

void GraphicsDevice::BindViewports(const ViewPort* pViewports, u32 count, CommandList cmd)
{
	D3D12_VIEWPORT viewPorts[6]; // Think max is 6?

	for (u32 i = 0; i < count; ++i)
	{
		viewPorts[i].TopLeftX = (FLOAT)pViewports[i].X;
		viewPorts[i].TopLeftY = (FLOAT)pViewports[i].Y;
		viewPorts[i].Width = (FLOAT)pViewports[i].Width;
		viewPorts[i].Height = (FLOAT)pViewports[i].Height;
		viewPorts[i].MinDepth = (FLOAT)pViewports[i].MinDepth;
		viewPorts[i].MaxDepth = (FLOAT)pViewports[i].MaxDepth;
	}

	// Set the Viewports on the selected commandlist
	GetGraphicsCommandList(cmd)->RSSetViewports(count, viewPorts);
}

void GraphicsDevice::BindDefaultViewAndScissor(CommandList cmd)
{
	// Bind Default view and scissor for frame
	GetGraphicsCommandList(cmd)->RSSetViewports(1, &m_DefaultView);
	GetGraphicsCommandList(cmd)->RSSetScissorRects(1, &m_DefaultScissor);
}


void GraphicsDevice::ClearRenderTarget(std::shared_ptr<TextureResource> target, const Color& color, CommandList cmd)
{
	const float clearColor[] = { color.r, color.g, color.b, color.a };
	D3D12_CPU_DESCRIPTOR_HANDLE rtv;
	rtv.ptr = (SIZE_T)(target ? target->RenderTargetView().m_Cpu : m_RenderTargets[m_BufferIndex]->RenderTargetView().m_Cpu);
	GetGraphicsCommandList(cmd)->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
}

void GraphicsDevice::ClearDepthTarget(std::shared_ptr<TextureResource> target, float depth, u32 stencil, CommandList cmd)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsv;
	dsv.ptr = (SIZE_T)(target ? target->DepthStencilView().m_Cpu : m_DepthTargets[m_BufferIndex]->DepthStencilView().m_Cpu);
	GetGraphicsCommandList(cmd)->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, depth, stencil, 0, nullptr);
}

//--Draw Methods--
void GraphicsDevice::DrawInstanced(u32 vertexCount, u32 instanceCount, u32 startVertex, u32 startInstance, CommandList cmd)
{
	GetGraphicsCommandList(cmd)->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
}

void GraphicsDevice::DrawIndexedInstanced(u32 indexCount, u32 instanceCount, u32 indexStart, u32 vertexStart, u32 instanceLocation, CommandList cmd)
{
	GetGraphicsCommandList(cmd)->DrawIndexedInstanced(indexCount, instanceCount, indexStart, vertexStart, instanceLocation);
}

void GraphicsDevice::Present()
{
	// Present the result to the screen
	assert(SUCCEEDED(m_SwapChain->Present(m_Parameters.m_VsyncInterval, 0)));

	// Signal the queue for this frame
	m_FrameFenceValue[BufferIndex()] = m_DirectQueue.Signal();

	// Tell ring buffer to mark end of this frames uploads!
	m_MemoryHandler.m_UploadBuffer.FinishFrame(m_FrameCount);

	//--Start Next Frame--
	++m_FrameCount;

	// Cache this
	m_BufferIndex = m_FrameCount % BufferCount;

	// Wait for [Frame - buffercount] to be finished
	WaitForPreviousFrame();

	// Try free up some memory
	m_MemoryHandler.FreeMemory(m_DirectQueue.LastCompletedFence());
}


DescriptorHandle GraphicsDevice::CreateShaderResourceView(ResourceDesc& desc, ID3D12Resource* resource)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = GetResourceFormat(desc.Format, false);
	srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (desc.Dimension == ResourceDimension::Texture1D)
	{
		// Array of 1D textures
		if (desc.DepthOrArraySize > 1)
		{
			srv.ViewDimension					= D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			srv.Texture1DArray.FirstArraySlice	= 0;
			srv.Texture1DArray.MostDetailedMip	= 0;
			srv.Texture1DArray.ArraySize		= desc.DepthOrArraySize;
			srv.Texture1DArray.MipLevels		= desc.MipCount;
		}
		else
		{
			srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			srv.Texture1D.MostDetailedMip = 0;
			srv.Texture1D.MipLevels = desc.MipCount;
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture2D || desc.Dimension == ResourceDimension::TextureCube)
	{
		// Array of 2D textures
		if (desc.DepthOrArraySize > 1)
		{
			// It is a cube texture
			if (desc.Dimension == ResourceDimension::TextureCube)
			{
				// An array of cubes
				if (desc.DepthOrArraySize > 6)
				{
					srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
					srv.TextureCubeArray.First2DArrayFace = 1;
					srv.TextureCubeArray.NumCubes = desc.DepthOrArraySize / 6;
					srv.TextureCubeArray.MostDetailedMip = 1;
					srv.TextureCubeArray.MipLevels = desc.MipCount;
				}
				else // Just a standard cube
				{
					srv.ViewDimension				= D3D12_SRV_DIMENSION_TEXTURECUBE;
					srv.TextureCube.MostDetailedMip	= 1;
					srv.TextureCube.MipLevels		= 1;
				}
			}
			else // Texture2D Array
			{
				// Multi Sampled Texture
				if (desc.MultiSample.Count > 1)
				{
					srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
					srv.Texture2DMSArray.FirstArraySlice = 1;
					srv.Texture2DMSArray.ArraySize = desc.DepthOrArraySize;
				}
				else
				{
					srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
					srv.Texture2DArray.FirstArraySlice = 1;
					srv.Texture2DArray.ArraySize = desc.DepthOrArraySize;
					srv.Texture2DArray.MostDetailedMip = 1;
					srv.Texture2DArray.MipLevels = desc.MipCount;
				}
			}
		}
		else // Standard Texture 2D
		{
			if (desc.MultiSample.Count > 1)
			{
				srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srv.Texture2D.MostDetailedMip = 0;
				srv.Texture2D.MipLevels = desc.MipCount;
			}
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture3D)
	{
		srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		srv.Texture3D.MostDetailedMip = 1;
		srv.Texture3D.MipLevels = desc.MipCount;
	}

	// Finally create the Shader Resource View
	DescriptorHandle handle = m_MemoryHandler.m_ResourceDescriptorHeap.Allocate();
	m_Device->CreateShaderResourceView(resource, &srv, HandleAsCPU(handle));
	return handle;
}

DescriptorHandle GraphicsDevice::CreateRenderTargetView(ResourceDesc& desc, ID3D12Resource* resource)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtv = {};
	rtv.Format = GetResourceFormat(desc.Format, false);

	if (desc.Dimension == ResourceDimension::Texture1D)
	{
		// Arrray of 1D Textures
		if (desc.DepthOrArraySize > 1)
		{
			rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
			rtv.Texture1DArray.FirstArraySlice = 0;
			rtv.Texture1DArray.MipSlice = 0;
			rtv.Texture1DArray.ArraySize = desc.DepthOrArraySize;
		}
		else // Standard 1D Textures
		{
			rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
			rtv.Texture1D.MipSlice = 0;
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture2D)
	{
		// Array Of 2D Textures
		if (desc.DepthOrArraySize > 1)
		{
			// Multi Sampled
			if (desc.MultiSample.Count > 1)
			{
				rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
				rtv.Texture2DMSArray.FirstArraySlice = 0;
				rtv.Texture2DMSArray.ArraySize = desc.DepthOrArraySize;
			}
			else // Standard 2D Texture Array
			{
				rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				rtv.Texture2DArray.FirstArraySlice = 0;
				rtv.Texture2DArray.ArraySize = desc.DepthOrArraySize;
				rtv.Texture2DArray.MipSlice = 0;
			}
		}
		else
		{
			// Multi Sampled
			if (desc.MultiSample.Count > 1)
			{
				rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
			}
			else // Standard 2D Texture
			{
				rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				rtv.Texture2D.MipSlice = 0;
			}
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture3D)
	{
		rtv.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
		rtv.Texture3D.MipSlice = 0;
		rtv.Texture3D.FirstWSlice = 0;
		rtv.Texture3D.WSize = -1; // Why -1 i forgot!!!?
	}

	DescriptorHandle handle = m_MemoryHandler.m_RTVDescriptorHeap.Allocate();
	m_Device->CreateRenderTargetView(resource, &rtv, HandleAsCPU(handle));

	return handle;
}

DescriptorHandle GraphicsDevice::CreateDepthStencilView(ResourceDesc& desc, ID3D12Resource* resource)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
	dsv.Format = GetResourceFormat(desc.Format, true);

	if (desc.Dimension == ResourceDimension::Texture1D)
	{
		if (desc.DepthOrArraySize > 1)
		{
			dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
			dsv.Texture1DArray.FirstArraySlice = 0;
			dsv.Texture1DArray.MipSlice = 0;
			dsv.Texture1DArray.ArraySize = desc.DepthOrArraySize;
		}
		else
		{
			dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
			dsv.Texture1D.MipSlice = 0;
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture2D)
	{
		if (desc.DepthOrArraySize > 1)
		{
			if (desc.MultiSample.Count > 1)
			{
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				dsv.Texture2DMSArray.FirstArraySlice = 0;
				dsv.Texture2DMSArray.ArraySize = desc.DepthOrArraySize;
			}
			else
			{
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				dsv.Texture2DArray.FirstArraySlice = 0;
				dsv.Texture2DArray.MipSlice = 0;
				dsv.Texture2DArray.ArraySize = desc.DepthOrArraySize;
			}
		}
		else
		{
			if (desc.MultiSample.Count > 1)
			{
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsv.Texture2D.MipSlice = 0;
			}
		}
	}

	DescriptorHandle handle = m_MemoryHandler.m_DSVDescriptorHeap.Allocate();
	m_Device->CreateDepthStencilView(resource, &dsv, HandleAsCPU(handle));
	return handle;
}

DescriptorHandle GraphicsDevice::CreateUnorderedAccessView(ResourceDesc& desc, ID3D12Resource* resource)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC uav = {};
	uav.Format = GetResourceFormat(desc.Format, false);


	if (desc.Dimension == ResourceDimension::Texture1D)
	{
		if (desc.DepthOrArraySize > 1)
		{
			uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
			uav.Texture1DArray.FirstArraySlice = 0;
			uav.Texture1DArray.ArraySize = desc.DepthOrArraySize;
			uav.Texture1DArray.MipSlice = 0;
		}
		else
		{
			uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
			uav.Texture1D.MipSlice = 0;
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture2D)
	{
		if (desc.DepthOrArraySize > 1)
		{
			uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			uav.Texture2DArray.FirstArraySlice = 0;
			uav.Texture2DArray.ArraySize = desc.DepthOrArraySize;
			uav.Texture2DArray.MipSlice = 0;
		}
		else
		{
			uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			uav.Texture2D.MipSlice = 0;
		}
	}
	else if (desc.Dimension == ResourceDimension::Texture3D)
	{
		uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
		uav.Texture3D.MipSlice = 0;
		uav.Texture3D.FirstWSlice = 0;
		uav.Texture3D.WSize = -1;
	}

	DescriptorHandle handle = m_MemoryHandler.m_ResourceDescriptorHeap.Allocate();
	m_Device->CreateUnorderedAccessView(resource, nullptr, &uav, HandleAsCPU(handle));
	return handle;
}

DescriptorHandle GraphicsDevice::CreateSamperState(SamplerDesc& desc)
{
	DescriptorHandle handle = m_MemoryHandler.m_SamplerDescriptorHeap.Allocate();

	D3D12_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter			= ConversionTypes::ConvertFilter(desc.m_Filter);
	samplerDesc.AddressU		= ConversionTypes::ConvertAddressMode(desc.m_AddressU);
	samplerDesc.AddressV		= ConversionTypes::ConvertAddressMode(desc.m_AddressV);
	samplerDesc.AddressW		= ConversionTypes::ConvertAddressMode(desc.m_AddressW);
	samplerDesc.MipLODBias		= desc.m_MipLODBias = 0;
	samplerDesc.MaxAnisotropy	= desc.m_MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc	= ConversionTypes::ConvertComparison(desc.m_Comparison);
	samplerDesc.BorderColor[0]	= desc.m_Color.r;
	samplerDesc.BorderColor[1]	= desc.m_Color.g;
	samplerDesc.BorderColor[2]	= desc.m_Color.b;
	samplerDesc.BorderColor[3]	= desc.m_Color.a;
	samplerDesc.MinLOD			= desc.m_MinLOD = MinLod;
	samplerDesc.MaxLOD			= desc.m_MaxLOD = MaxLod;

	m_Device->CreateSampler(&samplerDesc, HandleAsCPU(handle));
	return handle;
}

void GraphicsDevice::CreateSwapChain()
{
	// Set default viewport
	m_DefaultView.TopLeftX	= 0.0f;
	m_DefaultView.TopLeftY	= 0.0f;
	m_DefaultView.Width		= (FLOAT)m_Parameters.m_Width;
	m_DefaultView.Height	= (FLOAT)m_Parameters.m_Height;

	// Investigate inversing the depth for percision reasons?
	m_DefaultView.MinDepth = 0.0f;
	m_DefaultView.MaxDepth = 1.0f;

	// Set Default Scissor region
	m_DefaultScissor.left	= 0;
	m_DefaultScissor.top	= 0;
	m_DefaultScissor.right	= m_Parameters.m_Width;
	m_DefaultScissor.bottom = m_Parameters.m_Height;

	if (m_SwapChain != nullptr)
	{
		WaitForGPU();
		// Force Release the targets avoidng the memory manager
		for (size_t i = 0; i < BufferCount; ++i)
		{
			delete m_RenderTargets[i];
			delete m_DepthTargets[i];
		}

		m_MemoryHandler.Flush(); // Flush everything
		m_SwapChain->ResizeBuffers(BufferCount, m_Parameters.m_Width, m_Parameters.m_Height, ConversionTypes::ConvertFormat((SurfaceFormat)m_Parameters.m_RenderFormat), 0);
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC sc = {};
		sc.BufferCount							= BufferCount;
		sc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc.BufferDesc.Width						= m_Parameters.m_Width;
		sc.BufferDesc.Height					= m_Parameters.m_Height;
		sc.BufferDesc.Format					= ConversionTypes::ConvertFormat((SurfaceFormat)m_Parameters.m_RenderFormat); // This untill HDR support?
		sc.BufferDesc.RefreshRate.Numerator		= 0;
		sc.BufferDesc.RefreshRate.Denominator	= 1;
		sc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD; // Discard as we dont care about whats on previous frame?
		sc.SampleDesc.Count						= 1;
		sc.SampleDesc.Quality					= 0;
		sc.OutputWindow							= (HWND)m_Parameters.m_Handle;
		sc.Windowed								= true;

		HRESULT hr = m_Factory->CreateSwapChain(m_DirectQueue.GetCommandQueue(), &sc, &m_SwapChain);
		assert(SUCCEEDED(hr) && "Failed to create swap chain");
	}

	for (size_t i = 0; i < BufferCount; ++i)
	{
		ID3D12Resource* resource = nullptr;
		DescriptorHandle handle =  m_MemoryHandler.m_RTVDescriptorHeap.Allocate();
		assert(SUCCEEDED(m_SwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&resource))));
		m_Device->CreateRenderTargetView(resource, nullptr, HandleAsCPU(handle));

		// Set the Texture Resource data
		m_RenderTargets[i] = new TextureResource(this, resource);
		m_RenderTargets[i]->RenderTargetView() = handle;

		// Create Depth Textures
		m_DepthTargets[i] = new TextureResource(this, ResourceDesc::Tex2D(HeapType::Default, (SurfaceFormat)m_Parameters.m_DepthFormat, m_Parameters.m_Width, m_Parameters.m_Height, 1, 1, TextureLayout::Unkown, (u32)BindFlag::DepthStencil));
	}
}

DXGI_FORMAT GraphicsDevice::GetResourceFormat(SurfaceFormat format, bool isDepth)
{
	if (isDepth == false)
	{
		switch (format)
		{
		case SurfaceFormat::R16_Typeless:
			return DXGI_FORMAT_R16_UNORM;
			break;

		case SurfaceFormat::R32_Typeless:
			return DXGI_FORMAT_R32_FLOAT;
			break;

		case SurfaceFormat::R24G8_Typeless:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;

		case SurfaceFormat::R32G8X24_Typeless:
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;

		case SurfaceFormat::R8_Typeless:
			return DXGI_FORMAT_R8_UNORM;
			break;

		default:
			return ConversionTypes::ConvertFormat(format);
			break;
		}
	}
	else
	{
		switch (format)
		{
		case SurfaceFormat::R16_Typeless:
			return DXGI_FORMAT_D16_UNORM;
			break;

		case SurfaceFormat::R32_Typeless:
			return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
			break;

		case SurfaceFormat::R24G8_Typeless:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;

		case SurfaceFormat::R32G8X24_Typeless:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			break;

		default:
			// Give up at this point...
			return ConversionTypes::ConvertFormat(format);
			break;
		}
	}
}

void GraphicsDevice::CompilerShader(std::string fileName, const char* entry, const char* shadermodel, Byte** byteCode, u32& size)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;
	ID3DBlob* blob = nullptr;
	u32 flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif // _DEBUG

	hr = D3DCompileFromFile(StringUtil::Widen(fileName).c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, shadermodel, 0, 0, &blob, &error);
	if (FAILED(hr))
	{
		std::stringstream ss("");

		if (error != nullptr)
		{
			ss << (char*)error->GetBufferPointer();
			error->Release();
		}
		else
		{
			ss << "Unkown Shader Error.";
		}

		LogError(ss.str());
		assert(false);
	}

	if (error)
	{
		error->Release();
	}

	size = (u32)blob->GetBufferSize();
	*byteCode = new Byte[size];
	memcpy(*byteCode, blob->GetBufferPointer(), size);
	blob->Release();
}

ID3D12GraphicsCommandList* GraphicsDevice::GetGraphicsCommandList(CommandList cmd)
{
	return m_DirectCommandList.GetCommandList(cmd);
}

D3D12_CPU_DESCRIPTOR_HANDLE GraphicsDevice::HandleAsCPU(const DescriptorHandle& handle) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3dHandle = {(SIZE_T)(handle.m_Cpu)};
	return d3dHandle;
}

D3D12_GPU_DESCRIPTOR_HANDLE GraphicsDevice::HandleAsGPU(const DescriptorHandle& handle) const
{
	D3D12_GPU_DESCRIPTOR_HANDLE d3dHandle = { (SIZE_T)(handle.m_Gpu) };
	return d3dHandle;
}
