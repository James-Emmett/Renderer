#pragma once
#include "Engine/Object.h"

class Resource : public Object
{
	TYPE_OBJECT(Resource, Object);

protected:
	std::string		m_FilePath;
	unsigned int	m_Memory = 0;

public:
	void SetPath(const std::string& path) { m_FilePath = path; }
	const std::string& FilePath()const { return m_FilePath; }
	u32 MemoryUsage()const { return m_Memory; }
};