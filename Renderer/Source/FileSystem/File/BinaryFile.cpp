#include "FileSystem/File/BinaryFile.h"

BinaryFile::BinaryFile(const std::string& path, FileMode mode) : BaseFile(path, mode, FileType::Binary)
{
	Open(path, mode);
}

bool BinaryFile::Open(const std::string& path, FileMode mode)
{
	m_Path = path;
	m_Mode = mode;

	const char* fileMode;

	if (m_Mode == FileMode::Read)
	{
		fileMode = "rb";
	}
	else
	{
		fileMode = "wb";
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

void BinaryFile::Close()
{
	if (m_File)
	{
		fclose(m_File);
		m_FilePosition = 0;
	}

	m_Open = false;
}

bool BinaryFile::Read(Byte* data, u32 count)
{
	if (m_File && m_Mode == FileMode::Read)
	{
		bool result = fread(data, sizeof(Byte), count, m_File) >= count;
		if (result)
		{
			m_FilePosition += count;
			return true;
		}
	}

	return false;
}

bool BinaryFile::ReadFrom(Byte* data, u32 origin, u32 count)
{
	if (m_File && m_Mode == FileMode::Read)
	{
		Seek(count, origin);
		bool result = fread(data, sizeof(Byte), count, m_File);
		if (result)
		{
			m_FilePosition = ftell(m_File);
			return true;
		}
	}
	return false;
}

bool BinaryFile::Write(const Byte* data, u32 count)
{
	if (m_File && m_Mode == FileMode::Write)
	{
		bool result = fwrite(data, sizeof(Byte), count, m_File) >= count;
		if (result)
		{
			m_FilePosition += count;
			return true;
		}
	}

	return false;
}

bool BinaryFile::WriteByte(const Byte& value)
{
	if (m_Mode != FileMode::Read)
	{
		fwrite(&value, 1, sizeof(Byte), m_File);
		m_FilePosition += 1;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
bool BinaryFile::WriteWord(const Word& value)
{
	if (m_Mode != FileMode::Read)
	{
		Byte bytes[2];
		bytes[0] = value & 0xFF;
		bytes[1] = (value >> 8) & 0xFF;

		fwrite(bytes, 1, sizeof(bytes), m_File);
		m_FilePosition += 2;
		return true;
	}

	return false;
}

bool BinaryFile::WriteDword(const Dword& value)
{
	if (m_Mode != FileMode::Read)
	{
		Byte bytes[4];
		bytes[0] = value & 0xFF;
		bytes[1] = (value >> 8) & 0xFF;
		bytes[2] = (value >> 16) & 0xFF;
		bytes[3] = (value >> 24) & 0xFF;
		fwrite(bytes, 1, sizeof(bytes), m_File);
		m_FilePosition += 4;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// I had issues writing floats for some reason, so this hack was invented.
//-----------------------------------------------------------------------------
bool BinaryFile::WriteFloat(const float& value)
{
	if (m_File && m_Mode != FileMode::Write)
	{
		//Type pruning hack?
		union u
		{
			float f;
			Byte data[4];
		}bytes;
		bytes.f = value;

		fwrite(&bytes, sizeof(u), sizeof(bytes), m_File);
		m_FilePosition += 4;
		return true;
	}

	return false;
}

Byte BinaryFile::ReadByte()
{
	if (m_File && m_Mode == FileMode::Read)
	{
		Byte byte;
		fread(&byte, 1, sizeof(byte), m_File);
		m_FilePosition += 1;
		return byte;
	}

	return Byte();
}

Word BinaryFile::ReadWord()
{
	if (m_File && m_Mode == FileMode::Read)
	{
		Byte byte[2];
		fread(byte, 1, sizeof(byte), m_File);
		m_FilePosition += 2;
		return (byte[0] | byte[1] << 8);
	}

	return Byte();
}

Dword BinaryFile::ReadDword()
{
	if (m_File && m_Mode == FileMode::Read)
	{
		Byte byte[4];
		fread(byte, 1, sizeof(byte), m_File);
		m_FilePosition += 4;
		return (byte[0] | byte[1] << 8 | byte[2] << 16 | byte[3] << 24);
	}

	return Byte();
}

float BinaryFile::ReadFloat()
{
	if (m_File && m_Mode == FileMode::Read)
	{
		float value;
		fread(&value, 1, sizeof(value), m_File);
		m_FilePosition += 4;
		return value;
	}

	return float();
}

bool BinaryFile::WriteString(const std::string& string)
{
	if (!string.empty())
	{
		WriteDword((Dword)string.size() + 1);
		Write((Byte*)string.c_str(), (u32)string.size());

		char n = '\0';
		WriteByte(n); // Make sure to write null terminator.
		m_FilePosition += (int)string.length() + 1;
		return true;
	}
	return false;
}

std::string BinaryFile::ReadString()
{
	std::string string;
	int n = ReadDword();
	string.resize(n);
	Read((Byte*)string.c_str(), n);
	m_FilePosition += (int)string.length() + 1;
	return string;
}

void BinaryFile::Seek(u32 offset, u32 origin)
{
	fseek(m_File, offset, origin);
	m_FilePosition = ftell(m_File);
}

void BinaryFile::SeekStart()
{
	fseek(m_File, 0, SEEK_SET);
	m_FilePosition = ftell(m_File);
}

void BinaryFile::SeekEnd()
{
	fseek(m_File, 0, SEEK_END);
	m_FilePosition = ftell(m_File);
}

int BinaryFile::FilePosition() const
{
	return m_FilePosition;
}

bool BinaryFile::IsEndOfFile()
{
	return feof(m_File);
}
