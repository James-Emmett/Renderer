#pragma once
#include "Type.h"

/*
	This is basically Noel Lliopis method for RTTI, GetTypeInfoStatic only creates the static once.
	May look for neater way to handle the base object case
*/

#define TYPE_OBJECT(typeName, baseTypeName) \
    public: \
        virtual const std::string&	GetTypeName() const override{ return GetTypeInfoStatic()->GetName(); } \
        virtual const TypeInfo*		GetTypeInfo() const override { return GetTypeInfoStatic(); } \
        static  const std::string&	GetTypeNameStatic() { return GetTypeInfoStatic()->GetName(); } \
        static  const TypeInfo*		GetTypeInfoStatic() { static const TypeInfo typeInfoStatic(#typeName, baseTypeName::GetTypeInfoStatic()); return &typeInfoStatic; } \
	private:\
		virtual void NOTYPEINFO() override{}\

class Object
{
protected:
	std::string m_Name;
	u32			m_ID = -1; // Number that increases for each created thing

public:
	Object()
	{
		m_ID = NextID();
	}

public:
	bool operator==(Object* other)const
	{
		return m_ID == other->m_ID;
	}

	bool operator<(Object* other)const
	{
		return m_ID < other->m_ID;
	}

	bool operator>(Object* other)const
	{
		return m_ID > other->m_ID;
	}

	bool operator<=(Object* other)const
	{
		return m_ID <= other->m_ID;
	}

	bool operator>=(Object* other)const
	{
		return m_ID >= other->m_ID;
	}

public:
	const std::string& Name()const { return m_Name; }
	void SetName(std::string& name) { m_Name = name; }
	u32 GetID()const;

	// Type Info Base
public:
	virtual const std::string&	GetTypeName() const { return GetTypeInfoStatic()->GetName(); } \
	virtual const TypeInfo*		GetTypeInfo() const { return GetTypeInfoStatic(); } \
	static const std::string&	GetTypeNameStatic() { return GetTypeInfoStatic()->GetName(); } \
	static const TypeInfo*		GetTypeInfoStatic() { static const TypeInfo typeInfoStatic("Object", nullptr); return &typeInfoStatic; }
	static u32 NextID();

protected:
	virtual void NOTYPEINFO() = 0;
};