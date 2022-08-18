#include "Graphics/Common/VertexFormats.h"

//----------------------------------------------------------------------------
const InputLayout Vertex::inputLayout =
{
	{ VertexAttribute::Position, SurfaceFormat::R32G32B32_Float, 0, 0, InputType::PerVertex, 0 }
};

//-----------------------------------------------------------------------------
const InputLayout VertexTexture::inputLayout =
{
	{ VertexAttribute::Position , SurfaceFormat::R32G32B32_Float, 0, 0,  InputType::PerVertex, 0 },
	{ VertexAttribute::TexCoord , SurfaceFormat::R32G32_Float, 0, 0,  InputType::PerVertex, 0 }
};

//-----------------------------------------------------------------------------
const InputLayout VertexColor::inputLayout =
{
	{ VertexAttribute::Position , SurfaceFormat::R32G32B32_Float, 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Color    , SurfaceFormat::R32G32B32A32_Float,0, 0, InputType::PerVertex, 0 },
};

//-----------------------------------------------------------------------------
const InputLayout VertexColorTexture::inputLayout =
{
	{ VertexAttribute::Position  , SurfaceFormat::R32G32B32_Float, 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Color     , SurfaceFormat::R32G32B32A32_Float, 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::TexCoord  , SurfaceFormat::R32G32_Float, 0, 0,  InputType::PerVertex, 0 }
};

//-----------------------------------------------------------------------------
const InputLayout VertexNormal::inputLayout =
{
	{ VertexAttribute::Position	, SurfaceFormat::R32G32B32_Float   , 0,  0, InputType::PerVertex, 0},
	{ VertexAttribute::Normal	, SurfaceFormat::R32G32B32_Float   , 0,  0, InputType::PerVertex, 0},
	{ VertexAttribute::Color    , SurfaceFormat::R32G32B32A32_Float, 0,  0, InputType::PerVertex, 0},
	{ VertexAttribute::TexCoord , SurfaceFormat::R32G32_Float      , 0,  0, InputType::PerVertex, 0}
};

//-----------------------------------------------------------------------------
const InputLayout VertexMesh::inputLayout =
{
	{ VertexAttribute::Position	 , SurfaceFormat::R32G32B32_Float   , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Normal	 , SurfaceFormat::R32G32B32_Float   , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Tangent	 , SurfaceFormat::R32G32B32A32_Float	, 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Color	 , SurfaceFormat::R32G32B32A32_Float, 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::TexCoord  , SurfaceFormat::R32G32_Float      , 0, 0, InputType::PerVertex, 0 }
};

//-----------------------------------------------------------------------------
const InputLayout VertexSkimmedMesh::inputLayout =
{
	{ VertexAttribute::Position		, SurfaceFormat::R32G32B32_Float   , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Normal		, SurfaceFormat::R32G32B32_Float   , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Tangent		, SurfaceFormat::R32G32B32_Float   , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::Color		, SurfaceFormat::R32G32B32A32_Float, 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::TexCoord     , SurfaceFormat::R32G32_Float      , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::BlendIndices , SurfaceFormat::R32G32B32A32_Uint , 0, 0, InputType::PerVertex, 0 },
	{ VertexAttribute::BlendWeight  , SurfaceFormat::R32G32B32_Float   , 0, 0, InputType::PerVertex, 0 },
};