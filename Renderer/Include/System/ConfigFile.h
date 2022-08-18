//Note:
/*
	Common implementation, in this case its
	a modified ogre style system. Seen it in unity a few times though...
*/
#pragma once
#include <string>
#include <map>

typedef std::map<std::string, std::string> Settings;
typedef std::map<std::string, std::string>::iterator SettingsIterator;
typedef std::map<std::string, std::string>::const_iterator SettingsIteratorConst;
typedef std::map<std::string, Settings> Section;
typedef std::map<std::string, Settings>::iterator SectionIterator;
typedef std::map<std::string, Settings>::const_iterator SectionIteratorConst;

class ConfigFile
{
	//----------------Attributes------------------
public:
	Section m_Settings;

private:
	bool m_IsOpen = false;
	std::string m_FileName = "";

	//----------------Constructors---------------
public:
	ConfigFile();
	~ConfigFile();

	//----------------Methods--------------------
public:
	//Loads Settings File From Location
	void Open(std::string fileName);
	void Close();
	bool IsOpen()const;

	std::string GetString(const std::string& section, const std::string& key, std::string defaultValue = "");
	void SetString(const std::string& section, const std::string& key, const std::string& value);
	int  GetInt(const std::string& section, const std::string& key, int defaultValue = 0);
	void SetInt(const std::string& section, const std::string& key, int value);
	float GetFloat(const std::string& section, const std::string& key, float defaultValue = 0.0f);
	void SetFloat(const std::string& section, const std::string& key, float value);
	bool GetBool(const std::string& section, const std::string& key, bool defaultValue = false);
	void SetBool(const std::string& section, const std::string& key, bool value);

	void Clear();
};
