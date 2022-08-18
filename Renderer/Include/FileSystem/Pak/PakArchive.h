//NOTE:
/*
 Based On This Article:
 https://simoncoenen.com/blog/programming/PakFiles
*/

#pragma once
#include "System/Types.h"
#include <unordered_map>
#include <memory>

class PakFile;
class BinaryFile;

struct PakHeader
{
	char m_ID[4] = { "PAK" };
	int  m_Version = 0;
	char m_FolderPath[256]; // Windows max path is basically this, not since win10/11 though
	char m_PakName[50];
	u32  m_Entries = 0;
};

struct PakEntry
{
	char m_FilePath[256];
	bool m_Compressed;
	u32	 m_HashID;			 // Hash of the filepath, saves string dictionary lookup
	u32  m_UncompressedSize;
	u32  m_CompressedSize;
	u32  m_Offset;
};

class PakArchive
{
private:
	std::unique_ptr<BinaryFile> m_File;			 // So i dont have to self delete
	std::unordered_map<u32, PakEntry> m_Entries;
	PakHeader m_Header;
	std::string m_Path = "";

	typedef std::unordered_map<u32, PakEntry>::iterator EntryIterator;

public:
	PakArchive(const std::string& path);

public:
	bool Mount();
	bool HasFile(const std::string& path)const;
	bool HasFile(u32 hashID)const;
	// Expensive use sparingly
	std::unique_ptr<PakFile> GetFile(const std::string& path);
	// Much cheaper use this
	std::unique_ptr<PakFile> GetPakFile(u32 hashID);
	BinaryFile* ArchiveFile()const { return m_File.get(); }
};