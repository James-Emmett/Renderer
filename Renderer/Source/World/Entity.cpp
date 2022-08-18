#include "World/Entity.h"

const int Entity::Untaged = -1;

Entity::Entity(u32 id, std::string name, bool active, int tag)
{
	if (id == (u32)-1) { id = NextID(); }
	m_ID = id;
	m_Name = name;
	m_State = (EntityState)active;
	m_Tag = tag;
}

bool Entity::IsActive() const
{
	return m_State == EntityState::Active;
}

void Entity::SetActive(bool value)
{
	if (m_State != EntityState::Destroyed && m_State != (EntityState)value)
	{
		m_State = (EntityState)value;
		if (value)
		{
			OnActive();
		}
		else
		{
			OnDeactivated();
		}
	}
}

int Entity::Tag()const
{
	return m_Tag;
}

void Entity::SetTag(int tag)
{
	m_Tag = tag;
}

EntityState Entity::GetState() const
{
	return m_State;
}

void Entity::Destory()
{
	m_State = EntityState::Destroyed;
	// Cleans up all components first.
	OnDestroyed();
}