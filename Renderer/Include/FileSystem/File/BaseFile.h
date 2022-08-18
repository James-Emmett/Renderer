#pragma once
#include <string>
#include "FileSystem/Path.h"
#include "System/Types.h"

enum class FileMode { Read, Write, Append };
enum class FileType { Text, Binary };

class BaseFile
{
protected:
	FileMode	m_Mode = FileMode::Read;
	FileType	m_Type = FileType::Text;
	std::string m_Path = "";
	int			m_FilePosition = 0;
	bool		m_Open = false;

public:
	BaseFile() {}
	BaseFile(const std::string& path, FileMode mode, FileType type)
	{
		m_Path = path;
		m_Mode = mode;
		m_Type = type;
		m_FilePosition = 0;
	}

public:
	FileMode Mode()const;
	FileType Type()const;
	std::string GetPath()const;
	std::string FileName()const;
	std::string Directory()const;
	virtual bool Open(std::string path, FileMode mode) { return true; };
	virtual void Close()=0;
	virtual void Seek(u32 origin, u32 offset) = 0;
	virtual void SeekStart() = 0;
	virtual void SeekEnd() = 0;
	virtual int  FilePosition()const=0;
	bool IsOpen()const { return m_Open; }
	virtual bool IsEndOfFile()=0;
	int FileSize()const;
};