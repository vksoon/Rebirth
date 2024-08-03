#pragma once
#include "Prerequisites.h"
#include "LogSystem.h"

RB_NAMESPACE_BEGIN

enum class ESavedLogType
{
	Text,
};

class RB_CORE_API RDebug
{
public:
	RDebug() = default;

	void Log(const String& Message, ELogVerbosity Verbosity, uint32 Category = 0);

	RLogSystem& GetLogSystem() { return mLogSystem; }

	static RDebug* GetOrCreate();
	static void Terminate();

private:
	uint64 mLogHash = 0;
	RLogSystem mLogSystem;
	static RDebug* This;
};

RB_CORE_API RDebug& gDebug();

RB_NAMESPACE_END

#ifndef RB_LOG_VERBOSITY
#if RB_DEBUG_MODE
#define RB_LOG_VERBOSITY ELogVerbosity::Log
#else
#define RB_LOG_VERBOSITY ELogVerbosity::Warning
#endif
#endif

#define RB_LOG_CATEGORY(name, id) struct LogCategory##name { enum { _id = id }; static bool sRegistered; };

#define RB_LOG_CATEGORY_IMPL(name) bool LogCategory##name::sRegistered = Rebirth::RLogSystem::RegisterCategory(LogCategory##name::_id, #name);

#define RB_LOG_GET_CATEGORY_ID(category) LogCategory##category::_id

#define RB_LOG(verbosity, category, message, ...)															 \
	do																										 \
	{																										 \
		using namespace ::Rebirth;																	 \
		if ((int32)ELogVerbosity::verbosity <= (int32)RB_LOG_VERBOSITY)										 \
		{																									 \
			gDebug().Log(StringUtil::Format(String(message), ##__VA_ARGS__) + String("\n\t\t in ") +		 \
							__PRETTY_FUNCTION__ + " [" + __FILE__ + ":" + ToString((int32)__LINE__) + "]\n", \
						ELogVerbosity::verbosity, LogCategory##category::_id);								 \
		}																									 \
	} while (0)

RB_LOG_CATEGORY(Uncategorized, 0)
