#pragma once
#include <string>

namespace Path
{
	std::string FileName(const std::string& path);
	std::string FileNameWithoutExt(const std::string& path);
	std::string FileExtention(const std::string& path);
	std::string DirectoryPath(const std::string& path);
	bool FileExists(const std::string& path);
	bool DirectoryExists(const std::string& path);
	std::string GameDirectory();
	// Append the game name too the end of this
	std::string SaveDirectory();
	std::string ContentDirectory();
	std::string ConfigDirectory();
	std::string EngineConfigPath();
};