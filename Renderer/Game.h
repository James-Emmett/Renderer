#pragma once
#include "Engine/Engine.h"
#include "Resource/Mesh.h"
#include "Resource/Texture.h"
#include "World/Component/Transform.h"
#include "World/Camera.h"

class Game : public Engine
{
public:
	Mesh m_Mesh; // Probalby should be shared_ptr as well
	std::shared_ptr<Texture>			m_Albedo;
	std::shared_ptr<ConstantResource>	m_ConstantBuffer;
	std::shared_ptr<PipelineState>		m_PipelineState;
	std::shared_ptr<Camera>				m_Camera;
	std::shared_ptr<Transform>			m_Transform;

public:
	void Initialize();
	void Update();
	void Draw();
	void ShutDown();
};