#include "Graphics/DX12/RootSignature.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "System/Assert.h"

// This is not a nice function, quite heavy should avoid or improve?
RootSignature::RootSignature(GraphicsDevice* device, RootSignatureDesc& desc): m_Desc(desc)
{
	HRESULT hr = S_OK;

	std::vector<D3D12_ROOT_PARAMETER>		parameters;
	std::vector<D3D12_STATIC_SAMPLER_DESC>	samplers;

	// Just done this so i can delete more effeciently, still horible code
	std::vector<D3D12_DESCRIPTOR_RANGE*>	descriptorRanges;

	for (size_t i = 0; i < desc.m_ParameterCount; ++i)
	{
		RootParameter& rootParam = m_Desc.m_Parameters[i];

		D3D12_ROOT_PARAMETER param = {};
		param.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)rootParam.m_Type;
		param.ShaderVisibility = (D3D12_SHADER_VISIBILITY)rootParam.m_Visibility;

		if (rootParam.m_StructType == RootStructType::Constant)
		{
			param.Constants.ShaderRegister = rootParam.m_Constants.m_ShaderRegister;
			param.Constants.RegisterSpace = rootParam.m_Constants.m_RegisterSpace;
			param.Constants.Num32BitValues = rootParam.m_Constants.m_Num32BitValues;
		}
		else if (rootParam.m_StructType == RootStructType::Descriptor)
		{
			param.Descriptor.ShaderRegister = rootParam.m_Constants.m_ShaderRegister;
			param.Descriptor.RegisterSpace = rootParam.m_Constants.m_RegisterSpace;
		}
		else if (rootParam.m_StructType == RootStructType::Table)
		{
			param.DescriptorTable.NumDescriptorRanges = rootParam.m_DescriptorTable.m_RangeCount;

			// Have to do it global otherwise memory lost
			D3D12_DESCRIPTOR_RANGE* ranges = new D3D12_DESCRIPTOR_RANGE[rootParam.m_DescriptorTable.m_RangeCount];

			for (size_t j = 0; j < rootParam.m_DescriptorTable.m_RangeCount; ++j)
			{
				ranges[j].NumDescriptors = rootParam.m_DescriptorTable.m_Ranges[j].m_Count;
				ranges[j].BaseShaderRegister = rootParam.m_DescriptorTable.m_Ranges[j].m_BaseShaderRegister;
				ranges[j].RegisterSpace = rootParam.m_DescriptorTable.m_Ranges[j].m_RegisterSpace;
				ranges[j].OffsetInDescriptorsFromTableStart = rootParam.m_DescriptorTable.m_Ranges[j].m_Offset;
				ranges[j].RangeType = (D3D12_DESCRIPTOR_RANGE_TYPE)rootParam.m_DescriptorTable.m_Ranges[j].m_RangeType;
			}

			param.DescriptorTable.pDescriptorRanges = ranges;
			descriptorRanges.push_back(ranges);
		}

		parameters.push_back(param);
	}

	for (size_t i = 0; i < desc.m_SamplerCount; ++i)
	{
		StaticSamplerDesc& samplerDesc = desc.m_StaticSamplers[i];
		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter			= ConversionTypes::ConvertFilter(samplerDesc.m_Filter);
		sampler.AddressU		= ConversionTypes::ConvertAddressMode(samplerDesc.m_AddressU);
		sampler.AddressV		= ConversionTypes::ConvertAddressMode(samplerDesc.m_AddressV);
		sampler.AddressW		= ConversionTypes::ConvertAddressMode(samplerDesc.m_AddressW);
		sampler.MaxAnisotropy	= samplerDesc.m_MaxAnisotropy;
		sampler.MipLODBias		= samplerDesc.m_MipLODBias;
		sampler.MinLOD			= samplerDesc.m_MinLOD;
		sampler.MaxLOD			= samplerDesc.m_MaxLOD;
		sampler.ComparisonFunc	= ConversionTypes::ConvertComparison(samplerDesc.m_Comparison);
		sampler.ShaderRegister	= samplerDesc.m_ShaderRegister;
		sampler.RegisterSpace	= samplerDesc.m_RegisterSpace;
		sampler.ShaderVisibility = (D3D12_SHADER_VISIBILITY)samplerDesc.m_Visibility;
		sampler.BorderColor		 = (D3D12_STATIC_BORDER_COLOR)samplerDesc.m_StaticBorderColor;

		samplers.push_back(sampler);
	}

	D3D12_ROOT_SIGNATURE_DESC rootDesc = {};
	rootDesc.Flags = (D3D12_ROOT_SIGNATURE_FLAGS)m_Desc.m_Flags;
	rootDesc.NumParameters = desc.m_ParameterCount;
	rootDesc.NumStaticSamplers = desc.m_SamplerCount;
	rootDesc.pParameters = parameters.data();
	rootDesc.pStaticSamplers = samplers.data();

	ID3DBlob* signature = nullptr;
	hr = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	assert(SUCCEEDED(hr));

	hr = m_Device->NativeDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_Root));
	assert(SUCCEEDED(hr));

	// Okay delete those annoying global data ranges
	// This is horrible code
	for (size_t i = 0; i < descriptorRanges.size(); ++i)
	{
		delete[] descriptorRanges[i];
	}

	signature->Release();
}

RootSignature::RootSignature(GraphicsDevice* device, ID3D12RootSignature* root)
{
	m_Device = device;
	m_Root = root;
}

RootSignature::~RootSignature()
{
	if (m_Root)
	{
		m_Device->MemoryHandler()->Release(m_Root);
	}
}

ID3D12RootSignature* RootSignature::NativeSignature()
{
	return m_Root;
}

void RootSignature::Dispose()
{
	if (m_Root)
	{
		m_Device->MemoryHandler()->Release(m_Root);
		m_Root = nullptr;
	}
}

std::shared_ptr<RootSignature> RootSignature::CreateFromShader(GraphicsDevice* device, Byte* data, u32 size)
{
	// Firstly try grab root signature, Vertex shader might have one more often than the others
	HRESULT hr = S_OK;
	ID3D12RootSignatureDeserializer* rootDeserializer = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	hr = D3D12CreateRootSignatureDeserializer(data, size, IID_PPV_ARGS(&rootDeserializer));
	if (SUCCEEDED(hr))
	{
		const D3D12_ROOT_SIGNATURE_DESC* rootDesc = rootDeserializer->GetRootSignatureDesc();

		hr = device->NativeDevice()->CreateRootSignature(0, data, size, IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));
	}

	return std::make_shared<RootSignature>(device, rootSignature);
}
