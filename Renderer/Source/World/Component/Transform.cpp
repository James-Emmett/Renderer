#include "World/Component/Transform.h"
#include "Math/Mathf.h"

Transform::Transform(std::shared_ptr<Entity> entity)
{
	m_Entity = entity;
	m_Dirty = true;
	SetPosition(Vector3::Zero);
	SetRotation(Quaternion());
	SetScale(Vector3::One);
}

std::shared_ptr<Transform> Transform::Parent() const
{
	return m_Parent;
}

void Transform::SetParent(std::shared_ptr<Transform> parent)
{
	if (m_Parent == parent)
	{
		return;
	}

	if (m_Parent != nullptr)
	{

		// Copy old world first
		Matrix4 world = World();


		// Yuk, remove child from parents
		size_t size = m_Parent->m_Transforms.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_Parent->m_Transforms[i].get() == this)
			{
				// Swap with last, then pop it avoids shuffle of k elements from erase point
				m_Parent->m_Transforms[i] = m_Parent->m_Transforms.back();
				m_Parent->m_Transforms.pop_back();
				break;
			}
		}
	}
}

std::vector<std::shared_ptr<Transform>> Transform::Children()const
{
	return m_Transforms;
}

Matrix4 Transform::World()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}
	return m_World;
}

Matrix4 Transform::WorldToLocal()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}
	return m_Inverse;
}

Vector3 Transform::GetForward()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}

	return m_Forward;
}

void Transform::SetForward(const Vector3& value)
{
	SetRotation(Quaternion::LookRotation(value, Vector3::Up));
}

Vector3 Transform::GetRight()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}

	return m_Right;
}

void Transform::SetRight(const Vector3& value)
{
	SetRotation(Quaternion::LookRotation(Vector3::Cross(value, Vector3::Up), Vector3::Up));
}

Vector3 Transform::GetUp()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}

	return m_Up;
}


Vector3 Transform::GetPosition()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}

	return m_Position;
}

void Transform::SetPosition(const Vector3& position)
{
	MarkForUpdate();
	if (m_Parent == nullptr)
	{
		m_LocalPosition = position;
	}
	else
	{
		// Get the world position relative too the parents local space.
		m_LocalPosition = m_Parent->m_Inverse.TransformPoint3x4(position);
	}

	MarkForUpdate();
}

Vector3 Transform::GetScale()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}
	return m_Scale;
}

void Transform::SetScale(const Vector3& scale)
{
	MarkForUpdate();
	if (m_Parent == nullptr)
	{
		m_LocalScale = scale;
	}
	else
	{
		// Divison gets it relative to parent scale.
		// i.e if parent is 10 and child is 20 then child local scale is 2 as its
		// 2 * parent scale
		m_LocalScale.x = scale.x / m_Parent->GetScale().x;
		m_LocalScale.y = scale.y / m_Parent->GetScale().y;
	}
}

Quaternion Transform::GetRotation()
{
	if (m_Dirty)
	{
		UpdateMatrix();
	}

	return m_Rotation;
}

void Transform::SetRotation(Quaternion rotation)
{
	if (m_Parent == nullptr)
	{
		SetLocalRotation(rotation);
	}
	else
	{
		// Get into parents space by mutiplying by inverse
		SetLocalRotation(Quaternion::Inverse(m_Parent->GetRotation()) * rotation);
	}
}

Vector3 Transform::GetLocalPosition()
{
	return m_LocalPosition;
}

void Transform::SetLocalPosition(const Vector3& position)
{
	MarkForUpdate();
	m_LocalPosition = position;
}

Vector3 Transform::GetLocalScale()
{
	return m_LocalScale;
}

void Transform::SetLocalScale(const Vector3& scale)
{
	MarkForUpdate();
	m_LocalScale = scale;
}

Quaternion Transform::GetLocalRotation()
{
	return m_LocalRotation;
}

void Transform::SetLocalRotation(Quaternion rotation)
{
	MarkForUpdate();
	m_LocalRotation = Quaternion::Normalize(rotation);
}

void Transform::MarkForUpdate()
{
	if (m_Dirty == false)
	{
		m_Dirty = true;
		for (size_t i = 0; i < m_Transforms.size(); ++i)
		{
			m_Transforms[i]->MarkForUpdate();
		}
	}
}

void Transform::RemoveTransform(std::shared_ptr<Transform> transform)
{
	m_Transforms.erase(std::remove(m_Transforms.begin(), m_Transforms.end(), transform));
}

void Transform::Translate(const Vector3& delta, Space space)
{
	switch (space)
	{
	case Transform::Space::Self:
		// Add the delta relative too our local coordinate system/space..
		m_LocalPosition += Matrix4::Rotate(m_LocalRotation).TransformPoint3x4(delta);
		break;
	case Transform::Space::Parent:
		// Straight up as local is relative to parent anyways
		m_LocalPosition += delta;
		break;
	case Transform::Space::World:
		if (m_Parent != nullptr)
		{
			// Add the position transformed into the local space of the parents world space.
			m_LocalPosition += m_Parent->WorldToLocal().TransformPoint3x4(delta);
		}
		else
		{
			// No parent so world space is our default space..
			m_LocalPosition += delta;
		}
		break;
	}

	MarkForUpdate();
}

void Transform::Rotate(Quaternion delta, Space space)
{
	switch (space)
	{
	case Transform::Space::Self:
	case Transform::Space::Parent:
	{
		m_LocalRotation = Quaternion::Normalize(delta * m_LocalRotation);
	}
	break;
	case Transform::Space::World:
		if (m_Parent != nullptr)
		{
			// Multiply delta world relative space by parents inverse to get it relative to parent, then multiply by our local to modify it.
			m_LocalRotation = Quaternion::Normalize(m_LocalRotation * (Quaternion::Inverse(m_Parent->GetRotation()), delta));
		}
		else
		{
			m_LocalRotation = Quaternion::Normalize(m_LocalRotation * delta);
		}
		break;
	}

	// Not really nesassery as the functions cover it anyways.
	MarkForUpdate();
}

void Transform::LookAt(const Vector3& target)
{
	SetRotation(Quaternion::LookRotation(target - m_Position, Vector3::Up));
}

std::shared_ptr<Transform> Transform::GetChild(int index)
{
	return m_Transforms[index];
}

void Transform::UpdateMatrix()
{
	Matrix4 localMatrix = Matrix4::Translate(m_LocalPosition) * Matrix4::Rotate(m_LocalRotation) * Matrix4::Scale(m_LocalScale);


	if (m_Parent == nullptr)
	{
		m_World = localMatrix;
		m_Position = m_LocalPosition;
		m_Rotation = m_LocalRotation;
		m_Scale = m_Scale;
	}
	else
	{
		// Transform the local matrix by the parents world to get us relative to parents space.
		m_World = localMatrix * m_Parent->World();
		m_Position = m_World.TransformPoint3x4(m_LocalPosition); // Just roate + position no z divide and cheaper
		m_Rotation = m_Parent->m_Rotation * m_LocalRotation;
		// Just multiple these
		m_Scale = Vector3(m_Parent->m_Scale.x * m_LocalScale.x, m_Parent->m_Scale.y * m_LocalScale.y, m_Parent->m_Scale.z * m_LocalScale.z);
	}

	Matrix4 rot = Matrix4::Rotate(m_Rotation);
	m_Right = Vector3::Normalize(rot.TransformPoint3x4(Vector3(1, 0, 0)));
	m_Forward = Vector3::Normalize(rot.TransformPoint3x4(Vector3(0, 0, 1)));
	m_Up = Vector3::Normalize(rot.TransformPoint3x4(Vector3(0, 1, 0)));
	m_Inverse = Matrix4::Inverse(m_World);
	m_Dirty = false;
}