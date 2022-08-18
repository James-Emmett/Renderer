#pragma once
#include "PipelineDesc.h"
#include "SamplerDesc.h"

namespace CommonStates
{
	//--Common Descriptors--
	const BlendDesc Opaque =
	{
		false, // AlphaToCoverageEnable
		false, // IndependentBlendEnable
		{ {
			false,        // BlendEnable
			Blend::One,   // SrcBlend
			Blend::Zero,  // DestBlend
			BlendOp::Add, // BlendOp
			Blend::One,   // SrcBlendAlpha
			Blend::Zero,  // DestBlendAlpha
			BlendOp::Add, // BlendOpAlpha
			(u8)ColorWrite::All
		} }
	};

	const BlendDesc AlphaBlend =
	{
		false, // AlphaToCoverageEnable
		false, // IndependentBlendEnable
		{ {
			true,	              // BlendEnable
			Blend::One,           // SrcBlend
			Blend::Inv_Src_Alpha, // DestBlend
			BlendOp::Add,		  // BlendOp
			Blend::One,			  // SrcBlendAlpha
			Blend::Inv_Src_Alpha, // DestBlendAlpha
			BlendOp::Add,	      // BlendOpAlpha
			(u8)ColorWrite::All
		} }
	};

	const BlendDesc Additive =
	{
		false, // AlphaToCoverageEnable
		false, // IndependentBlendEnable
		{ {
			true, // BlendEnable
			Blend::Src_Alpha, // SrcBlend
			Blend::One,	      // DestBlend
			BlendOp::Add,     // BlendOp
			Blend::Src_Alpha, // SrcBlendAlpha
			Blend::One,       // DestBlendAlpha
			BlendOp::Add,     // BlendOpAlpha
			(u8)ColorWrite::All
		} }
	};

	const BlendDesc NonPremultiplied =
	{
		false, // AlphaToCoverageEnable
		false, // IndependentBlendEnable
		{ {
			true,		          // BlendEnable
			Blend::Src_Alpha,	  // SrcBlend
			Blend::Inv_Src_Alpha, // DestBlend
			BlendOp::Add,		  // BlendOp
			Blend::Src_Alpha,     // SrcBlendAlpha
			Blend::Inv_Src_Alpha, // DestBlendAlpha
			BlendOp::Add,        // BlendOpAlpha
			(u8)ColorWrite::All
		} }
	};

	const DepthStencilDesc DepthNone =
	{
		false,						// DepthEnable
		DepthWrite::Zero,			// Depth Mask
		ComparisonFunc::LessEqual,	// DepthFunc
		false,                      // StencilEnable
		0xFF,						// Stencil Read
		0xFF,						// Stencil Write
		{
			StencilOp::Keep,        // StencilFailOp
			StencilOp::Keep,        // StencilDepthFailOp
			StencilOp::Keep,        // StencilPassOp
			ComparisonFunc::Always  // StencilFunc
		}, // FrontFace
		{
			StencilOp::Keep,        // StencilFailOp
			StencilOp::Keep,        // StencilDepthFailOp
			StencilOp::Keep,        // StencilPassOp
			ComparisonFunc::Always  // StencilFunc
		} // BackFace
	};

	const DepthStencilDesc DepthDefault =
	{
		true,						// DepthEnable
		DepthWrite::All,			// Depth Mask
		ComparisonFunc::LessEqual,	// DepthFunc
		false,						// StencilEnable
		0xFF,						// Stencil Read
		0xFF,						// Stencil Write
		{
			StencilOp::Keep,       // StencilFailOp
			StencilOp::Keep,       // StencilDepthFailOp
			StencilOp::Keep,       // StencilPassOp
			ComparisonFunc::Always // StencilFunc
		}, // FrontFace
		{
			StencilOp::Keep,       // StencilFailOp
			StencilOp::Keep,       // StencilDepthFailOp
			StencilOp::Keep,       // StencilPassOp
			ComparisonFunc::Always // StencilFunc
		} // BackFace
	};

	const DepthStencilDesc DepthRead =
	{
		true,						// DepthEnable
		DepthWrite::Zero,			// Depth Mask
		ComparisonFunc::LessEqual,	// DepthFunc
		false,						// StencilEnable
		0xFF,						// Stencil Read
		0xFF,						// Stencil Write
		{
			StencilOp::Keep,       // StencilFailOp
			StencilOp::Keep,       // StencilDepthFailOp
			StencilOp::Keep,       // StencilPassOp
			ComparisonFunc::Always // StencilFunc
		}, // FrontFace
		{
			StencilOp::Keep,       // StencilFailOp
			StencilOp::Keep,       // StencilDepthFailOp
			StencilOp::Keep,       // StencilPassOp
			ComparisonFunc::Always // StencilFunc
		} // BackFace
	};

	const RasterDesc CullNone =
	{
		FillMode::Solid,	// Fill Mode
		CullMode::None,		// Cull Mode
		0,					// DepthBias
		0.0f,				// DepthBiasClamp
		0.0f,				// SlopeScaleBias
		true,				// DepthClipEnable
		false,				// MultisampleEnable
		false,				// AntialiasedLineEnable
		false,				// ConservativeRaster
		0,					// ForcedSampleCount
		false,				// FrontCounterClockwise
	};

	const RasterDesc CullClockwise =
	{
		FillMode::Solid,	// Fill Mode
		CullMode::Front,		// Cull Mode
		0,					// DepthBias
		0.0f,				// DepthBiasClamp
		0.0f,				// SlopeScaleBias
		true,				// DepthClipEnable
		false,				// MultisampleEnable
		false,				// AntialiasedLineEnable
		false,				// ConservativeRaster
		0,					// ForcedSampleCount
		false,				// FrontCounterClockwise
	};

	const RasterDesc CullCounterClockwise =
	{
		FillMode::Solid,	// Fill Mode
		CullMode::Back,		// Cull Mode
		0,					// DepthBias
		0.0f,				// DepthBiasClamp
		0.0f,				// SlopeScaleBias
		true,				// DepthClipEnable
		false,				// MultisampleEnable
		false,				// AntialiasedLineEnable
		false,				// ConservativeRaster
		0,					// ForcedSampleCount
		false,				// FrontCounterClockwise
	};

	const RasterDesc Wireframe =
	{
		FillMode::Wireframe,	// Fill Mode
		CullMode::None,			// Cull Mode
		0,						// DepthBias
		0.0f,					// DepthBiasClamp
		0.0f,					// SlopeScaleBias
		true,					// DepthClipEnable
		false,					// MultisampleEnable
		false,					// AntialiasedLineEnable
		false,					// ConservativeRaster
		0,						// ForcedSampleCount
		false,					// FrontCounterClockwise
	};

	const SamplerDesc PointWrap =
	{
		FilterMode::MinMagMipPoint,	// Filter Mode
		WrapMode::Repeat,			// AddressU
		WrapMode::Repeat,			// AddressV
		WrapMode::Repeat,			// AddressW
		0,							// MipLODBias
		0,							// Max Anisotropy
		ComparisonFunc::Never,		// Comparison
		Color::Black,				// BorderColor
		0,							// MinLOD
		FLT_MAX,					// MaxLOD
	};

	const SamplerDesc PointClamp =
	{
		FilterMode::MinMagMipPoint,	// FilterMode
		WrapMode::Clamp,			// AddressU
		WrapMode::Clamp,			// AddressV
		WrapMode::Clamp,			// AddressW
		0,							// MipLODBias
		0,							// Max Anisotropy
		ComparisonFunc::Never,		// Comparison
		Color::Black,				// BorderColor
		0,							// MinLOD
		FLT_MAX,					// MaxLOD
	};

	const SamplerDesc LinearWrap =
	{
		FilterMode::MinMagMipLinear,	// FilterMode
		WrapMode::Repeat,				// AddressU
		WrapMode::Repeat,				// AddressV
		WrapMode::Repeat,				// AddressW
		0,								// MipLODBias
		0,								// Max Anisotropy
		ComparisonFunc::Never,			// Comparison
		Color::Black,					// BorderColor
		0,								// MinLOD
		FLT_MAX,						// MaxLOD
	};

	const SamplerDesc LinearClamp =
	{
		FilterMode::MinMagMipLinear,	// FilterMode
		WrapMode::Clamp,				// AddressU
		WrapMode::Clamp,				// AddressV
		WrapMode::Clamp,				// AddressW
		0,								// MipLODBias
		0,								// Max Anisotropy
		ComparisonFunc::Never,			// Comparison
		Color::Black,					// Border Color
		0,								// MinLOD
		FLT_MAX,						// MaxLOD
	};

	const SamplerDesc AnisotropicWrap =
	{
		FilterMode::Anisotropic,	// FilterMode
		WrapMode::Repeat,			// AddressU
		WrapMode::Repeat,			// AddressV
		WrapMode::Repeat,			// AddressW
		0,							// MipLODBias
		0,							// Max Anisotropy
		ComparisonFunc::Never,		// Comparison
		Color::Black,
		0,							// MinLOD
		FLT_MAX,					// MaxLOD
	};

	const SamplerDesc AnisotropicClamp =
	{
		FilterMode::Anisotropic,	// FilterMode
		WrapMode::Clamp,			// AddressU
		WrapMode::Clamp,			// AddressV
		WrapMode::Clamp,			// AddressW
		0,							// MipLODBias
		0,							// MaxAnisotropy
		ComparisonFunc::Never,		// Border Color
		Color::Black,
		0,							// MinLOD
		FLT_MAX,					// MaxLOD
	};
};