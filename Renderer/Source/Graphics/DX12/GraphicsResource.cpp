#include "Graphics/DX12/GraphicsResource.h"
#include "Graphics/DX12/GraphicsDeviceDX12.h"
#include "Math/Mathf.h"

// These just temp hold the resource sizes required for uploading a texture, use GetCopyableFootprints to fill them up.!!.
static const u64 MAX_TEXTURE_SUBRESOURCE_COUNT = D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION * 14;

GraphicsResource::GraphicsResource(GraphicsDevice* device, ResourceDesc description): m_Device(device)
{
	m_Desc = description;

	if (description.Stride > 0)
	{
		m_Count = (u32)(description.Width / description.Stride);
	}

	m_ResourceState = ResourceState::Common;

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type					= ConversionTypes::ConvertHeapType(description.Heap);
	heapProps.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask		= 1;
	heapProps.VisibleNodeMask		= 1;

	u64 alignedSize = description.Width;

	// Adjust alignment to Constant buffer blocks, 256 bytes?
	if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::ConstantBuffer))
	{
		alignedSize = Mathf::AlignSize(alignedSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	}

	D3D12_RESOURCE_DESC desc	= {};
	desc.Dimension				= (description.Dimension == ResourceDimension::TextureCube) ? D3D12_RESOURCE_DIMENSION_TEXTURE2D : 
																 ConversionTypes::ConvertResourceDimension(description.Dimension);
	desc.Alignment				= description.Alignment;
	desc.Width					= alignedSize;
	desc.Height					= description.Height;
	desc.DepthOrArraySize		= description.DepthOrArraySize;
	desc.MipLevels				= description.MipCount;
	desc.Layout					= ConversionTypes::ConvertTextureLayout(description.Layout);
	desc.SampleDesc.Count		= description.MultiSample.Count;
	desc.SampleDesc.Quality		= description.MultiSample.Quality;
	desc.Format					= ConversionTypes::ConvertFormat(description.Format);
	desc.Flags					= D3D12_RESOURCE_FLAG_NONE;

	if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::UnorderedAccess))
	{
		desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::ShaderResource) == false)
	{
		desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}

	if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::RenderTarget))
	{
		desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		m_ResourceState = ResourceState::RenderTarget;
	}
	else if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::DepthStencil))
	{
		desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		m_ResourceState = ResourceState::DepthWrite;
	}
	else if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::VertexBuffer))
	{
		m_ResourceState = ResourceState::Vertex_And_Constant_Buffer;
	}
	else if (Mathf::HasBitSet(description.Flags, (u32)BindFlag::IndexBuffer))
	{
		m_ResourceState = ResourceState::IndexBuffer;
	}

	if (description.Heap == HeapType::ReadBack)
	{
		m_ResourceState = ResourceState::CopyDest;
		desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}
	else if (description.Heap == HeapType::Upload)
	{
		m_ResourceState = ResourceState::GenericRead;
	}

	// Find a nice way to include clear flag optimisation
	m_Device->NativeDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, ConversionTypes::ConvertResourceState(m_ResourceState), nullptr, IID_PPV_ARGS(&m_Resource));
	
	m_Size = desc.Width;
	m_Stride = description.Stride;
	m_ResourceDimension = description.Dimension;
	m_HeapType = description.Heap;
	m_InternalDesc = desc;
}

GraphicsResource::GraphicsResource(GraphicsDevice* device, ID3D12Resource* resource, HeapType type, u64 offset, u64 size, bool isOwner)
{
	m_IsSuballocation = isOwner;
	m_Device = device;
	m_Resource = resource;
	m_HeapType = type;
	m_Offset = offset;
	m_Size = size;

	m_ResourceDimension = ResourceDimension::Buffer;

	m_Desc.Width = size;
	m_Desc.Heap = type;
	m_Desc.Dimension = m_ResourceDimension;
}

Byte* GraphicsResource::Map(int subresource)
{
	Byte* pData = nullptr;
	m_Resource->Map(subresource, nullptr, (void**)&pData);
	return pData + m_Offset;
}

void GraphicsResource::Unmap(int subresource)
{
	m_Resource->Unmap(subresource, nullptr);
}

// This whole function seems super costly :/
void GraphicsResource::SetData(Byte* data, u64 size, u64 offset, CommandList cmd)
{
	if (m_ResourceDimension == ResourceDimension::Buffer)
	{
		if (m_HeapType == HeapType::Default)
		{
			if (m_ResourceState != ResourceState::CopyDest)
			{
				m_Device->Transition(this, 0, m_ResourceState, ResourceState::CopyDest, cmd);
			}

			// Upload To a Ring Buffer
			std::shared_ptr<GraphicsResource> uploadBuffer = m_Device->MemoryHandler()->m_UploadBuffer.Allocate(size);
			uploadBuffer->SetData(data, size, uploadBuffer->m_Offset, cmd);
			m_Device->CopyBuffer(this, offset, uploadBuffer.get(), uploadBuffer->m_Offset, size, cmd);
			m_Device->Transition(this, 0, ResourceState::CopyDest, m_ResourceState, cmd);
		}
		else
		{
			// It's upload so directly map to memory
			Byte* destData = Map();
			std::memcpy(destData, data, (size_t)size);
			Unmap();
		}
	}
	else if (m_ResourceDimension == ResourceDimension::Texture2D)
	{
		UINT64 totalBytes = 0;
		std::vector<UINT> NumRows(MAX_TEXTURE_SUBRESOURCE_COUNT);
		std::vector<UINT64> RowSizesInBytes(MAX_TEXTURE_SUBRESOURCE_COUNT);
		std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> FootPrintLayouts(MAX_TEXTURE_SUBRESOURCE_COUNT);
		u64 numSubResources = m_Desc.MipCount * m_Desc.DepthOrArraySize;

		// this function gets the size an upload buffer needs to be to upload a texture to the gpu.
		// each row must be 256 byte aligned except for the last row, which can just be the size in bytes of the row
		// eg. textureUploadBufferSize = ((((width * numBytesPerPixel) + 255) & ~255) * (height - 1)) + (width * numBytesPerPixel);
		//textureUploadBufferSize = (((imageBytesPerRow + 255) & ~255) * (textureDesc.Height - 1)) + imageBytesPerRow;
		m_Device->NativeDevice()->GetCopyableFootprints(&m_InternalDesc, 0, numSubResources, 0, FootPrintLayouts.data(), NumRows.data(), RowSizesInBytes.data(), &totalBytes);

		// now we create an upload heap to upload our texture to the GPU
		if (m_HeapType == HeapType::Default)
		{
			if (m_ResourceState != ResourceState::CopyDest)
			{
				m_Device->Transition(this, 0, m_ResourceState, ResourceState::CopyDest, cmd);
			}

			// Upload To a Ring Buffer, make a cleaner API access than this?????????
			std::shared_ptr<GraphicsResource> uploadBuffer = m_Device->MemoryHandler()->m_UploadBuffer.AllocateTextureMemory(totalBytes);
			Byte* memory = uploadBuffer->Map();

			// Copy the texture from the upload buffer into the texture buffer

			for (u64 arrayIndex = 0; arrayIndex < m_Desc.DepthOrArraySize; arrayIndex++)
			{
				for (u64 mipIndex = 0; mipIndex < m_Desc.MipCount; mipIndex++)
				{
					u64 subResourceIndex = mipIndex + (arrayIndex * m_Desc.MipCount);

					D3D12_PLACED_SUBRESOURCE_FOOTPRINT& subResourceLayout = FootPrintLayouts[subResourceIndex];
					u64 subResourceHeight = NumRows[subResourceIndex];
					u64 subResourcePitch = Mathf::AlignSize(subResourceLayout.Footprint.RowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
					u64 subResourceDepth = subResourceLayout.Footprint.Depth;
					u8* destinationSubResourceMemory = memory + subResourceLayout.Offset;

					// No idea what sliceIndex relates to? Ask Georgi??
					for (u64 sliceIndex = 0; sliceIndex < subResourceDepth; sliceIndex++)
					{
						// Get data location
						u8* sourceSubResourceMemory = data + mipIndex * arrayIndex * m_Desc.Stride;

						// Copy each row, seems super slow but required for padding?
						for (u64 height = 0; height < subResourceHeight; height++)
						{
							std::memcpy(destinationSubResourceMemory, sourceSubResourceMemory, Mathf::Min(subResourcePitch, (u64)m_Desc.Stride));
							destinationSubResourceMemory += subResourcePitch; // Add to the DX12 memory using there pitch
							sourceSubResourceMemory += m_Desc.Stride; // add to our local CPU copy using our pitch
						}
					}
				}
			}

			// Unmap memory ready for copy?
			uploadBuffer->Unmap();

			// Finally issue the copy requests to copy form uplaod heap to our texture resource
			for (u64 subResourceIndex = 0; subResourceIndex < numSubResources; subResourceIndex++)
			{
				D3D12_TEXTURE_COPY_LOCATION destination = {};
				destination.pResource = m_Resource;
				destination.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				destination.SubresourceIndex = (u32)subResourceIndex;

				D3D12_TEXTURE_COPY_LOCATION source = {};
				source.pResource = uploadBuffer->m_Resource;
				source.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
				source.PlacedFootprint = FootPrintLayouts[subResourceIndex];
				source.PlacedFootprint.Offset = uploadBuffer->m_Offset; // Both of these seem wrong?

				m_Device->GetGraphicsCommandList(cmd)->CopyTextureRegion(&destination,0, 0, 0, &source, nullptr);
			}
		
			m_Device->Transition(this, 0, ResourceState::CopyDest, m_ResourceState, cmd);
		}
		else
		{
			// It's upload so directly map to memory
			Byte* destData = Map();
			std::memcpy(destData + offset, data, (size_t)size);
			Unmap();
		}
	}
}

void GraphicsResource::Release()
{
	// Only release resource if we out right own it, otherwise leave it
	if (m_IsSuballocation == false)
	{
		if (m_Resource)
		{
			m_Device->MemoryHandler()->Release(m_Resource);
		}
	}
}

ResourceDesc& GraphicsResource::GetResourceDesc()
{
	return m_Desc;
}

std::shared_ptr<GraphicsResource> GraphicsResource::CreateBuffer(GraphicsDevice* device, HeapType heapType, u32 count, u32 stride, u32 flags)
{
	return std::make_shared<GraphicsResource>(device, ResourceDesc::Buffer(heapType, count * stride, stride, flags));
}
