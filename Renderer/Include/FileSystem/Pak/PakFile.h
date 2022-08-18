#pragma once
#include "FileSystem/File/BaseFile.h"
#include <vector>

struct PakEntry;
class PakArchive;
class PakFile : public BaseFile
{
protected:
	PakEntry*			m_Entry		= nullptr;
	PakArchive*			m_Archive	= nullptr;
	std::vector<u8>		m_UncompressedData;

public:
	PakFile(const std::string& path, PakArchive* archive, PakEntry* entry);

public:
	void  Close();
	bool  Read(u8* data, u32 count);
	bool  ReadFrom(u8* data, u32 origin, u32 count) { return true; }
	u8  ReadByte() {return 0;}
	u16  ReadWord() {return 0;}
	u32 ReadDword() {return 0;}
	float ReadFloat() {return 0;}
	std::string ReadString() { return ""; }
	void Seek(u32 origin, u32 offset) {}
	void SeekStart() {}
	void SeekEnd() {}
	int  FilePosition()const;
	bool IsEndOfFile();

private:
	bool CacheUncompressedData();
};