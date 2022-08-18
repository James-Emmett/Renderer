#pragma once
#include "Resource.h"
#include "Graphics/GraphicsDevice.h"

#define MESH_MAGIC 2646

// Part of a mesh, i.e submesh should go it's own material
// in the renderer component
struct MeshPart
{
	u64 m_Start = 0;
	u64 m_Count = 0;

	MeshPart(u64 start = 0, u64 count = 0) :
		m_Start(start), m_Count(count)
	{}
};

class Mesh : Resource
{
protected:
	std::shared_ptr<GraphicsResource>	m_VertexBuffer;
	std::shared_ptr<GraphicsResource>	m_IndexBuffer;
	std::vector<MeshPart>				m_MeshParts;
	std::vector<Vector3>				m_Vertices;
	std::vector<Vector3>				m_Normals;
	std::vector<Vector4>				m_Tangent;
	std::vector<Color>					m_Colors;
	std::vector<Vector2>				m_TexCords;
	std::vector<u32>					m_Indicies;
	std::vector<Byte>					m_PackedMesh;

	u32									m_VertexCount = 0;
	u32									m_IndexCount = 0;
	IndexFormat							m_IndexFormat;
	VertexFormat						m_VertexFormat;
	bool								m_IsReadable = false;
	bool								m_IsDirty = false;
	bool								m_IsPacked = false;

public:
	Mesh();
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	void operator=(const Mesh& mesh) = delete;
	void operator=(Mesh&& mesh)noexcept;

public:
	void SetVertexData(Byte* data, u32 byteCount, u32 dataStart = 0, u32 vertexStart = 0);
	void SetIndexData(Byte* data, u32 byteCount, u32 dataStart = 0, u32 indexStart = 0);
	void SetIndexFormat(IndexFormat format);
	void SetVertexFormat(VertexFormat format);

	void SetVertices(Vector3* data, u32 count);
	void SetNormals(Vector3* data, u32 count);
	void SetTangent(Vector4* data, u32 count);
	void SetColors(Color* data, u32 count);
	void SetUV(Vector2* data, u32 count);
	void SetMeshPart(u32 part, u32 indexCount, u32 indexStart);

	bool GetVertices(std::vector<Vector3>& vertices)const;
	bool GetNormals(std::vector<Vector3>& normals)const;
	bool GetTangent(std::vector<Vector4>& tangents)const;
	bool GetColors(std::vector<Color>& colors)const;
	bool GetUV(std::vector<Vector2>& uv)const;
	bool GetIndices(std::vector<u32>& indicies, u32 submesh)const;
	MeshPart GetMeshPart(u32 part)const;
	void ClearCpuData();

	//--Expensive Functions Avoid at Runtime--
	void PackMesh();
	void RecalculateNormals();
	void RecalculateTangents();
	void Upload(bool markNoLongerReadable, GraphicsDevice* device, CommandList cmd = 0);

	//--Counts--
	u32 VertexCount()const;
	u32 IndexCount()const;
	u32 MeshPartCount()const;

	//--Internal--
	std::shared_ptr<GraphicsResource> VertexBuffer();
	std::shared_ptr<GraphicsResource> IndexBuffer();

	void Dispose();

	static Mesh LoadFromFile(const std::string& filePath);

private:
	static Mesh LoadFromObj(const std::string& filePath);
};