#include "FileSystem/Pak/PakFile.h"
#include "FileSystem/Pak/PakArchive.h"
#include "FileSystem/File/BinaryFile.h"
#include "System/Assert.h"
//#include "Zlib/Include/zlib.h"

PakFile::PakFile(const std::string& path, PakArchive* archive, PakEntry* entry) : BaseFile(path, FileMode::Read, FileType::Binary)
{
	m_Archive	= archive;
	m_Entry		= entry;
}

void PakFile::Close()
{
	m_UncompressedData.clear();
	m_Open = false;
}

bool PakFile::Read(u8* data, u32 count)
{
	if (m_Archive == nullptr) { return false; }

	// Get how many bytes too read.
	int size = (m_FilePosition + count > m_Entry->m_UncompressedSize) ? (int)m_Entry->m_UncompressedSize - (int)m_FilePosition : count;

	// We set size too negative remaning bytes so we know we over ran or hit virtual EOF
	if (size <= 0)
	{
		// Probably should just assert here rather than delay the inevitable.
		return false;
	}

	// I think by default i may just read the entire binary blob into memory to prevent multi-threaded seeks
	// in the archieve?
	if (m_Entry->m_Compressed)
	{
		if (CacheUncompressedData() == false)
		{
			return false;
		}

		memcpy(data, m_UncompressedData.data() + m_FilePosition, size);
	}
	else
	{
		assert(m_Archive->ArchiveFile()->ReadFrom(data, m_Entry->m_Offset + m_FilePosition, count));
	}

	m_FilePosition += count;
	return true;
}

int PakFile::FilePosition() const
{
	return m_FilePosition;
}

bool PakFile::IsEndOfFile()
{
	return (m_FilePosition >= (int)m_Entry->m_UncompressedSize);
}

bool PakFile::CacheUncompressedData()
{
	if (m_UncompressedData.size() == 0 && m_Archive)
	{
		std::vector<u8> tempBuffer;
		tempBuffer.resize(m_Entry->m_CompressedSize);

		if (m_Archive->ArchiveFile()->ReadFrom(tempBuffer.data(), m_Entry->m_Offset, m_Entry->m_CompressedSize) == false)
		{
			return false;
		}

		// We shouldnt need this?
		if (m_Entry->m_Compressed == false)
		{
			tempBuffer.swap(m_UncompressedData);
		}

// I need to finish this with ZLib compression libary
		m_UncompressedData.resize(m_Entry->m_UncompressedSize);
		if (true)
		{
			//uLongf sizeUncompressed = m_Entry->m_UncompressedSize;
			//uLongf sizeCompressed	= m_Entry->m_CompressedSize;
			//assert(uncompress(m_UncompressedData.data(), &sizeUncompressed, tempBuffer.data(), sizeCompressed) == Z_OK);
		}
	}
	return true;
}
