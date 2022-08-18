#pragma once
#include "InputLayout.h"
#include "ComparisonFunction.h"
#include "MultiSample.h"

enum class PrimitiveTopology
{
	Undefined,
	Point,
	Line,
	Triangle,
	Patch,
};

enum class FillMode
{
	Wireframe,
	Solid
};

enum class CullMode
{
	None,
	Front,
	Back
};

enum class Blend
{
	Zero,
	One,
	Src_Color,
	Inv_Src_Color,
	Src_Alpha,
	Inv_Src_Alpha,
	Dest_Color,
	Inv_Dest_Color,
	Dest_Alpha,
	Inv_Dest_Alpha,
	Blend_Factor,
	Inv_Blend_Factor,
	Src1_Color,
	Inv_Src1_Color,
	Src1_Alpha,
	Inv_Src1_Alpha,
	Src_Alpha_Sat
};

enum class BlendOp
{
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max
};

enum class ColorWrite
{
	Red,
	Green,
	Blue,
	Alpha,
	All
};

enum class StencilOp
{
	Zero,
	Keep,
	Replace,
	Increment,
	Decrement,
	IncrementSat,
	DecrementSat,
	Invert
};

enum class DepthWrite
{
	Zero,
	All
};

struct RenderTargetBlend
{
	bool	BlendEnabled = false;
	Blend	SrcColorBlend = Blend::One;
	Blend	DestColorBlend = Blend::Zero;
	BlendOp ColorBlendOp = BlendOp::Add;
	Blend	SrcAlphaBlend = Blend::One;
	Blend	DestAlphaBlend = Blend::Zero;
	BlendOp AlphaBlendOp = BlendOp::Add;
	u8		ColorWriteMask = (u8)ColorWrite::All;
};

struct BlendDesc
{
	bool AlphaToCoverage = false;
	bool IndependentBlend = false;
	RenderTargetBlend RenderBlend[8];
};

// Write up about bias/shadow acne
//https://digitalrune.github.io/DigitalRune-Documentation/html/3f4d959e-9c98-4a97-8d85-7a73c26145d7.htm
struct RasterDesc
{
	FillMode	Fill					= FillMode::Solid;
	CullMode	Cull					= CullMode::Back;
	int			DepthBias				= 0;	// Move pixels by depth offset during render
	float		DepthBiasClamp			= 0.0f; // Clamp the pixel shift
	float		SlopeScaledDepthBias	= 0.0f; // Move sloped pixels by a biased amount
	bool		DepthClip				= true;
	bool		MultiSampled			= false;
	bool		AntialisedLine			= false;
	bool		ConservativeRaster		= false;
	u32			ForcedSampleCount		= 0;
	bool		FrontCounterClockwise	= false;
};

struct DepthStencilOp
{
	StencilOp		FailOp		= StencilOp::Keep;		 // What to do when stencil fails
	StencilOp		DepthFailOp = StencilOp::Keep;		 // What to do when Stencil pass but depth fail.
	StencilOp		PassOp		= StencilOp::Keep;		 // What to do when Stencil and depth pass.
	ComparisonFunc	StencilFunc = ComparisonFunc::Never; // How stencil values compared
};

struct DepthStencilDesc
{
	bool			DepthEnabled	= true;
	DepthWrite		WriteMask		= DepthWrite::All;
	ComparisonFunc	DepthFunc		= ComparisonFunc::Less;
	bool			StencilEnabled	= false;
	u8				StencilRead		= 0xFF;
	u8				StencilWrite	= 0xFF;
	DepthStencilOp	FrontFace;
	DepthStencilOp	BackFace;
};


struct ShaderData
{
	Byte*	m_Data = nullptr;
	u32		m_Size = 0;

	ShaderData(Byte* data = nullptr, u32 size = 0)
	{
		m_Data = data;
		m_Size = size;
	}

	bool IsValid()
	{
		return m_Data != nullptr;
	}
};

struct PipelineDesc
{
	ShaderData			Vertex;
	ShaderData			Pixel;
	ShaderData			Hull;
	ShaderData			Domain;
	ShaderData			Geometry;
	BlendDesc			BlendState;
	RasterDesc			RasterState;
	DepthStencilDesc	DepthStencilState;
	InputLayout			Layout;
	PrimitiveTopology	Topology = PrimitiveTopology::Triangle;
	MultiSampleDesc		MultiSample;
	RenderFormat		RenderTargets[8] = { RenderFormat::R8G8B8A8_Unorm };
	DepthFormat			DepthTarget = DepthFormat::Depth32;
	u32					NumRenderTargets = 1;
	u32					SamplerMask = 0xFFFFFFFF;
	// Make some root signature object?

	PipelineDesc() {}
};