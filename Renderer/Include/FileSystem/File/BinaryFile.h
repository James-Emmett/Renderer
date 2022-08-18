//NOTE:
/*
	I dont think i really need the endian flipping unless i add the Endian check.
	All target platforms are x86 based architecture now so can just use fwrite and fread 
	with no pre-flip? Im leaving the endian waste of time flip in anyways because i may
	need it in the future.
*/
#pragma once
#include "BaseFile.h"
#include "System/Types.h"

class BinaryFile : public BaseFile
{
protected:
	FILE* m_File;

public:
	BinaryFile(const std::string& path, FileMode mode);


public:
	bool  Open(const std::string& path, FileMode mode);
	void  Close();
	bool  Read(Byte* data, u32 count);
	bool  ReadFrom(Byte* data, u32 origin, u32 count);
	bool  Write(const Byte* data, u32 count);
	bool  WriteByte(const Byte& value);
	bool  WriteWord(const Word& value);
	bool  WriteDword(const Dword& value);
	bool  WriteFloat(const float& value);
	Byte  ReadByte();
	Word  ReadWord();
	Dword ReadDword();
	float ReadFloat();
	bool  WriteString(const std::string& string);
	std::string ReadString();
	void Seek(u32 origin, u32 offset);
	void SeekStart();
	void SeekEnd(); 
	int  FilePosition()const;
	bool IsEndOfFile();
};