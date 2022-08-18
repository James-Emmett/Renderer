#pragma once
#include "System/Types.h"
#include "SamplerDesc.h"
#include <vector>

const u32 RangeOffsetAppend = 0xffffffff;

enum class RootSignatureFlag 
{
	None = 0,
	AllowInputAssembler = 0x1,
	DenyVertexShader = 0x2,
	DenyHullShader = 0x4,
	DenyDomainShader = 0x8,
	DenyGeometryShader = 0x10,
	DenyPixelShader = 0x20,
	AllowStreamOutput = 0x40,
	LocalRootSignature = 0x80,
	AmplificationAccess = 0x100,
	DenyMeshShader = 0x200,
	ResourceHeapDirectIndexed = 0x400,
	SamplerHeapDirectIndexed = 0x800
};

enum class RootParameterType
{
	DexcriptorTable,
	Constant32Bit,
	ConstantBuffer,
	ShaderResource,
	UnorderedAccess
};

enum class RootStructType
{
	Table,
	Constant,
	Descriptor
};

enum class ShaderVisibility 
{
	All = 0,
	Vertex = 1,
	Hull = 2,
	Domain = 3,
	Geometry = 4,
	Pixel = 5,
	Amplification = 6,
	Mesh = 7
};

enum class RangeType
{
	ShaderResource,
	UnorderedAccess,
	ConstantBuffer,
	Sampler
};

enum class StaticBorderColor 
{
	TransparentBlack,
	OpaqueBlack,
	OpaqueWhite
};

struct RootConstant
{
	u32 m_ShaderRegister; // Which register: t1,t2,t3 etc.
	u32 m_RegisterSpace;  // Allows Overlapping between shaders?
	u32 m_Num32BitValues; // Number of constants that occupy a single slot? I think this means Matrix spread across 4 slots act as single slot?

	RootConstant(u32 shaderReg = 0, u32 regSpace = 0, u32 values = 0)
	{
		m_ShaderRegister = shaderReg;
		m_RegisterSpace = regSpace;
		m_Num32BitValues = values;
	}
};

struct RootDescriptor
{
	u32 m_ShaderRegister;	// Which register: t1,t2,t3 etc.
	u32 m_RegisterSpace;	// Allows Overlapping between shaders?

	RootDescriptor(u32 shaderReg = 0, u32 regSpace = 0)
	{
		m_ShaderRegister	= shaderReg;
		m_RegisterSpace		= regSpace;
	}
};

struct DescriptorRange
{
	RangeType	m_RangeType;			// Type of Range, CBV etc
	u32			m_Count;				// How many descirptor in this range
	u32			m_BaseShaderRegister;	// Start index of shader register: b0, t3 etc.
	u32			m_RegisterSpace;		// Something to do with overlapping registers between shaders?
	u32			m_Offset;				// Offset from the start of root argument table? Just use Append?

	DescriptorRange(RangeType type = RangeType::ConstantBuffer, u32 count = 0, u32 baseReg = 0, u32 regSpace = 0, u32 offset = RangeOffsetAppend)
	{
		m_RangeType = type;
		m_Count = count;
		m_BaseShaderRegister = baseReg;
		m_RegisterSpace = regSpace;
		m_Offset = offset;
	}
};

struct RootTable 
{
	u32	m_RangeCount = 0;
	std::vector<DescriptorRange> m_Ranges;

	RootTable() { m_RangeCount = 0; };
	RootTable(u32 count, std::vector<DescriptorRange>& ranges)
	{
		m_RangeCount = count;
		m_Ranges = ranges;
	}

	RootTable(const RootTable& table)
	{
		m_RangeCount	= table.m_RangeCount;
		m_Ranges		= table.m_Ranges;
	}

	void AddRange(DescriptorRange& range)
	{
		m_Ranges.push_back(range);
		++m_RangeCount;
	}

	void operator=(const RootTable& table)
	{
		m_RangeCount	= table.m_RangeCount;
		m_Ranges		= table.m_Ranges;
	}
};

class RootParameter 
{
public:
	RootParameterType	m_Type;
	RootStructType		m_StructType;
	ShaderVisibility	m_Visibility;
	union 
	{
		RootTable		m_DescriptorTable;
		RootConstant    m_Constants;
		RootDescriptor  m_Descriptor;
	};

	RootParameter(RootParameterType type, ShaderVisibility visibility, RootTable table)
	{
		m_Type = type;
		m_StructType = RootStructType::Table;
		m_Visibility = visibility;
		m_DescriptorTable = table;
	}

	RootParameter(RootParameterType type, ShaderVisibility visibility, RootConstant constant)
	{
		m_Type = type;
		m_StructType = RootStructType::Constant;
		m_Visibility = visibility;
		m_Constants = constant;
	}

	RootParameter(RootParameterType type, ShaderVisibility visibility, RootDescriptor descriptor)
	{
		m_Type = type;
		m_StructType = RootStructType::Descriptor;
		m_Visibility = visibility;
		m_Descriptor = descriptor;
	}

	RootParameter(const RootParameter& parameter)
	{
		m_Type			= parameter.m_Type;
		m_StructType	= parameter.m_StructType;
		m_Visibility	= parameter.m_Visibility;

		if (m_StructType == RootStructType::Constant)
		{
			m_Constants = parameter.m_Constants;
		}
		else if (m_StructType == RootStructType::Descriptor)
		{
			m_Descriptor = parameter.m_Descriptor;
		}
		else if (m_StructType == RootStructType::Table)
		{
			m_DescriptorTable = parameter.m_DescriptorTable;
		}
	}

	~RootParameter(){}

	void operator=(const RootParameter& parameter)
	{
		m_Type			= parameter.m_Type;
		m_StructType	= parameter.m_StructType;
		m_Visibility	= parameter.m_Visibility;

		if (m_StructType == RootStructType::Constant)
		{
			m_Constants = parameter.m_Constants;
		}
		else if (m_StructType == RootStructType::Descriptor)
		{
			m_Descriptor = parameter.m_Descriptor;
		}
		else if (m_StructType == RootStructType::Table)
		{
			m_DescriptorTable = parameter.m_DescriptorTable;
		}
	}
};

struct StaticSamplerDesc : SamplerDesc
{
	u32 m_ShaderRegister = 0;
	u32 m_RegisterSpace = 0;
	ShaderVisibility m_Visibility = ShaderVisibility::All;
	StaticBorderColor m_StaticBorderColor = StaticBorderColor::OpaqueBlack;

	StaticSamplerDesc() {}
	StaticSamplerDesc(u32 shaderReg, u32 regSpace, ShaderVisibility visibility, StaticBorderColor color, SamplerDesc& desc) :
		SamplerDesc(desc.m_Filter, desc.m_AddressU, desc.m_AddressV, desc.m_AddressW, desc.m_MipLODBias, 
					desc.m_MaxAnisotropy, desc.m_Comparison, desc.m_Color, desc.m_MinLOD, desc.m_MaxLOD)
	{
		m_ShaderRegister = shaderReg;
		m_RegisterSpace = regSpace;
		m_Visibility = visibility;
		m_StaticBorderColor = color;
	}
};

class RootSignatureDesc
{
public:
	u32								m_ParameterCount = 0;
	std::vector<RootParameter>		m_Parameters;
	u32								m_SamplerCount = 0;
	std::vector<StaticSamplerDesc>	m_StaticSamplers;
	RootSignatureFlag				m_Flags = RootSignatureFlag::None;

	RootSignatureDesc() {}
	RootSignatureDesc(std::vector<RootParameter>& parameters, std::vector<StaticSamplerDesc>& staticSamplers)
	{
		m_ParameterCount	= (u32)parameters.size();
		m_Parameters		= parameters;
		m_SamplerCount		= (u32)staticSamplers.size();
		m_StaticSamplers	= staticSamplers;
	}

	void AddParameter(RootParameter& parameter)
	{
		m_Parameters.push_back(parameter);
		++m_ParameterCount;
	}

	void AddSampler(StaticSamplerDesc& samplerDesc)
	{
		m_StaticSamplers.push_back(samplerDesc);
		++m_SamplerCount;
	}
};