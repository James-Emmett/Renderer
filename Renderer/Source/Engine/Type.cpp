#include "Engine/Type.h"
#include "System/Hash32.h"
#include "System/Assert.h"

TypeInfo::TypeInfo(const std::string& name, const TypeInfo* parent)
{
	m_Name = name;
	m_Hash = Hash32::ComputeHash((u8*)name.c_str(), (unsigned int)name.length());
	m_Parent = parent;
}

TypeInfo::~TypeInfo()
{
	m_Parent = nullptr;
}

bool TypeInfo::IsType(u32 typeHash) const
{
	// Loop through this and all parents
	const TypeInfo* type = this;
	while (type != nullptr)
	{
		if (typeHash == m_Hash)
		{
			return true;
		}

		type = type->m_Parent;
	}

	return false;
}

bool TypeInfo::IsType(const TypeInfo* type) const
{
	assert(type != nullptr);
	return IsType(type->GetType());
}