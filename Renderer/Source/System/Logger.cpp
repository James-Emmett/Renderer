#include "System/Logger.h"
#include "System/Types.h"
#include "System/Assert.h"
#include "System/Timer.h"
#include "FileSystem/File/TextFile.h"

std::string logTable[5] =
{
	"[Info]: ",
	"[Warning]: ",
	"[Error]: ",
	"[Assert]: ",
	"[Fatal]: "
};


LogHandler& LogHandler::Instance()
{
	static LogHandler logger;
	return logger;
}

void LogHandler::NotifyObservers(LogType type, const std::string& message)
{
	for (LogObserver* observer : m_Observers)
	{
		// Only output if you have correct level output!
		if (observer->m_Level == LogLevel::All || (int)type <= (int)observer->m_Level)
		{
			observer->WriteMessage(message);
		}
	}
}

void LogHandler::Subscribe(LogObserver* observer)
{
	if (observer)
	{
		Instance().m_Observers.insert(observer);
	}
}

void LogHandler::Unsubscribe(LogObserver* observer)
{
	if (observer)
	{
		Instance().m_Observers.erase(observer);
	}
}

void LogHandler::WriteMessage(LogType type, char const* file, int line, const std::string& message)
{
	std::string shortFile = file;
	u32 start = (u32)shortFile.find_last_of("\\");
	shortFile = shortFile.substr((size_t)start + 1);

	std::string outMessage;
	outMessage += logTable[(size_t)type];
	outMessage += " [" + shortFile + "] ";
	outMessage += " [Line: " + std::to_string(line) + "]:";
	outMessage += " [Message: " + message + " ]";

	Instance().NotifyObservers(type, outMessage.c_str());
}

void LogHandler::WriteMessage(LogType type, const std::string& message)
{
	std::string outMessage;
	outMessage += logTable[(size_t)type];
	outMessage += " " + message;
	Instance().NotifyObservers(type, outMessage.c_str());
}

void LogHandler::FlushAll()
{
	std::set<LogObserver*>& observerSet = Instance().m_Observers;

	for (LogObserver* observer : observerSet)
	{
		observer->Flush();
	}
}

FileLogger::FileLogger()
{
	std::string file = "Log.log";
	int count = 0;
	while (Path::FileExists(file))
	{
		++count;
		file = "Log " + std::to_string(count);
		file += ".log";
	}

	m_FileName = file;
}

void FileLogger::WriteMessage(const std::string& message)
{
	size_t length = message.length();
	if (m_Count + length >= m_Capcity)
	{
		Flush();
	}

	// Wirte the message to buffer
	memcpy(byte + m_Count, message.c_str(), length);
	m_Count += (int)length;
	byte[m_Count++] = '\n';
}

void FileLogger::Flush()
{
	TextFile file;
	file.Open(m_FileName, FileMode::Append);
	file.Write(byte, m_Count);
	m_Count = 0;
	file.Close();
}