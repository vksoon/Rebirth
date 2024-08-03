#include "LogSystem.h"
#include "Exceptions.h"

RB_NAMESPACE_BEGIN

Map<uint32, String> RLogSystem::GCategories;

RLogSystem::~RLogSystem()
{
	Clear();
}

void RLogSystem::LogMessage(const String& Message, ELogVerbosity Verbosity, uint32 Category)
{
	RecursiveLock Lock(mMutex);

	mEntries.clear();

	while (!mUnreadEntries.empty())
	{
		mUnreadEntries.pop();
	}

	mHash++;
}

void RLogSystem::Clear()
{
	RecursiveLock Lock(mMutex);

	mEntries.clear();

	while (!mUnreadEntries.empty())
	{
		mUnreadEntries.pop();
	}

	mHash++;
}

void RLogSystem::Clear(ELogVerbosity Verbosity, uint32 Category)
{
	RecursiveLock Lock(mMutex);

	Array<RLogEntry> NewEntries;
	for (auto& Entry : mEntries)
	{
		if (((Verbosity == ELogVerbosity::Any) || Entry.GetVerbosity() == Verbosity) &&
			(Category == (uint32)-1 || Entry.GetCategory() == Category))
			continue;

		NewEntries.push_back(Entry);
	}

	mEntries = NewEntries;

	Queue<RLogEntry> NewUnreadEntries;
	while (!mUnreadEntries.empty())
	{
		RLogEntry Entry = mUnreadEntries.front();
		mUnreadEntries.pop();

		if (((Verbosity == ELogVerbosity::Any) || Entry.GetVerbosity() == Verbosity) &&
			(Category == (uint32)-1 || Entry.GetCategory() == Category))
			continue;

		NewUnreadEntries.push(Entry);
	}

	mUnreadEntries = NewUnreadEntries;
	mHash++;
}

bool RLogSystem::GetUnreadEntry(RLogEntry& Entry)
{
	RecursiveLock Lock(mMutex);

	if (mUnreadEntries.empty())
	{
		return false;
	}

	Entry = mUnreadEntries.front();
	mUnreadEntries.pop();
	mEntries.push_back(Entry);
	mHash++;

	return true;
}

bool RLogSystem::GetLastEntry(RLogEntry& Entry)
{
	RecursiveLock Lock(mMutex);

	if (mEntries.size() == 0)
	{
		return false;
	}

	Entry = mEntries.back();
	return true;
}

Array<RLogEntry> RLogSystem::GetEntries() const
{
	RecursiveLock Lock(mMutex);

	return mEntries;
}

bool RLogSystem::RegisterCategory(uint32 Id, const char* Name)
{
	if (!CategoryExists(Id))
	{
		GCategories.emplace(Id, Name);
		return true;
	}

	return false;
}

bool RLogSystem::CategoryExists(uint32 Id)
{
	if (GCategories.empty())
	{
		return false;
	}

	auto Search = GCategories.find(Id);
	if (Search == GCategories.end())
	{
		return false;
	}

	return true;
}

bool RLogSystem::GetCategoryName(uint32 Id, String& Name)
{
	auto Search = GCategories.find(Id);
	if (Search != GCategories.end())
	{
		Name = Search->second;
		return true;
	}

	Name = "Unknown";
	return false;
}

Array<RLogEntry> RLogSystem::GetAllEntries() const
{
	Array<RLogEntry> Entries;
	{
		std::unique_lock Lock(mMutex);

		for (auto& Entry : mEntries)
		{
			Entries.push_back(Entry);
		}

		Queue<RLogEntry> UnreadEntries = mUnreadEntries;
		while (!UnreadEntries.empty())
		{
			Entries.push_back(UnreadEntries.front());
			UnreadEntries.pop();
		}
	}

	return Entries;
}

RB_NAMESPACE_END