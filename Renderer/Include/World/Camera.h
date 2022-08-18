#pragma once
#include "Math/Mathf.h"
#include "Math/Matrix4.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "Engine/Object.h"
#include "Component/Transform.h"
#include <memory>

enum class Projection { Orthographic, Perspective };
enum class ClearFlag { SolidColor, SkyBox, None };

// Camera an object or should it be a component?
class Camera : Object
{
public:
	TYPE_OBJECT(Entity, Object)

private:
	Matrix4		m_Projection = Matrix4::Identity;
	Matrix4		m_InvProjection = Matrix4::Identity;
	Matrix4		m_View = Matrix4::Identity;
	Matrix4		m_InvView = Matrix4::Identity;
	Rect<float>	m_ViewPortRect = Rect<float>(0, 0, 1, 1);
	float		m_NearPlane = 0.01f;
	float		m_FarPlane = 100.0f;
	float		m_AspectRatio = 0.0f;
	float		m_FOV = Mathf::PI / 3; // 60 degrees
	u32			m_Width = 0;
	u32			m_Height = 0;
	bool		m_ViewDirty = true;
	bool		m_ProjectDrity = true;

public:
	static Camera main;
	std::shared_ptr<Transform>	m_Transform = nullptr;
	Projection					m_ProjectionMode = Projection::Perspective;
	ClearFlag					m_ClearFlags = ClearFlag::SolidColor;
	Color						m_Background = Color::CornflowerBlue;

public:
	Camera(std::shared_ptr<Transform> transform);
	~Camera();

	// Creates Perspective or Ortho camera
	void	SetCamera(u32 width, u32 height, float fov, float near, float far, Projection mode);
	void	SetNearPlane(float nearPlane);
	float	GetNearPlane()const;
	void	SetFarPlane(float farPlane);
	float	GetFarPlane()const;
	void	SetAspectRatio(float aspect);
	float	GetAspect()const;
	void	SetFOV(float fov);
	float	GetFOV()const;
	float	GetNearPlaneWidth()const;
	float	GetNearPlaneHeight()const;
	float	GetFarPlaneWidth()const;
	float	GetFarPlaneHeight()const;
	float	GetFocalLength()const;
	void	SetWidth(u32 width = 1920);
	u32		GetWidth()const;
	void	SetHeight(u32 height = 1080);
	u32		GetHeight()const;
	Matrix4 GetView()const;
	void	SetView(Matrix4 view);
	Matrix4 GetInvView()const;
	Matrix4 GetProjection()const;
	void	SetProjection(Matrix4 proj);
	Matrix4 GetInvProjection()const;
	Matrix4 GetViewProjection()const;
	void	LookAt(Vector3 target);
	Vector3 ViewPortToWorldPoint(const Vector3& position)const;
	Vector3 GetPosition()const;

	//--Helper Movement functions--
	void Move(const Vector3& translate);
	void Rotation(const Vector3& euler);
	void Rotate(float yaw, float pitch);
	virtual void Update(float deltaTime);
	void OnGui();
};