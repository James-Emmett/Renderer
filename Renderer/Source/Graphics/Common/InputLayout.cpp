#include "Graphics/Common/InputLayout.h"

std::string VertexAttributeLUT[] =
{
	"POSITION",
	"NORMAL",
	"TANGENT",
	"COLOR",
	"TEXCOORD",
	"BlendWeight",
	"BlendIndicies"
};

std::string VertexAttributeToString(VertexAttribute attribute)
{
	return VertexAttributeLUT[(u32)attribute];
}
