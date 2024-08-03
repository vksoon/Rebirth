#pragma once
#include "Prerequisites.h"
#include "Timer.h"
#include "Threading.h"

RB_NAMESPACE_BEGIN

enum class ELogVerbosity
{
	Fatal,
	Error,
	Warning,
	Info,
	Log,
	Verbose,
	VeryVerbose,
	Any,
};

class RB_CORE_API RLogEntry
{
public:
	RLogEntry() = default;

	RLogEntry(String Message, ELogVerbosity Verbosity, uint32 Category)
		: mMessage(std::move(Message))
		, mVerbosity(Verbosity)
		, mCategory(Category)
		, mLocalTime(std::time(nullptr))
	{
	}

	ELogVerbosity GetVerbosity() const { return mVerbosity; }
	uint32 GetCategory() const { return mCategory; }
	const String& GetMessage() const { return mMessage; }
	const std::time_t& GetLocalTime() const { return mLocalTime; }

private:
	String mMessage;
	ELogVerbosity mVerbosity;
	uint32 mCategory;
	std::time_t mLocalTime;
};

class RB_CORE_API RLogSystem
{
public:
	RLogSystem() = default;
	~RLogSystem();

	void LogMessage(const String& Message, ELogVerbosity Verbosity, uint32 Category);
	void Clear();
	void Clear(ELogVerbosity Verbosity, uint32 Category);
	Array<RLogEntry> GetEntries() const;
	bool GetUnreadEntry(RLogEntry& Entry);
	bool GetLastEntry(RLogEntry& Entry);
	uint64 GetHash() const { return mHash; }

	static bool CategoryExists(uint32 Id);
	static uint32 GetNumCategories() { return (uint32)GCategories.size(); }
	static bool GetCategoryName(uint32 Id, String& Name);
	static bool RegisterCategory(uint32 Id, const char* Name);

private:
	friend class Debug;

	Array<RLogEntry> GetAllEntries() const;
	Array<RLogEntry> mEntries;
	Queue<RLogEntry> mUnreadEntries;
	uint64 mHash = 0;

	mutable RecursiveMutex mMutex;

	static Map<uint32, String> GCategories;
};

RB_NAMESPACE_END
