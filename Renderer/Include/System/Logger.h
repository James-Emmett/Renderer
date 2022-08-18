#pragma once
#include <set>
#include <string>

enum class LogType
{
	Info,
	Warning,
	Error,
	Fatal
};

enum class LogLevel
{
	Low = 1,
	Medium,
	High,
	All
};

class LogObserver
{
public:
	LogLevel m_Level = LogLevel::All;
	virtual void WriteMessage(const std::string& message) = 0;
	virtual void Flush() {} // Not all will have a flush!
};

class LogHandler
{
private:
	//Better way? Prevents Dups, small list could just Vector with linear scan?
	std::set<LogObserver*> m_Observers;

public:
	void NotifyObservers(LogType type, const std::string& message);
	static LogHandler& Instance();
	static void Subscribe(LogObserver* observer);
	static void Unsubscribe(LogObserver* observer);
	static void WriteMessage(LogType type, char const* file, int line, const std::string& message);
	static void WriteMessage(LogType type, const std::string& message);
	static void FlushAll();
};

#if defined(DEBUG)
	#define LogInfo(message)	LogHandler::WriteMessage(LogType::Info, message);
	#define LogWarning(message)	LogHandler::WriteMessage(LogType::Warning, __FILE__, __LINE__, message);
	#define LogError(message)	LogHandler::WriteMessage(LogType::Error, __FILE__, __LINE__, message);
	#define LogFatal(message)	LogHandler::WriteMessage(LogType::Fatal, __FILE__, __LINE__, message);
#else
	#define  LogWarning(message);
	#define  LogError(message);
	#define  LogInfo(message);
#endif


class FileLogger : public LogObserver
{
private:
	int m_Capcity = 1000;
	int m_Count = 0;
	char byte[2500] = {};
	std::string m_FileName;

public:
	FileLogger();
	void WriteMessage(const std::string& message);
	void Flush();
};