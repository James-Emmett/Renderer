#include "FileSystem/Pak/PakArchive.h"
#include "FileSystem/Pak/PakFile.h"
#include "FileSystem/File/BinaryFile.h"
#include "System/Hash32.h"
#include "System/Assert.h"

PakArchive::PakArchive(const std::string& path)
{
	m_Path = path;
}

bool PakArchive::Mount()
{
	m_File = std::make_unique<BinaryFile>(m_Path, FileMode::Read);
	if (m_File->Open(m_Path, FileMode::Read) == false)
	{
		return false;
	}

	// Header is stored at the end, so we need to read from the end :/
	int size = m_File->FileSize(); // Check this is supported on all platforms!!!!
	if (m_File->ReadFrom((Byte*)&m_Header, size - sizeof(PakHeader), sizeof(PakHeader)) == false)
	{
		return false;
	}

	if (strcmp("PAK", m_Header.m_ID) != 0)
	{
		return false;
	}

	// Load entries into hash map, slightly slower than a bulk read too vector,
	// but means the load of each individual file is best case O(1), so front load the work
	// in a loading screen!!!!
	m_File->SeekStart();
	for (size_t i = 0; i < m_Header.m_Entries; ++i)
	{
		PakEntry entry;
		if (m_File->Read((Byte*)&entry, sizeof(PakEntry)) == false)
		{
			return false;
		}

		m_Entries.insert(std::make_pair(entry.m_HashID, entry));
	}

	return true;
}

bool PakArchive::HasFile(const std::string& path) const
{
	return m_Entries.find(Hash32::ComputeHash((Byte*)path.c_str(), (unsigned int)path.length())) != m_Entries.end();
}

bool PakArchive::HasFile(u32 hashID) const
{
	return m_Entries.find(hashID) != m_Entries.end();
}

std::unique_ptr<PakFile> PakArchive::GetFile(const std::string& path)
{
	EntryIterator itr = m_Entries.find(Hash32::ComputeHash((Byte*)path.c_str(), (unsigned int)path.length()));
	assert(itr != m_Entries.end());
	return std::make_unique<PakFile>(path, this, &itr->second);
}

std::unique_ptr<PakFile> PakArchive::GetPakFile(u32 hashID)
{
	EntryIterator itr = m_Entries.find(hashID);
	assert(itr != m_Entries.end());
	return std::make_unique<PakFile>(itr->second.m_FilePath, this, &itr->second);
}