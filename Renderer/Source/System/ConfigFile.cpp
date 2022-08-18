#include "System/ConfigFile.h"
#include "System/Logger.h"
#include "FileSystem/File/TextFile.h"
#include <locale>


ConfigFile::ConfigFile()
{
}


ConfigFile::~ConfigFile()
{
}

void ConfigFile::Open(std::string fileName)
{
	//Load File into our file structure
	TextFile configFile;

	if (configFile.Open(fileName.c_str(), FileMode::Read))
	{
		std::string line;
		std::string key;
		std::string value;

		std::string currentSection = "";
		Settings* currentSettings = nullptr;

		//Load all Config settings into the Map
		while (!configFile.IsEndOfFile())
		{
			configFile.ReadLine(line, true);

			if (line.length() > 0 && line.at(0) != '#') // ignore comments
			{
				if (line.at(0) == '[' && line.at(line.length() - 1) == ']')
				{
					// Section
					currentSection = line.substr(1, line.length() - 2);
					currentSettings = &m_Settings[currentSection];	// This creates the section if it doesnt exist already
				}
				else
				{
					int seperator_Pos = (int)line.find_first_of('=', (size_t)0);
					if (seperator_Pos > 0)
					{
						key = line.substr(0, seperator_Pos);
						value = line.substr((size_t)seperator_Pos + 1);
						currentSettings->insert(std::make_pair(key, value));
					}
				}
			}
		}
	}

	configFile.Close();

	m_IsOpen = true;
	m_FileName = fileName;
}

void ConfigFile::Close()
{
	TextFile configFile;

	if (configFile.Open(m_FileName.c_str(), FileMode::Write))
	{
		// Loop through each section
		for (auto const& sectionItem : m_Settings)
		{
			// Write the section header to file
			std::string section = "[" + sectionItem.first + "]" + "\n";
			configFile.Write(section.c_str(), (u32)section.size());

			// Write out each sections settings to file
			for (auto const& settingItem : sectionItem.second)
			{
				std::string value = settingItem.first + "=" + settingItem.second + "\n";
				configFile.Write(value.c_str(), (u32)value.size());
			}

			configFile.Write("\n", 1);
		}

		configFile.Close();
	}

	Clear();
}

bool ConfigFile::IsOpen() const
{
	return m_IsOpen;
}

std::string ConfigFile::GetString(const std::string& section, const std::string& key, std::string defaultValue)
{
	Settings* current = &m_Settings[section]; // Creates section if it doesnt exist!
	SettingsIteratorConst settingItr = current->find(key);
	if (settingItr != current->end())
	{
		return settingItr->second;
	}
	else
	{
		current->insert(std::pair<std::string, std::string>(key, defaultValue)); // Just overwrites it
	}

	return defaultValue;
}

void ConfigFile::SetString(const std::string& section, const std::string& key, const std::string& value)
{
	Settings* current = &m_Settings[section]; // Creates section if it doesnt exist!
	SettingsIterator itr = current->find(key);
	if (itr != current->end())
	{
		itr->second = value;
	}
	else
	{
		current->insert(std::pair<std::string, std::string>(key, value)); // Insert it.
	}
}

int ConfigFile::GetInt(const std::string& section, const std::string& key, int defaultValue)
{
	std::string value = GetString(section, key, std::to_string(defaultValue));
	return std::stoi(value);
}

void ConfigFile::SetInt(const std::string& section, const std::string& key, int value)
{
	SetString(section, key, std::to_string(value)); // Just overwrites it
}

float ConfigFile::GetFloat(const std::string& section, const std::string& key, float defaultValue)
{
	std::string value = GetString(section, key, std::to_string(defaultValue));
	return std::stof(value);
}

void ConfigFile::SetFloat(const std::string& section, const std::string& key, float value)
{
	SetString(section, key, std::to_string(value)); // Just overwrites it
}

bool ConfigFile::GetBool(const std::string& section, const std::string& key, bool defaultValue)
{
	std::string value = GetString(section, key, std::to_string(defaultValue));

	if (value == "")
	{
		LogError("Key Not Found!");
		return false;
	}

	if (value == "True" || value == "true" || value == "Yes" || value == "1")
	{
		return true;
	}

	return false;
}

void ConfigFile::SetBool(const std::string& section, const std::string& key, bool value)
{
	SetString(section, key, std::to_string(value)); // Just overwrites it
}

void ConfigFile::Clear()
{
	m_Settings.clear();
	m_IsOpen = false;
	m_FileName = "";
}
