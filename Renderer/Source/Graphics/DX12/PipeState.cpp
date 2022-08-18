#include "Graphics/DX12/PipelineState.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "System/Assert.h"

PipelineState::PipelineState(GraphicsDevice* device, std::shared_ptr<RootSignature> root, PipelineDesc& desc): 
	m_Device(device), m_Root(root), m_Desc(desc)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeDesc = {};

	// Grab the active shaders
	if (desc.Vertex.IsValid())
	{
		pipeDesc.VS.pShaderBytecode = desc.Vertex.m_Data;
		pipeDesc.VS.BytecodeLength = desc.Vertex.m_Size;
	}

	if (desc.Pixel.IsValid())
	{
		pipeDesc.PS.pShaderBytecode = desc.Pixel.m_Data;
		pipeDesc.PS.BytecodeLength = desc.Pixel.m_Size;
	}

	if (desc.Hull.IsValid())
	{
		pipeDesc.HS.pShaderBytecode = desc.Hull.m_Data;
		pipeDesc.HS.BytecodeLength = desc.Hull.m_Size;
	}

	if (desc.Domain.IsValid())
	{
		pipeDesc.DS.pShaderBytecode = desc.Domain.m_Data;
		pipeDesc.DS.BytecodeLength = desc.Domain.m_Size;
	}

	if (desc.Geometry.IsValid())
	{
		pipeDesc.GS.pShaderBytecode = desc.Geometry.m_Data;
		pipeDesc.GS.BytecodeLength = desc.Geometry.m_Size;
	}

	// Raster state
	D3D12_RASTERIZER_DESC rDesc = {};
	rDesc.FillMode				= ConversionTypes::ConvertFillMode(desc.RasterState.Fill);
	rDesc.CullMode				= ConversionTypes::ConvertCullMode(desc.RasterState.Cull);
	rDesc.DepthBias				= desc.RasterState.DepthBias;
	rDesc.DepthBiasClamp		= desc.RasterState.DepthBiasClamp;
	rDesc.SlopeScaledDepthBias	= desc.RasterState.SlopeScaledDepthBias;
	rDesc.DepthClipEnable		= desc.RasterState.DepthClip;
	rDesc.ForcedSampleCount		= desc.RasterState.ForcedSampleCount;
	rDesc.FrontCounterClockwise = desc.RasterState.FrontCounterClockwise;
	rDesc.MultisampleEnable		= desc.RasterState.MultiSampled;
	rDesc.AntialiasedLineEnable = desc.RasterState.AntialisedLine;
	rDesc.ConservativeRaster	= desc.RasterState.ConservativeRaster ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	pipeDesc.RasterizerState = rDesc;

	// Depth Desc
	D3D12_DEPTH_STENCIL_DESC dDesc = {};
	dDesc.DepthEnable		= desc.DepthStencilState.DepthEnabled;
	dDesc.DepthWriteMask	= ConversionTypes::ConvertDepthWrite(desc.DepthStencilState.WriteMask);
	dDesc.DepthFunc			= ConversionTypes::ConvertComparison(desc.DepthStencilState.DepthFunc);
	dDesc.StencilEnable		= desc.DepthStencilState.StencilEnabled;
	dDesc.StencilReadMask	= desc.DepthStencilState.StencilRead;
	dDesc.StencilWriteMask	= desc.DepthStencilState.StencilWrite;

	// Stencil Front Operations
	dDesc.FrontFace.StencilFailOp		= ConversionTypes::ConvertStencilOp(desc.DepthStencilState.FrontFace.FailOp);
	dDesc.FrontFace.StencilPassOp		= ConversionTypes::ConvertStencilOp(desc.DepthStencilState.FrontFace.PassOp);
	dDesc.FrontFace.StencilDepthFailOp	= ConversionTypes::ConvertStencilOp(desc.DepthStencilState.FrontFace.DepthFailOp);
	dDesc.FrontFace.StencilFunc			= ConversionTypes::ConvertComparison(desc.DepthStencilState.FrontFace.StencilFunc);

	// Stencil Back Operations
	dDesc.BackFace.StencilFailOp		= ConversionTypes::ConvertStencilOp(desc.DepthStencilState.BackFace.FailOp);
	dDesc.BackFace.StencilPassOp		= ConversionTypes::ConvertStencilOp(desc.DepthStencilState.BackFace.PassOp);
	dDesc.BackFace.StencilDepthFailOp	= ConversionTypes::ConvertStencilOp(desc.DepthStencilState.BackFace.DepthFailOp);
	dDesc.BackFace.StencilFunc			= ConversionTypes::ConvertComparison(desc.DepthStencilState.BackFace.StencilFunc);

	pipeDesc.DepthStencilState = dDesc;

	// Blend Desc
	D3D12_BLEND_DESC bDesc = {};
	bDesc.AlphaToCoverageEnable = desc.BlendState.AlphaToCoverage;
	bDesc.IndependentBlendEnable = desc.BlendState.IndependentBlend;

	for (size_t i = 0; i < 8; ++i)
	{
		bDesc.RenderTarget[i].BlendEnable			= desc.BlendState.RenderBlend[i].BlendEnabled;
		bDesc.RenderTarget[i].SrcBlend				= ConversionTypes::ConvertBlend(desc.BlendState.RenderBlend[i].SrcColorBlend);
		bDesc.RenderTarget[i].DestBlend				= ConversionTypes::ConvertBlend(desc.BlendState.RenderBlend[i].DestColorBlend);
		bDesc.RenderTarget[i].BlendOp				= ConversionTypes::ConvertBlendOp(desc.BlendState.RenderBlend[i].ColorBlendOp);
		bDesc.RenderTarget[i].SrcBlendAlpha			= ConversionTypes::ConvertBlend(desc.BlendState.RenderBlend[i].SrcAlphaBlend);
		bDesc.RenderTarget[i].DestBlendAlpha		= ConversionTypes::ConvertBlend(desc.BlendState.RenderBlend[i].DestAlphaBlend);
		bDesc.RenderTarget[i].BlendOpAlpha			= ConversionTypes::ConvertBlendOp(desc.BlendState.RenderBlend[i].AlphaBlendOp);
		bDesc.RenderTarget[i].RenderTargetWriteMask = ConversionTypes::ConvertBlendWrite((ColorWrite)desc.BlendState.RenderBlend[i].ColorWriteMask);

		// What are these?
		bDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
		bDesc.RenderTarget[i].LogicOpEnable = false;
		// May as well use this loop to grab the formats as well
		pipeDesc.RTVFormats[i] = ConversionTypes::ConvertFormat((SurfaceFormat)desc.RenderTargets[i]);
	}

	pipeDesc.BlendState = bDesc;

	// Input Layout
	D3D12_INPUT_LAYOUT_DESC lDesc = {};
	lDesc.NumElements = desc.Layout.Count();
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs;
	const std::vector<VertexAttributeDesc>& attributes = desc.Layout.Attributes();

	std::vector<std::string> semanticNames(attributes.size());

	for (size_t i = 0; i < lDesc.NumElements; ++i)
	{
		D3D12_INPUT_ELEMENT_DESC eDesc = {};
		semanticNames[i]			= VertexAttributeToString(attributes[i].m_Attribute);
		eDesc.SemanticName			= semanticNames[i].c_str();
		eDesc.Format				= ConversionTypes::ConvertFormat(attributes[i].m_Format);
		eDesc.InputSlot				= attributes[i].m_Slot;
		eDesc.SemanticIndex			= attributes[i].m_Index;
		eDesc.AlignedByteOffset		= InputLayout::Aligned_Element;
		eDesc.InputSlotClass		= ConversionTypes::ConvertInputClass(attributes[i].m_Type);
		eDesc.InstanceDataStepRate  = attributes[i].m_Rate;
		elementDescs.push_back(eDesc);
	}

	lDesc.pInputElementDescs = elementDescs.data(); // Hopefully copied during create ?
	pipeDesc.InputLayout = lDesc;

	pipeDesc.SampleMask				= desc.SamplerMask;
	pipeDesc.SampleDesc.Count		= desc.MultiSample.Count;
	pipeDesc.SampleDesc.Quality		= desc.MultiSample.Quality;
	pipeDesc.PrimitiveTopologyType	= ConversionTypes::ConvertPipeTopology(desc.Topology);
	pipeDesc.NodeMask				= 0;
	pipeDesc.NumRenderTargets		= desc.NumRenderTargets;
	pipeDesc.DSVFormat				= m_Device->GetResourceFormat((SurfaceFormat)desc.DepthTarget, true);

	// Create the actual pipline state
	HRESULT hr = m_Device->NativeDevice()->CreateGraphicsPipelineState(&pipeDesc, IID_PPV_ARGS(&m_PipeLine));
	assert(SUCCEEDED(hr));
}

PipelineState::~PipelineState()
{
	Release();
}

void PipelineState::Release()
{
	if (m_PipeLine)
	{
		m_Device->MemoryHandler()->Release(m_PipeLine);
	}

	if (m_Root)
	{
		m_Root->Dispose();
	}
}

const PipelineDesc& PipelineState::GetDesc()const
{
	return m_Desc;
}

PrimitiveTopology PipelineState::Topology() const
{
	return m_Desc.Topology;
}

ID3D12PipelineState* PipelineState::NativePipeline()
{
	return m_PipeLine;
}

ID3D12RootSignature* PipelineState::NativeRootSignature()
{
	return m_Root->NativeSignature();
}
