#pragma once
#include <queue>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h> // Remove Debug Only
#include "Math/Rect.h"
#include "ConversionTypes.h"
#include "DescriptorAllocator.h"
#include "CommandQueue.h"
#include "CommandListManager.h"
#include "TextureResource.h"
#include "PipelineState.h"
#include "ConstantResource.h"
#include "UploadRingBuffer.h"

// Link libs
#pragma comment(lib, "D3DCompiler.lib") // Remove Debug Only
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class MemoryHandler
{
private:
	GraphicsDevice* m_Device = nullptr;
	std::queue<std::pair<ID3D12Resource*, u64>>		 m_ResourceDestroyQueue;
	std::queue<std::pair<ID3D12PipelineState*, u64>> m_PipelineDestroyQueue;
	std::queue<std::pair<ID3D12RootSignature*, u64>> m_RootSignatureDestoryQueue;

	std::queue<std::pair<DescriptorHandle, u64>>	 m_RTVDescDestoryQueue;
	std::queue<std::pair<DescriptorHandle, u64>>	 m_DSVDescDestoryQueue;
	std::queue<std::pair<DescriptorHandle, u64>>	 m_SamplerDescDestoryQueue;
	std::queue<std::pair<DescriptorHandle, u64>>	 m_ResourceDescDestoryQueue;

	std::mutex m_MemLock;

public:
	// Manages CPU Resident Descriptors and deffer destorys stuff
	DescriptorAllocator m_RTVDescriptorHeap;
	DescriptorAllocator m_DSVDescriptorHeap;
	DescriptorAllocator m_SamplerDescriptorHeap;
	DescriptorAllocator m_ResourceDescriptorHeap; // CBV_SRV_UAV

	// GPU Memory stuff?
	DescriptorHeap m_ShaderVisibleResourceHeap; // CBV_SRV_UAV
	DescriptorHeap m_ShaderVisibleSamplerHeap;

	//--Ring Upload Buffer--
	UploadRingBuffer m_UploadBuffer;

public:
	void Initialize(GraphicsDevice* device);
	void Dispose();
	void FreeMemory(u64 frame);
	void Release(ID3D12Resource* resource);
	void Release(ID3D12PipelineState* pipeLine);
	void Release(ID3D12RootSignature* rootSig);
	void Release(DescriptorHandle handle, DescHeapType type);

	// Flushing is dangerous, only call if you 100% know GPU is finished,
	// i.e you called WaitForGPU() and want to release EVERYTHING in the queues
	void Flush();
};

class GraphicsDevice
{
private:
	GraphicsParameters	m_Parameters;
	IDXGIFactory1*		m_Factory		= nullptr;
	IDXGISwapChain*		m_SwapChain		= nullptr;
	ID3D12Device*		m_Device		= nullptr;
	ID3D12Debug1*		m_Debug			= nullptr;
	ID3D12DebugDevice*	m_DebugDevice	= nullptr;
	u32					m_FactoryFlags	= 0;

	u64					m_FrameFenceValue[BufferCount]; // Fence value for each Frame
	u64					m_FrameCount = 0; // Track how many frames have passed
	u32					m_BufferIndex = 0;

	//--Queues--
	CommandQueue		m_DirectQueue;
	CommandQueue		m_ComputeQueue;
	CommandQueue		m_CopyQueue;

	//--CommandLists--
	CommandListManager	m_DirectCommandList;
	CommandListManager	m_CopyCommandList;

	//--Back Buffers--
	TextureResource*	m_RenderTargets[BufferCount] = { nullptr };
	TextureResource*	m_DepthTargets[BufferCount] = { nullptr };

	//--Default View/Scissors--
	D3D12_VIEWPORT		m_DefaultView = {};
	D3D12_RECT			m_DefaultScissor = {};

	//--Memory managment handlers--
	MemoryHandler		m_MemoryHandler; // Handles Descriptors and deffered deletion of resources

public:
	GraphicsDevice();
	~GraphicsDevice() {};

public:
	// Should add Adapter LUID for VR headset support?
	void Initialize(GraphicsParameters& parameters);
	void Dispose();
	u64 FrameCount()const;
	u32 BufferIndex()const;
	ID3D12Device* NativeDevice();
	MemoryHandler* MemoryHandler();
	void WaitForGPU();
	void WaitForPreviousFrame();
	const GraphicsParameters& Parameters()const;

	//--Command List Functions--
	CommandList BeginCommandList(QueueType type);
	u64			SubmitCommandList(QueueType type, CommandList cmd);

	void Transition(GraphicsResource* resource, u32 subresource, ResourceState before, ResourceState after, CommandList cmd);
	void TransitionBackBuffer(ResourceState before, ResourceState after, CommandList cmd);
	void SetRootConstant32Bit(u32 rootIndex, u32 srcData, u32 offset, CommandList cmd);
	void SetRootConstant(std::shared_ptr<ConstantResource> buffer, u32 rootIndex, CommandList cmd);
	void SetGraphicsRootDescriptorTable(u32 rootIndex, const DescriptorHandle& handle, CommandList cmd);
	void SetDescriptors(DescriptorHeap* resourceHeap, DescriptorHeap* sampler, CommandList cmd);
	DescriptorHandle CopyDescriptors(DescriptorHeap& destAllocator, const DescriptorHandle& srcHandle, u32 count);
	void CopyBuffer(GraphicsResource* dest, u64 destOffset, GraphicsResource* src, u64 offset, u64 size, CommandList cmd);
	void BindVertexBuffer(std::shared_ptr<GraphicsResource>* vertexBuffer, u32 slot, u32* offsets, u32 count, CommandList cmd);
	void BindIndexBuffer(std::shared_ptr<GraphicsResource> indexBuffer, u32 offsets, CommandList cmd);
	void BindPipeline(std::shared_ptr<PipelineState> pipeLine, CommandList cmd);
	// Use nullptr for default frame target and depth
	void BindRenderTarget(std::shared_ptr<TextureResource> render, TextureResource* depth, CommandList cmd);
	void BindScissorRects(const Rect<u32>* pRects, u32 count, CommandList cmd);
	void BindViewports(const ViewPort* pViewports, u32 count, CommandList cmd);
	void BindDefaultViewAndScissor(CommandList cmd);

	//--Clear--
	void ClearRenderTarget(std::shared_ptr<TextureResource> target, const Color& color, CommandList cmd);
	void ClearDepthTarget(std::shared_ptr<TextureResource> target, float depth, u32 stencil, CommandList cmd);

	//--Draw Methods--
	void DrawInstanced(u32 vertexCount, u32 instanceCount, u32 startVertex, u32 startInstance, CommandList cmd);
	void DrawIndexedInstanced(u32 indexCount, u32 instanceCount, u32 indexStart, u32 vertexStart, u32 instanceLocation, CommandList cmd);
	// Presents to Screen
	void Present();

	//--Resource View Functions--
	DescriptorHandle CreateShaderResourceView(ResourceDesc& desc, ID3D12Resource* resource);
	DescriptorHandle CreateRenderTargetView(ResourceDesc& desc, ID3D12Resource* resource);
	DescriptorHandle CreateDepthStencilView(ResourceDesc& desc, ID3D12Resource* resource);
	DescriptorHandle CreateUnorderedAccessView(ResourceDesc& desc, ID3D12Resource* resource);
	DescriptorHandle CreateSamperState(SamplerDesc& desc);

	DXGI_FORMAT	GetResourceFormat(SurfaceFormat format, bool isDepth);

	// Debug compile shader, Debug only do this offline for production!!
	void CompilerShader(std::string fileName, const char* entry, const char* shadermodel, Byte** byteCode, u32& size);


	// Hate this
	ID3D12GraphicsCommandList* GetGraphicsCommandList(CommandList cmd);

private:
	void CreateSwapChain();
	D3D12_CPU_DESCRIPTOR_HANDLE HandleAsCPU(const DescriptorHandle& handle)const;
	D3D12_GPU_DESCRIPTOR_HANDLE HandleAsGPU(const DescriptorHandle& handle)const;
};