#include "FileSystem/File/BaseFile.h"
#include <sys/stat.h>

FileMode BaseFile::Mode()const { return m_Mode; }
FileType BaseFile::Type()const { return m_Type; }
std::string BaseFile::GetPath()const { return m_Path; }
std::string BaseFile::FileName()const { return Path::FileName(m_Path); }
std::string BaseFile::Directory()const { return Path::DirectoryPath(m_Path); }

int BaseFile::FileSize() const
{
	struct stat st;
	stat(m_Path.c_str(), &st);
	return st.st_size;
}
