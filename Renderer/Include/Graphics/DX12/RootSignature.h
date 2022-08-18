#pragma once
#include <d3d12.h>
#include "Graphics/Common/RootDesc.h"
#include <memory>

class GraphicsDevice;
class RootSignature
{
protected:
	GraphicsDevice*			m_Device = nullptr;
	ID3D12RootSignature*	m_Root = nullptr;
	RootSignatureDesc		m_Desc;

public:
	RootSignature(GraphicsDevice* device, RootSignatureDesc& desc);
	RootSignature(GraphicsDevice* device, ID3D12RootSignature* root);
	~RootSignature();

public:
	ID3D12RootSignature* NativeSignature();
	void Dispose();

	// Prefer this much lighter
	static std::shared_ptr<RootSignature> CreateFromShader(GraphicsDevice* device, Byte* data, u32 size);
};