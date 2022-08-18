#include "FileSystem/Path.h"
#include "System/StringUtil.h"

#ifdef WIN32
	// Just get core stuff no bloat please.
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <shlobj_core.h>
#endif

std::string Path::FileName(const std::string& path)
{
	// Get the last directory slash pos + 1 is start of name
	size_t pos = path.find_last_of("\\") + 1;
	if (pos != std::string::npos)
	{
		return path.substr(pos, path.length() - pos);
	}
	return std::string();
}

std::string Path::FileNameWithoutExt(const std::string& path)
{
	// Get the last directory slash pos + 1 is start of name
	size_t pos = path.find_last_of("\\") + 1;
	size_t ext = path.find_last_of(".");
	size_t length = path.length();
	if (pos != std::string::npos)
	{
		return path.substr(pos, (length - (length - ext)) - pos);
	}
	return std::string();
}

std::string Path::FileExtention(const std::string& path)
{
	size_t ext = path.find_last_of(".");
	return path.substr(ext, path.length() - ext);
}

std::string Path::DirectoryPath(const std::string& path)
{
	size_t dir = path.find_last_of("\\");
	return path.substr(0, dir);
}

bool Path::FileExists(const std::string& path)
{
	#ifdef WIN32
		DWORD attribute = GetFileAttributes(StringUtil::Widen(path).c_str());
		return attribute != (INVALID_FILE_ATTRIBUTES) && (attribute & FILE_ATTRIBUTE_DIRECTORY) == false;
	#endif // WIN32
	// Do Switch, Xbox, Psx so on...

	return false;
}

bool Path::DirectoryExists(const std::string& path)
{
#ifdef WIN32
	DWORD attribute = GetFileAttributes(StringUtil::Widen(path).c_str());
	return attribute != (INVALID_FILE_ATTRIBUTES) && (attribute & FILE_ATTRIBUTE_DIRECTORY);
#endif // WIN32
	// Do Switch, Xbox, Psx so on...

	return false;
}

std::string Path::GameDirectory()
{
	return ".\\Game\\";
}

std::string Path::SaveDirectory()
{
	#ifdef WIN32
	// Should not fail in 99.999999% of cases on win7/10
	LPWSTR path = nullptr;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_SavedGames, KF_FLAG_CREATE, NULL, &path)))
	{
		// Because windows id disgusting and uses 2 byte unicode trash, just use char jeeze
		return StringUtil::Narrow((wchar_t*)path) + "\\";
	}
	#endif
	// Do Switch, Xbox, Psx so on...

	return "";
}

std::string Path::ContentDirectory()
{
	return ".\\Game\\Content\\";
}

std::string Path::ConfigDirectory()
{
	return ".\\Game\\Config\\";
}

std::string Path::EngineConfigPath()
{
	return ".\\Game\\Config\\Engine.cfg";
}