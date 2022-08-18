#pragma once
#include "System/Types.h"
#include "ComparisonFunction.h"
#include "Math/Color.h"

const float MinLod = 1.175494351e-38F;
const float MaxLod = 3.402823466e+38F;

enum class TextureType
{
	Texture1D,
	Texture2D,
	Texture3D,
	TextureCube
};

enum class WrapMode
{
	Repeat,
	Mirror,
	Clamp,
	Border,
	Mirror_Once
};

enum class FilterMode
{
	MinMagMipPoint,
	MinMagPointMipLinear,
	MinPointMagLinearMipPoint,
	MinPointMagMipLinear,
	MininearMagMipPoint,
	MinLinearMagPointMipLinear,
	MinMagLinearMipPoint,
	MinMagMipLinear,
	Anisotropic,
	ComparisonMinMagMipPoint,
	ComparisonMinMagPointMipLinear,
	ComparisonMinPointMagLinearMipPoint,
	ComparisonMinPointMagMipLinear,
	ComparisonMinLinearMagMipPoint,
	ComparisonMinLinearMagPointMipLinear,
	ComparisonMinMagLinearMipPoint,
	ComparisonMinMagMipLinear,
	ComparisonAnisotropic,
	MinimunMinMagMipPoint,
	MinimunMinMagPointMipLinear,
	MinimunMinPointMagLinearMipPoint,
	MinimunMinPointMagMipLinear,
	MinimunMinLinearMagMipPoint,
	MinimunMinLinearMagPointMipLinear,
	MinimunMinMagLinearMipPoint,
	MinimunMinMagMipLinear,
	MinimunAnisotropic,
	MaximunMinMagMipPoint,
	MaximunMinMagPointMipLinear,
	MaximunMinPointMagLinearMipPoint,
	MaximunMinPointMagMipLinear,
	MaximunMinLinearMagMipPoint,
	MaximunMinLinearMagPointMipLinear,
	MaximunMinMagLinearMipPoint,
	MaximunMinMagMipLinear,
	MaximunAnisotropic,
};

struct SamplerDesc
{
	FilterMode		m_Filter = FilterMode::MinPointMagMipLinear;
	WrapMode		m_AddressU = WrapMode::Repeat;
	WrapMode		m_AddressV = WrapMode::Repeat;
	WrapMode		m_AddressW = WrapMode::Repeat;
	float			m_MipLODBias = 0;
	u32				m_MaxAnisotropy = 1;
	ComparisonFunc	m_Comparison = ComparisonFunc::Never;
	Color			m_Color = Color::Black;
	float			m_MinLOD = MinLod;
	float			m_MaxLOD = MaxLod;

	SamplerDesc(FilterMode mode = FilterMode::MinPointMagMipLinear, WrapMode u = WrapMode::Repeat, WrapMode v = WrapMode::Repeat, WrapMode w = WrapMode::Repeat,
				float lodBias = 0, u32 anisotropy = 1, ComparisonFunc compare = ComparisonFunc::Never, Color color = Color::Black, float minLod = MinLod, float maxLod = MaxLod)
	{
		m_Filter = mode;
		m_AddressU = u;
		m_AddressV = v;
		m_AddressW = w;
		m_MipLODBias = lodBias;
		m_MaxAnisotropy = anisotropy;
		m_Comparison = ComparisonFunc::Never;
		m_Color = color;
		m_MinLOD = minLod;
		m_MaxLOD = maxLod;
	}
};

