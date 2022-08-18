#include "System/Assert.h"
#include "System/Logger.h"

void FailedAssert(const char* file, int line, const char* expression)
{
	LogHandler::WriteMessage(LogType::Error, file, line, expression);
	LogHandler::FlushAll();
}