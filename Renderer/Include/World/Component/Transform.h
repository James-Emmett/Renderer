#pragma once
#include "Math/Matrix4.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include <memory>
#include <vector>

class Entity;
class Transform
{
public:
	enum class Space { Self, Parent, World };
	std::shared_ptr<Entity> m_Entity;

private:
	std::shared_ptr<Transform>					m_Parent = nullptr;
	std::vector<std::shared_ptr<Transform>>		m_Transforms;
	Matrix4										m_World;
	Matrix4										m_Inverse;

	Vector3										m_Position;
	Vector3										m_Scale;
	Quaternion									m_Rotation;

	Vector3										m_LocalPosition;
	Vector3										m_LocalScale;
	Quaternion									m_LocalRotation;

	bool										m_Dirty;

	//--Dir Vectors--
	Vector3										m_Forward;
	Vector3										m_Right;
	Vector3										m_Up;

public:
	Transform(std::shared_ptr<Entity> entity = nullptr);

public:
	std::shared_ptr<Transform> Parent()const;
	void SetParent(std::shared_ptr<Transform> parent);
	std::vector<std::shared_ptr<Transform>> Children()const;
	Matrix4		World();
	Matrix4		WorldToLocal();
	Vector3		GetForward();
	void		SetForward(const Vector3& value);
	Vector3		GetRight();
	void		SetRight(const Vector3& value);
	Vector3		GetUp();
	Vector3		GetPosition();
	void		SetPosition(const Vector3& position);
	Vector3		GetScale();
	void		SetScale(const Vector3& scale);
	Quaternion	GetRotation();
	void		SetRotation(Quaternion rotation);
	Vector3		GetLocalPosition();
	void		SetLocalPosition(const Vector3& position);
	Vector3		GetLocalScale();
	void		SetLocalScale(const Vector3& scale);
	Quaternion	GetLocalRotation();
	void		SetLocalRotation(Quaternion rotation);
	void		MarkForUpdate();
	void		RemoveTransform(std::shared_ptr<Transform> transform);
	void		Translate(const Vector3& delta, Space space);
	void		Rotate(Quaternion delta, Space space);
	void		LookAt(const Vector3& target);

	std::shared_ptr<Transform> GetChild(int index);
private:
	void UpdateMatrix();
};