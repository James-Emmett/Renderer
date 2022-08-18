#include "Engine/Object.h"
static u32 s_ID = 0; // 0 is an invalid ID

u32 Object::GetID() const
{
	return m_ID;
}

u32 Object::NextID()
{
	return ++s_ID;
}