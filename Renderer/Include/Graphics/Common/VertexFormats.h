#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Color.h"
#include "InputLayout.h"

enum class VertexFormat
{
	Vertex = 12,
	VertexTexture = 20,
	VertexColor = 28,
	VertexColorTexture = 36,
	VertexNormal = 48,
	VertexMesh = 64,
	VertexSkimmedMesh = 92
};

struct Vertex
{
	Vector3 m_Position;
	Vertex(const Vector3& position)
	{
		m_Position = position;
	}

	bool operator==(const Vertex& vertex)const
	{
		return m_Position == vertex.m_Position;
	}

	static const InputLayout inputLayout;
};

struct VertexTexture
{
	Vector3 m_Position;
	Vector2 m_Texture;
	bool operator==(const VertexTexture& vertex)const
	{
		return m_Position == vertex.m_Position && m_Texture == vertex.m_Texture;
	}

	static const InputLayout inputLayout;
};

struct VertexColor
{
	Vector3 m_Position;
	Color   m_Color;
	bool operator==(const VertexColor& vertex)const
	{
		return m_Position == vertex.m_Position && m_Color == vertex.m_Color;
	}

	static const InputLayout inputLayout;
};


struct VertexColorTexture
{
	Vector3 m_Position;
	Color   m_Color;
	Vector2 m_Texture;
	bool operator==(const VertexColorTexture& vertex)const
	{
		return m_Position == vertex.m_Position && m_Color == vertex.m_Color && m_Texture == vertex.m_Texture;
	}

	static const InputLayout inputLayout;
};

struct VertexNormal
{
	Vector3 m_Position;
	Vector3 m_Normal;
	Color   m_Color;
	Vector2 m_Texture;

	bool operator==(const VertexNormal& vertex)const
	{
		return m_Position == vertex.m_Position && m_Normal == vertex.m_Normal &&
			m_Color == vertex.m_Color && m_Texture == vertex.m_Texture;
	}

	static const InputLayout inputLayout;
};

struct VertexMesh
{
	Vector3 m_Position;
	Vector3 m_Normal;
	Vector4 m_Tangent;
	Color   m_Color;
	Vector2 m_Texture;

	VertexMesh& operator=(const VertexMesh& mesh)
	{
		m_Position = mesh.m_Position;
		m_Normal = mesh.m_Normal;
		m_Tangent = mesh.m_Tangent;
		m_Color = mesh.m_Color;
		m_Texture = mesh.m_Texture;
		return *this;
	}

	bool operator==(const VertexMesh& vertex)const
	{
		return m_Position == vertex.m_Position && m_Normal == vertex.m_Normal &&
			m_Tangent == vertex.m_Tangent && m_Color == vertex.m_Color &&
			m_Texture == vertex.m_Texture;
	}

	static const InputLayout inputLayout;
};

struct VertexSkimmedMesh
{
	Vector3 m_Position;
	Vector3 m_Normal;
	Vector3 m_Tangent;
	Color   m_Color;
	Vector2 m_Texture;
	unsigned int m_JointIndex[4];
	Vector3 m_Weight;

	static const InputLayout inputLayout;
};