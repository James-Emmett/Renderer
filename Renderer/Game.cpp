#include "Game.h"
#include <Graphics/Common/CommonStates.h>
#include "Input/Input.h"

void Game::Initialize()
{
	const GraphicsParameters& parameters = m_GraphicsDevice->Parameters();
	m_Transform = std::make_shared<Transform>();
	m_Camera = std::make_shared<Camera>(m_Transform);
	m_Camera->SetCamera(parameters.m_Width, parameters.m_Height, 80, 0.001, 100, Projection::Perspective);

	// Get command list for copying data
	CommandList cmd = m_GraphicsDevice->BeginCommandList(QueueType::Direct);

	// Load Mesh
	m_Mesh = Mesh::LoadFromFile("Content\\MaterialBall\\MaterialBall.obj");
	m_Mesh.Upload(true, m_GraphicsDevice, cmd);

	// Load Texture
	m_Albedo = Texture::LoadFromFile("Content\\MaterialBall\\Albedo.tga");
	m_Albedo->Upload(cmd, true);

	// Create Constant Buffer, cause its "Static" atm just map to shader visible immediatly?
	m_ConstantBuffer = std::make_shared<ConstantResource>(m_GraphicsDevice, ResourceDesc::Buffer(HeapType::Default, sizeof(Matrix4), 1, (u32)BindFlag::ConstantBuffer));


	// Submit Buffer Upload Commands
	m_GraphicsDevice->SubmitCommandList(QueueType::Direct, cmd);

	// Load basic texture showing pipeline
	PipelineDesc pipeDesc = {};
	pipeDesc.BlendState = CommonStates::Opaque;
	pipeDesc.RasterState = CommonStates::CullNone;
	pipeDesc.DepthStencilState = CommonStates::DepthDefault;
	pipeDesc.Layout = VertexMesh::inputLayout;

	// Compile vertex Shader
	m_GraphicsDevice->CompilerShader("Content\\Shaders\\VertexColor.shader", "vert", "vs_5_0", &pipeDesc.Vertex.m_Data, pipeDesc.Vertex.m_Size);
	m_GraphicsDevice->CompilerShader("Content\\Shaders\\VertexColor.shader", "frag", "ps_5_0", &pipeDesc.Pixel.m_Data, pipeDesc.Pixel.m_Size);

	m_PipelineState = std::make_shared<PipelineState>(m_GraphicsDevice, RootSignature::CreateFromShader(m_GraphicsDevice, pipeDesc.Vertex.m_Data, pipeDesc.Vertex.m_Size), pipeDesc);

}

void Game::Update()
{
	const KeyboardData& keyBoard = Input::GetKeyboard();
	Vector2 input = Vector2::Zero;

	if (keyBoard.IsKey(Keys::A))
	{
		input.x = -1;
	}

	if (keyBoard.IsKey(Keys::D))
	{
		input.x = 1;
	}

	if (keyBoard.IsKey(Keys::W))
	{
		input.y = 1;
	}

	if (keyBoard.IsKey(Keys::S))
	{
		input.y = -1;
	}


	float rotDelta = 0.0f;
	if (keyBoard.IsKey(Keys::Q))
	{
		rotDelta = -1;
	}

	if (keyBoard.IsKey(Keys::E))
	{
		rotDelta = 1;
	}

	m_Camera->Move((m_Camera->m_Transform->GetRight() * input.x + m_Camera->m_Transform->GetForward() * input.y) * 8 * m_Time.DeltaTime());
	m_Camera->Rotate(rotDelta * 4 * m_Time.DeltaTime(), 0.0f);
	m_Camera->Update(m_Time.DeltaTime());
}

void Game::Draw()
{
	CommandList cmd = m_GraphicsDevice->BeginCommandList(QueueType::Direct);

	// Maybe hide this set descriptors somewhere inside GrpahicsDevice?
	// Bind the heaps we are using this frame to the command list
	m_GraphicsDevice->SetDescriptors(&m_GraphicsDevice->MemoryHandler()->m_ShaderVisibleResourceHeap, &m_GraphicsDevice->MemoryHandler()->m_ShaderVisibleSamplerHeap, cmd);

	// Fake WVP for object just at center of world
	Matrix4 WVP = Matrix4::Identity * m_Camera->GetViewProjection();
	m_ConstantBuffer->SetData((Byte*)&WVP, sizeof(Matrix4), 0, cmd);

	// Alot more code to just clear the screen and set the render target
	m_GraphicsDevice->BindDefaultViewAndScissor(cmd);
	m_GraphicsDevice->TransitionBackBuffer(ResourceState::Present, ResourceState::RenderTarget, cmd);
	m_GraphicsDevice->ClearRenderTarget(nullptr, Color::CornflowerBlue, cmd);
	m_GraphicsDevice->ClearDepthTarget(nullptr, 1.0f, 0, cmd);
	m_GraphicsDevice->BindRenderTarget(nullptr, nullptr, cmd);

	// Bind Pipeline State
	m_GraphicsDevice->BindPipeline(m_PipelineState, cmd);

	// Set the constant buffer on the root descirptor table
	m_GraphicsDevice->SetGraphicsRootDescriptorTable(0, m_ConstantBuffer->ShaderVisibleCBV(), cmd);
	m_GraphicsDevice->SetGraphicsRootDescriptorTable(1, m_Albedo->GetTextureResource()->ShaderResourceViewGPUVisible(), cmd);


	// Draw the Material Ball
	std::shared_ptr<GraphicsResource> vertexBuffer = m_Mesh.VertexBuffer();
	m_GraphicsDevice->BindVertexBuffer(&vertexBuffer, 0, 0, 1, cmd);
	m_GraphicsDevice->BindIndexBuffer(m_Mesh.IndexBuffer(), 0, cmd);
	m_GraphicsDevice->DrawIndexedInstanced(m_Mesh.IndexCount(), 1, 0, 0, 0, cmd);

	// Set render target back to present and submit this Render command list
	m_GraphicsDevice->TransitionBackBuffer(ResourceState::RenderTarget, ResourceState::Present, cmd);
	m_GraphicsDevice->SubmitCommandList(QueueType::Direct, cmd);

}

void Game::ShutDown()
{
	m_ConstantBuffer->Release();
	m_PipelineState.reset();
	m_Mesh.Dispose();
	m_Albedo->Release();
}