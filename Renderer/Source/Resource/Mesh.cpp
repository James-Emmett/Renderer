#include "Resource/Mesh.h"
#include "System/Assert.h"
#include <System/Logger.h>
#include "tinyobj/tiny_obj_loader.h"
#include "System/Hash32.h"
#include <unordered_map>

Mesh::Mesh()
{

}

Mesh::Mesh(Mesh&& mesh) noexcept
{
	m_VertexBuffer = std::move(mesh.m_VertexBuffer);
	m_IndexBuffer = std::move(mesh.m_IndexBuffer);
	m_MeshParts = std::move(mesh.m_MeshParts);
	m_Indicies = std::move(mesh.m_Indicies);
	m_Vertices = std::move(mesh.m_Vertices);
	m_Normals = std::move(mesh.m_Normals);
	m_Tangent = std::move(mesh.m_Tangent);
	m_Colors = std::move(mesh.m_Colors);
	m_TexCords = std::move(mesh.m_TexCords);
	m_PackedMesh = std::move(mesh.m_PackedMesh);
	m_VertexCount = mesh.m_VertexCount;
	m_IndexCount = mesh.m_IndexCount;
	m_IsReadable = mesh.m_IsReadable;
	m_IsDirty = mesh.m_IsDirty;
}

Mesh::~Mesh()
{
	Dispose();
}

void Mesh::operator=(Mesh&& mesh) noexcept
{
	m_VertexBuffer = std::move(mesh.m_VertexBuffer);
	m_IndexBuffer = std::move(mesh.m_IndexBuffer);
	m_MeshParts = std::move(mesh.m_MeshParts);
	m_Indicies = std::move(mesh.m_Indicies);
	m_Vertices = std::move(mesh.m_Vertices);
	m_Normals = std::move(mesh.m_Normals);
	m_Tangent = std::move(mesh.m_Tangent);
	m_Colors = std::move(mesh.m_Colors);
	m_TexCords = std::move(mesh.m_TexCords);
	m_PackedMesh = std::move(mesh.m_PackedMesh);
	m_VertexCount = mesh.m_VertexCount;
	m_IndexCount = mesh.m_IndexCount;
	m_IsReadable = mesh.m_IsReadable;
	m_IsDirty = mesh.m_IsDirty;
}

void Mesh::SetVertexData(Byte* data, u32 byteCount, u32 dataStart, u32 vertexStart)
{
	assert(data && "Vertex Data Was Null");

	m_VertexCount += byteCount;
	m_PackedMesh.resize(m_VertexCount);
	std::memcpy(m_PackedMesh.data() + vertexStart, data + dataStart, byteCount);
	m_IsDirty = true;
	m_IsPacked = true;
}

void Mesh::SetIndexData(Byte* data, u32 byteCount, u32 dataStart, u32 indexStart)
{
	assert(data && "Index Data Was Null");

	m_IndexCount += byteCount;
	m_Indicies.resize(m_IndexCount);
	std::memcpy(m_Indicies.data() + indexStart, data + dataStart, byteCount);
	m_IsDirty = true;
}

void Mesh::SetIndexFormat(IndexFormat format)
{
	m_IndexFormat = format;
}

void Mesh::SetVertexFormat(VertexFormat format)
{
	m_VertexFormat = format;
}

void Mesh::SetVertices(Vector3* data, u32 count)
{
	assert(data && "Vertex Data Was Null");
	m_VertexCount = count;
	m_Vertices.assign(&data[0], &data[count]);
	m_IsDirty = true;
}

void Mesh::SetNormals(Vector3* data, u32 count)
{
	assert(data && "Normal Data Was Null");
	m_Normals.assign(&data[0], &data[count]);
	m_IsDirty = true;
}

void Mesh::SetTangent(Vector4* data, u32 count)
{
	assert(data && "Tangent Data Was Null");
	m_Tangent.assign(&data[0], &data[count]);
	m_IsDirty = true;
}

void Mesh::SetColors(Color* data, u32 count)
{
	assert(data && "Colour Data Was Null");
	m_Colors.assign(&data[0], &data[count]);
	m_IsDirty = true;
}

void Mesh::SetUV(Vector2* data, u32 count)
{
	assert(data && "UV's Data Was Null");
	m_TexCords.assign(&data[0], &data[count]);
	m_IsDirty = true;
}

void Mesh::SetMeshPart(u32 part, u32 indexCount, u32 indexStart)
{
	if (m_MeshParts.empty() || part >= (u32)m_MeshParts.size())
	{
		m_MeshParts.resize((u64)part + 1);
	}

	m_MeshParts[part].m_Count = indexCount;
	m_MeshParts[part].m_Start = indexStart;
	m_IsDirty = true;
}

bool Mesh::GetVertices(std::vector<Vector3>& vertices)const
{
	if (!m_IsReadable || m_Vertices.empty())
	{
		LogError("Vertex data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	vertices = m_Vertices;
	return true;
}

bool Mesh::GetNormals(std::vector<Vector3>& normals)const
{
	if (!m_IsReadable || m_Normals.empty())
	{
		LogError("Normals data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	normals = m_Normals;
	return true;
}

bool Mesh::GetTangent(std::vector<Vector4>& tangents)const
{
	if (!m_IsReadable || m_Tangent.empty())
	{
		LogError("Tangent data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	tangents = m_Tangent;
	return true;
}

bool Mesh::GetColors(std::vector<Color>& colors)const
{
	if (!m_IsReadable || m_Colors.empty())
	{
		LogError("Color data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	colors = m_Colors;
	return true;
}

bool Mesh::GetUV(std::vector<Vector2>& uv)const
{
	if (!m_IsReadable || m_TexCords.empty())
	{
		LogError("Texcoords data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	uv = m_TexCords;
	return true;
}

bool Mesh::GetIndices(std::vector<u32>& indicies, u32 submesh)const
{
	if (!m_IsReadable || m_Indicies.empty())
	{
		LogError("Tangent data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	indicies = m_Indicies;
	return true;
}

MeshPart Mesh::GetMeshPart(u32 part)const
{
	if (!m_IsReadable || m_MeshParts.empty())
	{
		LogError("MeshParts data missing or unaccessible");
		assert(false);
		return false;
	}

	// Copy, ouch
	return m_MeshParts[part];
}

void Mesh::ClearCpuData()
{
	m_Vertices.clear();
	m_Indicies.clear();
	m_Normals.clear();
	m_Tangent.clear();
	m_Colors.clear();
	m_TexCords.clear();
	m_PackedMesh.clear();
}

// I disagree with how this works, but support it anyway.
void Mesh::PackMesh()
{
	if (m_IsDirty && !m_IsPacked && m_Vertices.size() >= 0)
	{
		m_PackedMesh.clear();
		m_PackedMesh.resize(m_VertexCount * (u32)VertexFormat::VertexMesh);

		// Recompute normals if required
		if (m_Normals.size() < m_VertexCount)
		{
			m_Normals.clear();
			RecalculateNormals();
		}

		// Recalculate Tangents
		if (m_Tangent.size() < m_VertexCount)
		{
			m_Tangent.clear();
			RecalculateTangents();
		}

		// Add default colors
		if (m_Colors.size() < m_VertexCount)
		{
			m_Colors.clear();
			m_Colors.resize(m_VertexCount);
			for (size_t i = 0; i < m_VertexCount; ++i)
			{
				m_Colors[i] = Color(0, 0, 0, 1);
			}
		}

		// Add some unit UV data
		if (m_TexCords.size() < m_VertexCount)
		{
			m_TexCords.clear();
			m_TexCords.resize(m_VertexCount);
			for (size_t i = 0; i < m_VertexCount; ++i)
			{
				m_TexCords[i] = Vector2(0, 0);
			}
		}

		u64 byteCount = 0;
		// Pack the mesh
		for (size_t i = 0; i < m_VertexCount; ++i)
		{
			VertexMesh mesh;
			mesh.m_Position = m_Vertices[i];
			mesh.m_Normal = m_Normals[i];
			mesh.m_Tangent = m_Tangent[i];
			mesh.m_Color = m_Colors[i];
			mesh.m_Texture = m_TexCords[i];

			// copy into packed mesh data
			std::memcpy(&m_PackedMesh[byteCount], &mesh, (u32)VertexFormat::VertexMesh);
			byteCount += (u32)VertexFormat::VertexMesh;
		}

		m_IsPacked = true;
		m_VertexFormat = VertexFormat::VertexMesh;
	}
}

void Mesh::RecalculateNormals()
{
	if (m_Vertices.empty())
	{
		LogError("Cannot Generate Normals when vertices are null.");
		return;
	}

	std::vector<Vector3> normals(m_VertexCount, Vector3(0, 0, 0));
	for (MeshPart mesh : m_MeshParts)
	{
		for (u32 i = 0; i < mesh.m_Count; i += 3)
		{
			u64 index = mesh.m_Start + i;

			//--Indices--
			u32 id0 = m_Indicies[index];
			u32 id1 = m_Indicies[(size_t)index + 1];
			u32 id2 = m_Indicies[(size_t)index + 2];

			// Triangle
			Vector3 p0 = m_Vertices[id0];
			Vector3 p1 = m_Vertices[id1];
			Vector3 p2 = m_Vertices[id2];

			Vector3 E1 = p1 - p0;
			Vector3 E2 = p2 - p0;

			// Calculate face normal
			Vector3 n = Vector3::Cross(E1, E2);

			// Get angles between edges
			float A = Vector3::Angle(p1 - p0, p2 - p0);
			float B = Vector3::Angle(p2 - p1, p0 - p1);
			float C = Vector3::Angle(p0 - p2, p1 - p2);

			// Accumulate normals with weights.
			normals[id0] += n * A;
			normals[id1] += n * B;
			normals[id2] += n * C;
		}
	}

	m_Normals.clear();
	m_Normals.resize(m_VertexCount, Vector3());

	for (u32 i = 0; i < m_VertexCount; i++)
	{
		m_Normals[i] = Vector3::Normalize(normals[i]);
	}

	m_IsDirty = true;
}

void Mesh::RecalculateTangents()
{
	if (m_Vertices.empty())
	{
		LogError("Cannot Generate Tangents when vertices are null.");
		return;
	}

	std::vector<Vector3> tangents(m_VertexCount, Vector3());
	std::vector<Vector3> bitangents(m_VertexCount, Vector3());

	// Loop each submesh, and each submehses triangles!
	for (MeshPart mesh : m_MeshParts)
	{
		for (u32 i = 0; i < mesh.m_Count; i += 3)
		{
			u64 index = mesh.m_Start + i;

			//--Indices--
			u32 i0 = m_Indicies[index];
			u32 i1 = m_Indicies[(size_t)index + 1];
			u32 i2 = m_Indicies[(size_t)index + 2];

			// Triangle
			Vector3& p0 = m_Vertices[i0];
			Vector3& p1 = m_Vertices[i1];
			Vector3& p2 = m_Vertices[i2];

			//--Uv Edges--
			Vector2 uv1 = m_TexCords[i1] - m_TexCords[i0];
			Vector2 uv2 = m_TexCords[i2] - m_TexCords[i0];

			//--Triangle Edges--
			Vector3 E1 = p1 - p0;
			Vector3 E2 = p2 - p0;

			//--Formula--
			float r = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
			Vector3 t = (E1 * uv2.y - E2 * uv1.y) * r;
			Vector3 b = (E2 * uv1.x - E1 * uv2.x) * r;

			// Accumulate for smoothing between shared vertexs
			tangents[i0] += t;
			tangents[i1] += t;
			tangents[i2] += t;

			bitangents[i0] += b;
			bitangents[i1] += b;
			bitangents[i2] += b;
		}
	}

	m_Tangent.clear();
	m_Tangent.resize(m_VertexCount, Vector4());
	for (u32 i = 0; i < m_VertexCount; i++)
	{
		Vector3 t = Vector3::Normalize(tangents[i]);
		Vector3 b = Vector3::Normalize(bitangents[i]);
		Vector3 n = Vector3::Normalize(m_Normals[i]);

		// Ortho-normalize tangent
		m_Tangent[i] = Vector3::Normalize(t - (n * Vector3::Dot(t, n)));

		// Orthogonal bitangent checked agaisnt bitangent for direction.
		m_Tangent[i].w = (Vector3::Dot(Vector3::Cross(t, b), n) > 0.0f) ? 1.0f : -1.0f;
	}

	m_IsDirty = true;
}

void Mesh::Upload(bool markNoLongerReadable, GraphicsDevice* device, CommandList cmd)
{
	if (m_VertexCount == 0) { LogError("No vertices to set."); return; }
	if (m_Indicies.empty()) { LogError("No Indices to set."); return; }
	if (m_IsReadable == false) { LogError("Cannot update non readable buffer"); return; }
	if (m_IsDirty == false) { return; }

	// Set Mesh Part to be entire mesh
	if (m_MeshParts.empty())
	{
		m_MeshParts.emplace_back(0, m_IndexCount);
	}

	// Pack if it needs packing
	PackMesh();

	// Create Buffers
	if (m_VertexBuffer == nullptr)
	{
		m_VertexBuffer = GraphicsResource::CreateBuffer(device, HeapType::Default, m_VertexCount, (u32)m_VertexFormat, 0);
	}

	m_VertexBuffer->SetData(m_PackedMesh.data(), (u32)m_VertexFormat * m_VertexCount, 0, cmd);

	u32 stride = m_IndexFormat == IndexFormat::I16 ? 2 : 4;
	// Create IndexBuffer
	if (m_IndexBuffer == nullptr)
	{
		m_IndexBuffer = GraphicsResource::CreateBuffer(device, HeapType::Default, m_IndexCount, stride, 0);
	}

	m_IndexBuffer->SetData((byte*)m_Indicies.data(), m_Indicies.size() * stride, 0, cmd);

	if (markNoLongerReadable)
	{
		m_IsReadable = false;
		ClearCpuData();
	}
}

void Mesh::Dispose()
{
	if (m_VertexBuffer)
	{
		m_VertexBuffer.reset();
		m_VertexBuffer = nullptr;
	}

	if (m_IndexBuffer)
	{
		m_IndexBuffer.reset();
		m_IndexBuffer = nullptr;
	}
}

u32 Mesh::VertexCount()const
{
	return m_VertexCount;
}

u32 Mesh::IndexCount()const
{
	return m_IndexCount;
}

u32 Mesh::MeshPartCount()const
{
	return (u32)m_MeshParts.size();
}

std::shared_ptr<GraphicsResource> Mesh::VertexBuffer()
{
	return m_VertexBuffer;
}

std::shared_ptr<GraphicsResource> Mesh::IndexBuffer()
{
	return m_IndexBuffer;
}


Mesh Mesh::LoadFromFile(const std::string& fileName)
{
	//--Get Extension--
	std::string ext = fileName.c_str();
	ext = ext.substr(ext.find_last_of(".") + 1);
	for (int i = 0; i < 3; i++)
	{
		ext[i] = tolower(ext[i]);
	}

	if (ext == "obj")
	{
		return LoadFromObj(fileName);
	}
	else if (ext == "mesh")
	{
		// RE ADD SUPPORT FOR CUSTOM FORMAT
		//BinaryFile file;
		//if (file.Open(fileName.c_str(), FileMode::Read))
		//{
		//	assert(file.ReadDword() == MESH_MAGIC && "Failed To Read Mesh.");
		//	u32 vertexCount = file.ReadDword();
		//	u32 indexCount = file.ReadDword();
		//	VertexMesh* vertexData = new VertexMesh[vertexCount];
		//	u32* indexData = new u32[indexCount];
		//
		//	if (!file.ReadBuffer((Byte*)vertexData, (u32)VertexFormat::VertexMesh * vertexCount))
		//	{
		//		assert(0 && "Failed To Load Mesh.");
		//	}
		//
		//	if (!file.ReadBuffer((Byte*)indexData, sizeof(u32) * indexCount))
		//	{
		//		assert(0 && "Failed To Load Mesh.");
		//	}
		//
		//	//--Read Submeshes--
		//	u32 subMeshCount = file.ReadDword();
		//	for (u32 i = 0; i < subMeshCount; ++i)
		//	{
		//		u32 indexCount = file.ReadDword();
		//		u32 indexOffset = file.ReadDword();
		//		SetSubMesh((u32)i, indexCount, indexOffset);
		//	}
		//
		//	SetVertexData(vertexData, vertexCount);
		//	SetIndices(indexData, indexCount);
		//	m_IsDirty = true;
		//	m_IsReadable = true;
		//	m_IsPacked = true;
		//	m_FilePath = fileName.c_str();
		//	m_Name = fileName.c_str();
		//	Apply(); // Submit to GPU
		//
		//	//--Clean data--
		//	delete[] vertexData;
		//	delete[] indexData;
		//	return;
		//}
	}

	assert(0 && "Failed To Load Mesh.");
	return Mesh();
}

Mesh Mesh::LoadFromObj(const std::string& filePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filePath.c_str());

	if (!warning.empty())
	{
		LogWarning(warning);
	}

	if (!error.empty())
	{
		LogWarning(error);
	}

	if (!result)
	{
		assert(0 && "Failed To Load Mesh.");
	}

	if (attrib.texcoords.empty())
	{
		attrib.texcoords.resize(attrib.vertices.size());
	}

	if (attrib.colors.empty())
	{
		attrib.colors.resize(attrib.vertices.size());
	}

	if (attrib.normals.empty())
	{
		attrib.normals.resize(attrib.vertices.size());
	}

	Mesh mesh;

	u32 index_offset = 0;
	// Could hash instead?
	std::unordered_map<u64, u64> indexMap = {};

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Set submesh
		mesh.SetMeshPart((u32)s, (u32)shapes[s].mesh.indices.size(), index_offset);
		for (u32 i = 0; i < (u32)shapes[s].mesh.indices.size(); i++)
		{
			// access to vertex
			tinyobj::index_t idx = shapes[s].mesh.indices[i];

			if (idx.texcoord_index < 0)
			{
				idx.texcoord_index = 0;
			}

			if (idx.normal_index < 0)
			{
				idx.normal_index = 0;
			}

			// We could just store packed, but we would end up repacking again anyways?
			VertexMesh vertex;
			vertex.m_Position	= Vector3(attrib.vertices[(u64)3 * idx.vertex_index + 0], attrib.vertices[(u64)3 * idx.vertex_index + 1], attrib.vertices[(u64)3 * idx.vertex_index + 2]);
			vertex.m_Normal		= Vector3(attrib.normals[(u64)3 * idx.normal_index + 0], attrib.normals[(u64)3 * idx.normal_index + 1], attrib.normals[(u64)3 * idx.normal_index + 2]);
			vertex.m_Texture	= Vector2(attrib.texcoords[(u64)2 * idx.texcoord_index + 0], 1.0f - attrib.texcoords[(u64)2 * idx.texcoord_index + 1]);
			vertex.m_Color		= Color(attrib.colors[(u64)3 * idx.vertex_index + 0], attrib.colors[(u64)3 * idx.vertex_index + 1], attrib.colors[(u64)3 * idx.vertex_index + 2]);

			// Tangent is 0,0,0 but should still be okay
			u32 hash = Hash32::ComputeHash((Byte*)&vertex, (u64)VertexFormat::VertexMesh);

			// Check if this vertex is unique
			std::unordered_map<u64, u64>::iterator itr = indexMap.find(hash);
			u64 index = (int)mesh.m_Vertices.size();
			if (itr == indexMap.end())
			{
				// Push back the data and assign the index
				mesh.m_Vertices.push_back(vertex.m_Position);
				mesh.m_Normals.push_back(vertex.m_Normal);
				mesh.m_TexCords.push_back(vertex.m_Texture);
				mesh.m_Colors.push_back(vertex.m_Color);
				indexMap[hash] = index;
			}
			else
			{
				index = itr->second;
			}

			mesh.m_Indicies.push_back(index);
		}

		index_offset += (u32)shapes[s].mesh.indices.size();
	}

	//--Set counts--
	mesh.m_VertexCount = (u32)mesh.m_Vertices.size();
	mesh.m_IndexCount = index_offset;
	mesh.m_IsDirty = true;
	mesh.m_IsReadable = true;
	mesh.m_IsPacked = false; // Urgg had to change this
	mesh.m_FilePath = filePath.c_str();
	mesh.m_Name = filePath.c_str();
	mesh.RecalculateTangents();

	return mesh;
}