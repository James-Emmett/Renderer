#pragma once
#include "Resource/Texture.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Common/CommonStates.h"
#include "System/Logger.h"
#include "System/Window.h"
#include "FileSystem/Path.h"
#include "Math/Mathf.h"
#include "Engine/Application.h"
#include "System/Assert.h"
#include "Engine/Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Texture::Texture() : Resource()
{
}

Texture::Texture(Texture&& texture)noexcept : Resource(std::move(texture))
{
	// steal
	m_GraphicsDevice = texture.m_GraphicsDevice;
	m_Texture = texture.m_Texture;

	m_Texture = texture.m_Texture;
	m_Data = texture.m_Data;
	m_LookUpTable = std::move(texture.m_LookUpTable);

	// Clear other.
	texture.m_GraphicsDevice = nullptr;
	texture.m_Data = nullptr;
	texture.m_Texture = nullptr;
}

Texture::~Texture()
{
	if (m_Data)
	{
		delete[] m_Data;
		m_Data = nullptr;
		m_LookUpTable.clear();
	}

	if (m_GraphicsDevice)
	{
		if (m_Texture)
		{
			m_Texture->Release();
		}
	}
}

void Texture::CreateTexture(GraphicsDevice* device, ResourceDesc& desc)
{
	m_GraphicsDevice = device;
	//--Calculate cpu buffer size--
	m_ByteCount = TextureHelper::CalculateTotalBytes(desc.Format, desc.Width, desc.Height, desc.DepthOrArraySize, desc.MipCount);
	m_Data = new Byte[m_ByteCount];
	m_Texture = std::make_shared<TextureResource>(m_GraphicsDevice, desc);
	GenerateLookUpTable();
}

void Texture::GetGPUData(Byte* data, u32 byteCount)
{
	//m_GraphicsDevice->GetTextureData(m_TextureHandle, data, byteCount);
}

void Texture::SetData(Byte* data)
{
	if (m_ByteCount == 0) { LogError("No Bytes to set."); return; }

	if (!m_Data)
	{
		m_Data = new Byte[m_ByteCount];
	}

	memcpy(m_Data, data, m_ByteCount);
}

void Texture::operator=(Texture&& texture) noexcept
{
	// Copy
	m_GraphicsDevice = texture.m_GraphicsDevice;
	m_Texture = m_Texture;
	m_Data = texture.m_Data;
	m_LookUpTable = std::move(texture.m_LookUpTable);

	// Clear other.
	texture.m_GraphicsDevice = nullptr;
	texture.m_Data = nullptr;
}

u32 Texture::GetWidth() const
{
	return m_Texture->Width();
}

u32 Texture::GetHeight() const
{
	return m_Texture->Height();
}

u32 Texture::GetDepth() const
{
	return m_Texture->Depth();
}

u32 Texture::GetMipCount() const
{
	return m_Texture->MipCount();
}

u32 Texture::GetRowPitch() const
{
	return m_Texture->Stride();
}

SurfaceFormat Texture::GetFormat() const
{
	return m_Texture->Format();
}

ResourceDimension Texture::GetTextureType() const
{
	return m_Texture->Dimension();
}

bool Texture::IsDisposed() const
{
	return m_GraphicsDevice == nullptr && m_Texture == nullptr;
}

bool Texture::IsCube() const
{
	return GetTextureType() == ResourceDimension::TextureCube;
}

bool Texture::IsSRGB() const
{
	return TextureHelper::IsSRGBFormat(m_Texture->Format());
}

bool Texture::HasMips() const
{
	return m_Texture->MipCount() > 1;
}

u32 Texture::GetByteCount()const
{
	return m_ByteCount;
}

Byte* Texture::GetData()const
{
	return m_Data;
}

void Texture::SetSurfaceData(Byte* data, u32 mipLevel, u32 arrayLevel)
{
	if (m_ByteCount == 0 && m_Texture == nullptr) { LogWarning("No Bytes to set or invlaud TextureResource"); return; }
	if (mipLevel > m_Texture->MipCount()) { LogWarning("MipLevel is too high."); return; }
	if (arrayLevel > m_Texture->Depth()) { LogWarning("ArrayLevel is too high."); return; }

	TextureLevel* surface = &m_LookUpTable[(arrayLevel * m_Texture->MipCount()) + mipLevel];
	memcpy(surface->ptr, data, surface->byteCount);

}

Byte* Texture::GetSurfaceData(u32 mipLevel, u32 arrayLevel)const
{
	if (mipLevel > m_Texture->MipCount()) { LogError("MipLevel is too high."); return nullptr; }
	if (arrayLevel > m_Texture->Depth()) { LogError("ArrayLevel is too high."); return nullptr; }
	const TextureLevel* surface = &m_LookUpTable[(arrayLevel * m_Texture->MipCount()) + mipLevel];
	return surface->ptr;
}

Byte* Texture::GetSurfaceArrayData(u32 arrayLevel) const
{
	if (m_ByteCount == 0 && m_Texture == nullptr) { LogWarning("No Bytes to set."); return nullptr; }
	if (arrayLevel > m_Texture->Depth()) { LogWarning("ArrayLevel is too high."); return nullptr; }
	const TextureLevel* surface = &m_LookUpTable[(arrayLevel * m_Texture->MipCount())];
	return surface->ptr;
}

void Texture::SetSurfaceArrayData(Byte* data, u32 arrayLevel)
{
	if (m_ByteCount == 0 && m_Texture == nullptr) { LogWarning("No Bytes to set."); return; }
	if (arrayLevel > m_Texture->Depth()) { LogWarning("ArrayLevel is too high."); return; }

	// How many bytes to copy over
	u32 byteCount = TextureHelper::CalculateTotalBytes(m_Texture->Format(), m_Texture->Width(), m_Texture->Height(), m_Texture->Depth(), m_Texture->MipCount());
	// The start location to overwrite from
	TextureLevel* surface = &m_LookUpTable[(arrayLevel * m_Texture->MipCount())];
	memcpy(surface->ptr, data, byteCount);
}

Color Texture::GetPixel(u32 x, u32 y, u32 z, u32 array, u32 mip)
{
	u32 index = ((y * m_Texture->Width()) + x) * z * TextureHelper::BytesPerBlock(m_Texture->Format());
	float R = 1.0f / 255;

	Color color;
	color.r = m_Data[index] * R;
	color.g = m_Data[index + 1] * R;
	color.b = m_Data[index + 2] * R;
	color.a = m_Data[index + 3] * R;

	return color;
}

void Texture::SetPixel(Color& color, u32 x, u32 y, u32 z, u32 mip)
{
	u32 index = ((y * m_Texture->Width()) + x) * z * TextureHelper::BytesPerBlock(m_Texture->Format());

	m_Data[index] = (Byte)(color.r * 255);
	m_Data[index + sizeof(Byte)] = (Byte)(color.g * 255);
	m_Data[index + (sizeof(Byte) * 2)] = (Byte)(color.b * 255);
	m_Data[index + (sizeof(Byte) * 3)] = (Byte)(color.a * 255);
}

void Texture::ClearCPUData()
{
	if (m_Data)
	{
		delete[] m_Data;
		m_LookUpTable.clear();
	}
}

void Texture::Upload(CommandList cmd, bool clearCPU)
{
	if (m_GraphicsDevice == nullptr) { return; }
	if (m_ByteCount == 0) { LogWarning("No Bytes to set."); return; }
	if (m_Texture == nullptr) { LogWarning("Cannot update null Texture."); return; }

	m_Texture->SetData(m_Data, m_ByteCount, 0, cmd);

	if (clearCPU)
	{
		delete[] m_Data;
		m_Data = nullptr;
	}
}

std::shared_ptr<Texture> Texture::LoadFromFile(const std::string& fileName)
{
	std::shared_ptr<Texture> texture = nullptr;

	//--Get Extension--
	std::string ext = fileName.c_str();
	ext = ext.substr(ext.find_last_of(".") + 1);
	for (int i = 0; i < ext.size(); ++i)
	{
		ext[i] = tolower(ext[i]);
	}

	std::string filePath = fileName;
	if (ext != "texture")
	{
		std::string filePath = fileName;
		filePath = filePath.substr(0, filePath.find_last_of("."));
		filePath += ".texture";
		
		if (Path::FileExists(filePath))
		{
			//LoadFromBinary(filePath); Re-Add This back in
		}
		else
		{
			if (ext == "jpg" || ext == "png" || ext == "bmp" || ext == "tga")
			{
				texture = LoadFromSource(fileName);
			}

			filePath = fileName;
		}
	}
	else if (ext == "texture")
	{
		// LoadFromBinary(filePath); Re-Add This back in
	}

	if (texture)
	{
		texture->SetPath(filePath);
	}

	return texture;
}

void Texture::Release()
{
	if (m_Data != nullptr)
	{
		delete[] m_Data;
		m_Data = nullptr;
		m_LookUpTable.clear();
	}

	if (m_Texture)
	{
		m_Texture->Release();
	}
}

ResourceDesc Texture::GetTextureInfo() const
{
	return m_Texture->GetResourceDesc();
}

std::shared_ptr<TextureResource> Texture::GetTextureResource() const
{
	return m_Texture;
}

std::shared_ptr<Texture> Texture::LoadFromSource(std::string fileName)
{
	std::shared_ptr<Texture> texture = nullptr;
	int width = 0;
	int height = 0;
	int comp = 0;

	unsigned char* p = stbi_load(fileName.c_str(), &width, &height, &comp, STBI_rgb_alpha);
	ResourceDesc desc;
	if (p != nullptr)
	{
		if (width != 0 && height != 0)
		{
			// i mean loading a source is already slow anyway, so do a check for non SRGB and covnert.
			desc.Format = SurfaceFormat::R8G8B8A8_Unorm_SRGB;

			size_t found = fileName.find_last_of("_");
			if (found != std::string::npos)
			{
				std::string type = fileName.substr(found + 1, fileName.find_last_of(".") - (found + 1));

				if (type == "Normal" || type == "Roughness" || "type" == "Metalness")
				{
					desc.Format = SurfaceFormat::R8G8B8A8_Unorm;
				}
			}

			desc.Width = width;
			desc.Height = height;
			desc.DepthOrArraySize = 1;
			desc.MipCount = 1;
			desc.Stride = TextureHelper::PitchSize(desc.Format, width);
			desc.Dimension = ResourceDimension::Texture2D;
			desc.Flags = (u32)BindFlag::ShaderResource;

			texture = std::make_shared<Texture>();
			texture->CreateTexture(Application::GEngine->Device(), desc);
			texture->SetData(p);
		}

		stbi_image_free(p);
	}
	else
	{
		assert(0 && "Failed to load texture.");
	}

	return texture;
}

void Texture::GenerateLookUpTable()
{
	u32 w;
	u32 h;
	u32 d;

	u32 arraySize	= m_Texture->Depth();
	u32 mipCount	= m_Texture->MipCount();
	Byte* ptr		= m_Data;
	for (u32 i = 0; i < arraySize; ++i)
	{
		w = m_Texture->Width();
		h = m_Texture->Height();
		d = m_Texture->Depth(); // For 3D texture which can have mips

		for (u32 j = 0; j < mipCount; ++j)
		{
			TextureLevel level;
			level.width = w;
			level.height = h;
			level.depth = d;
			level.ptr = ptr;
			level.byteCount = TextureHelper::CalculateSurfaceSize(m_Texture->Format(), w, h) * d;
			m_LookUpTable.push_back(level);

			ptr += level.byteCount;

			w = Mathf::Max(w >> 1u, 1u);
			h = Mathf::Max(h >> 1u, 1u);
		}
	}
}

// This works, but is probably too expensive to justify it...
inline u32 TextureOffset(const ResourceDesc& info, u32 mipLevel, u32 arraySlice, u32 byteCount)
{
	u32 pow = (u32)Mathf::Pow(4, (float)mipLevel);
	u32 A = byteCount / info.DepthOrArraySize; // Total bytes to a surface + mips
	u32 B = TextureHelper::CalculateSurfaceSize(info.Format, info.Width, info.Height); // Total bytes to top level surface
	return  (arraySlice * A) + B + (B * ((u32)Mathf::Floor((float)pow / 3) / pow)); // offset bytes
}