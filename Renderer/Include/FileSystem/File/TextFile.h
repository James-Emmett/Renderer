#pragma once
#include "BaseFile.h"
#include "System/Types.h"

class TextFile : public BaseFile
{
public:
	FILE* m_File = nullptr;

public:
	TextFile(){}
	TextFile(const std::string& path, FileMode mode);

public:
	bool Open(const std::string& path, FileMode mode);
	void Close();

	bool Read(char* buffer, u32 bufferSize);
	bool ReadLine(char* buffer, u32 bufferSize);
	// Max Length 1024
	bool ReadLine(std::string& line, bool removeTralingSpace);
	std::string ReadLine(bool removeTralingSpace);
	bool Write(const char* buffer, u32 bufferSize);
	bool WriteLine(const std::string& line);

	void Seek(u32 origin, u32 offset);
	void SeekStart();
	void SeekEnd();
	int  FilePosition()const;
	bool IsEndOfFile();
};