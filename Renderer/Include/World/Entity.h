// Note:
/*
	The Object model in this engine is very very basic, it does not take advantage of DOD,
	However its written strictly as a heirarchy of Entities with Components too remove the
	Virtual function/Virtual Machine nature of Unity style Component Entity Model. This means
	it is slightly more perfomant in exchange for slightly more rigidity, however im the only programmer
	there is no designers or designer side scripting etc. So its fine for this project aslthough a tad bit
	manual.
*/
#pragma once
#include "Component/Transform.h"
#include "Engine/Object.h"

enum class EntityState { Deactive, Active, Destroyed };

class Scene;
class Entity : public Object
{
	TYPE_OBJECT(Entity, Object)

public:
	Transform m_Transform;

private:
	static const int Untaged;
	EntityState		 m_State = EntityState::Deactive;
	int				 m_Tag = -1;

public:
	Entity(u32 id = -1, std::string name = "Entity", bool active = true, int tag = Untaged);
	bool IsActive()const;
	void SetActive(bool value);
	int  Tag()const;
	void SetTag(int tag);
	EntityState GetState()const;
	virtual void OnActive() {};
	virtual void OnDeactivated() {};
	virtual void OnDestroyed() = 0;
	void Destory();
};