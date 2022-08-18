#include "FileSystem/File/TextFile.h"
#include "System/Assert.h"

TextFile::TextFile(const std::string& path, FileMode mode) : BaseFile(path, mode, FileType::Text)
{
	Open(path, mode);
}

bool TextFile::Open(const std::string& path, FileMode mode)
{
	m_Path = path;
	m_Mode = mode;

	const char* fileMode;

	if (m_Mode == FileMode::Read)
	{
		fileMode = "r";
	}
	else if (m_Mode == FileMode::Write)
	{
		fileMode = "w";
	}
	else
	{
		fileMode = "a";
	}

	fopen_s(&m_File, m_Path.c_str(), fileMode);
	if (m_File == nullptr)
	{
		return false;
	}

	m_FilePosition = 0;
	m_Open = true;
	return true;
}

void TextFile::Close()
{
	if (m_File)
	{
		fclose(m_File);
	}

	m_FilePosition = 0;
	m_Open = false;
}

bool TextFile::Read(char* buffer, u32 size)
{
	if (buffer && m_Mode == FileMode::Read)
	{
		bool result = fread(buffer, sizeof(char), size, m_File) >= size;
		if (result)
		{
			m_FilePosition += ftell(m_File);
			return true;
		}
	}

	return false;
}

bool TextFile::ReadLine(char* buffer, u32 size)
{
	if (buffer && m_Mode == FileMode::Read)
	{
		bool result = fgets(buffer, size, m_File) != NULL;
		if (result)
		{
			m_FilePosition = ftell(m_File);
			return true;
		}
	}

	return false;
}

bool TextFile::ReadLine(std::string& line, bool removeTralingSpace)
{
	if (m_Mode == FileMode::Read)
	{
		char buffer[1024];
		if (fgets(buffer, 1024, m_File) != NULL)
		{
			line = std::string(buffer);
			if (removeTralingSpace)
			{
				while (line.length() > 0 && isspace(line.back()))
				{
					line.pop_back();
				}
			}

			m_FilePosition += (int)line.length();
			return true;
		}
	}
	return false;
}

std::string TextFile::ReadLine(bool removeTralingSpace)
{
	std::string line = "";
	if (m_Mode == FileMode::Read)
	{
		char buffer[1024];
		if (fgets(buffer, 1024, m_File) != NULL)
		{
			line = std::string(buffer);
			if (removeTralingSpace)
			{
				while (line.length() > 0 && isspace(line.back()))
				{
					line.pop_back();
				}
			}

			m_FilePosition += (int)line.length();
		}
	}
	return line;
}

bool TextFile::Write(const char* buffer, u32 size)
{
	if (buffer != nullptr && (m_Mode == FileMode::Write || m_Mode == FileMode::Append))
	{
		bool result = fwrite(buffer, sizeof(char), size, m_File) >= size;
		if (result)
		{
			m_FilePosition += size;
			return true;
		}
	}
	return false;
}

bool TextFile::WriteLine(const std::string& line)
{
	if ((m_Mode == FileMode::Write || m_Mode == FileMode::Append))
	{
		assert(fputs(line.c_str(), m_File) != EOF);
		assert(fputc('\n', m_File) != EOF);
		m_FilePosition = ftell(m_File);
		return true;
	}

	return false;
}

void TextFile::Seek(u32 origin, u32 offset)
{
	fseek(m_File, offset, origin);
	m_FilePosition = ftell(m_File);
}

void TextFile::SeekStart()
{
	fseek(m_File, 0, SEEK_SET);
	m_FilePosition = ftell(m_File);
}

void TextFile::SeekEnd()
{
	fseek(m_File, 0, SEEK_END);
	m_FilePosition = ftell(m_File);
}

int TextFile::FilePosition() const
{
	return m_FilePosition;
}

bool TextFile::IsEndOfFile()
{
	return feof(m_File);
}