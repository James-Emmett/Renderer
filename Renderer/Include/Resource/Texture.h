#pragma once
//Note::
/*
	Replace with slim down GPU only version, use ContentLoaders too load
	different supported formats and return a TextureData. Use specific managers like
	TextureManager too support this and have a register loader function.

	Will leave alone for now!
*/

#pragma once
#include "Graphics/GraphicsDevice.h"
#include "Resource.h"
#include <vector>

// Meta data for mips
struct TextureLevel
{
	Byte* ptr = nullptr;
	u32 width = 0;
	u32 height = 0;
	u32 depth = 0;
	u32 byteCount = 0;
};

class GraphicsDevice;
class Texture : public Resource
{
protected:
	GraphicsDevice* m_GraphicsDevice = nullptr;

	//--Gpu Memory--
	std::shared_ptr<TextureResource> m_Texture;

	//--CPU Data--
	u32		m_ByteCount = 0;
	Byte*	m_Data = nullptr;
	std::vector<TextureLevel> m_LookUpTable;

public:
	Texture();
	Texture(const Texture& texture) = delete;
	Texture(Texture&& texture) noexcept;
	~Texture();

	void CreateTexture(GraphicsDevice* device, ResourceDesc& desc);

public:
	void operator=(const Texture& texture) = delete;
	void operator=(Texture&& texture) noexcept;

public:
	u32					GetWidth()const;
	u32					GetHeight()const;
	u32					GetDepth()const;
	u32					GetMipCount()const;
	u32					GetRowPitch()const;
	SurfaceFormat		GetFormat()const;
	ResourceDimension	GetTextureType()const;
	bool				IsDisposed()const;
	bool				IsCube()const;
	bool				IsSRGB()const;
	bool				HasMips()const;
	u32					GetByteCount()const;
	Byte*				GetData()const;
	void				GetGPUData(Byte* data, u32 byteCount);
	void				SetData(Byte* data);
	Byte*				GetSurfaceData(u32 mipLevel = 0, u32 arrayLevel = 0)const;
	void				SetSurfaceData(Byte* data, u32 mipLevel = 0, u32 arrayLevel = 0);
	Byte*				GetSurfaceArrayData(u32 arrayLevel = 0)const;
	void				SetSurfaceArrayData(Byte* data, u32 arrayLevel = 0);
	Color				GetPixel(u32 x, u32 y, u32 z = 1, u32 depthOrArray = 1, u32 mip = 1);
	void				SetPixel(Color& color, u32 x, u32 y, u32 depthOrArray = 1, u32 mip = 1);
	void				ClearCPUData();

	// Submits data to low-level GPU interface if required.
	void Upload(CommandList cmd, bool clearCPU = true);
	static std::shared_ptr<Texture> LoadFromFile(const std::string& fileName);
	void Release();

	std::shared_ptr<TextureResource>	GetTextureResource()const;
	ResourceDesc						GetTextureInfo()const;

protected:
	static  std::shared_ptr<Texture> LoadFromSource(std::string fileName);
	void GenerateLookUpTable();

};