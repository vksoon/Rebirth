#include "Log.h"
#include "Exceptions.h"
#include "Timer.h"

#include <windows.h>
#include <iostream>

void LogToIDEConsole(const Rebirth::String& message, const char* channel)
{
	static std::mutex mutex;

	std::unique_lock lock(mutex);
	::OutputDebugString("[");
	::OutputDebugString(channel);
	::OutputDebugString("] ");
	::OutputDebugString(message.c_str());
	::OutputDebugString("\n");

	// Also default output in case we're running without debugger attached
	std::cout << "[" << channel << "] " << message << std::endl;
}

bool LogCategoryUncategorized::sRegistered = false;

RB_NAMESPACE_BEGIN

RDebug* RDebug::This = nullptr;

RDebug* RDebug::GetOrCreate()
{
	if (This == nullptr)
	{
		This = RB_New<RDebug>();

		LogCategoryUncategorized::sRegistered = RLogSystem::RegisterCategory(LogCategoryUncategorized::_id, "Uncategorized");
	}

	return This;
}

void RDebug::Terminate()
{
	if (This != nullptr)
	{
		RB_Delete<RDebug>(This);
		This = nullptr;
	}
}

void RDebug::Log(const String& Message, ELogVerbosity Verbosity, uint32 Category /* = 0 */)
{
	mLogSystem.LogMessage(Message, Verbosity, Category);

	if (Verbosity != ELogVerbosity::Log)
	{
		switch (Verbosity)
		{
		case ELogVerbosity::Fatal:
			LogToIDEConsole(Message, "FATAL");
			break;
		case ELogVerbosity::Error:
			LogToIDEConsole(Message, "ERROR");
			break;
		case ELogVerbosity::Warning:
			LogToIDEConsole(Message, "WARNING");
			break;
		default:
		case ELogVerbosity::Info:
			LogToIDEConsole(Message, "INFO");
			break;
		case ELogVerbosity::Verbose:
			LogToIDEConsole(Message, "VERBOSE");
			break;
		case ELogVerbosity::VeryVerbose:
			LogToIDEConsole(Message, "VERY_VERBOSE");
			break;
		}
	}
}

RB_CORE_API RDebug& gDebug()
{
	return *RDebug::GetOrCreate();
}

RB_NAMESPACE_END