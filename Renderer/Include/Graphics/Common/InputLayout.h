#pragma once
#include "System/Types.h"
#include "Graphics/Common/SurfaceFormat.h"
#include <vector>
#include <string>

enum class InputType
{
	PerVertex,
	Instance
};

enum class VertexAttribute
{
	Position,
	Normal,
	Tangent,
	Color,
	TexCoord,
	BlendWeight,
	BlendIndices
};

struct VertexAttributeDesc
{
	VertexAttribute m_Attribute;
	SurfaceFormat	m_Format;
	u32				m_Slot;
	u32				m_Index;
	InputType		m_Type;
	u32				m_Rate;

	VertexAttributeDesc(VertexAttribute attribute =  VertexAttribute::Position, SurfaceFormat format = SurfaceFormat::R32G32B32_Float, u32 slot = 0, u32 index = 0, InputType type = InputType::PerVertex, u32 rate = 0)
	{
		m_Attribute	= attribute;
		m_Format	= format;
		m_Slot		= slot;
		m_Index		= index;
		m_Type		= type;
		m_Rate		= rate;
	}
};

std::string VertexAttributeToString(VertexAttribute attribute);

struct InputLayout
{
public:
	static const u32 Aligned_Element = 0xFFFFFFFF;

private:
	std::vector<VertexAttributeDesc> m_Attributes;
	u32 m_Stride = 0;

public:
	InputLayout() { }

	// Initialize from list, prefer this
	InputLayout(std::initializer_list<VertexAttributeDesc> elements) : m_Attributes(elements)
	{
		CalculateStride();
	}

	// Copy from vector
	InputLayout(std::vector<VertexAttributeDesc>& elements)
	{
		m_Attributes = elements;
		CalculateStride();
	}

	// Can add one by one not recommended.
	void AddAttribute(VertexAttributeDesc& element)
	{
		m_Attributes.push_back(element);
		// Add the stride of this attribute to our stride count
		m_Stride += TextureHelper::BytesPerBlock(element.m_Format);
	}

	u32 Count()const
	{
		return (u32)m_Attributes.size();
	}

	bool IsEmpty()const
	{
		return m_Attributes.empty();
	}

	u32 Stride()
	{
		m_Stride;
	}

	const std::vector<VertexAttributeDesc>& Attributes()const
	{
		return m_Attributes;
	}

private:
	void CalculateStride()
	{
		m_Stride = 0;

		// Loop through and accumulate each size
		for (size_t i = 0; i < m_Attributes.size(); ++i)
		{
			m_Stride += TextureHelper::BytesPerBlock(m_Attributes[i].m_Format);
		}
	}
};