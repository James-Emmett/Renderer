#pragma once
#include "System/Types.h"
#include <string>

// Type for the fake RTTI System
class TypeInfo
{
private:
	const TypeInfo*	m_Parent = nullptr;
	std::string		m_Name;
	u32				m_Hash;

public:
	TypeInfo(const std::string& name, const TypeInfo* parent);
	~TypeInfo();

public:
	bool IsType(u32 typeHash)const;
	bool IsType(const TypeInfo* type)const;
	
	// This just saves time for typeing
	template<typename T>
	bool IsType() { return IsType(T::GetTypeInfoStatic()); }

	const u32 GetType()const { return m_Hash; }
	const std::string& GetName()const { return m_Name; }
	const TypeInfo* GetParent()const { return m_Parent; }
};