#pragma once
#include <d3d12.h>
#include "Graphics/Common/PipelineDesc.h"
#include "Graphics/DX12/RootSignature.h"

class GraphicsDevice;
class PipelineState
{
protected:
	PipelineDesc					m_Desc;
	GraphicsDevice*					m_Device;
	ID3D12PipelineState*			m_PipeLine = nullptr;
	std::shared_ptr<RootSignature>	m_Root = nullptr;

public:
	PipelineState(GraphicsDevice* device, std::shared_ptr<RootSignature> root,  PipelineDesc& desc);
	~PipelineState();

public:
	void Release();
	const PipelineDesc&  GetDesc()const;
	PrimitiveTopology	 Topology()const;
	ID3D12PipelineState* NativePipeline();
	ID3D12RootSignature* NativeRootSignature();
};