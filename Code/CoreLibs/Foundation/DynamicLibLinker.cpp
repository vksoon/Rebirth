#include "DynamicLibLinker.h"
#include "Exceptions.h"

#if RB_PLATFORM == RB_PLATFORM_MSW
#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER)
#define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <windows.h>
#endif

RB_NAMESPACE_BEGIN

RDynamicLibLinker::RDynamicLibLinker(const String& name)
	: mName(name)
	, mHandle(nullptr)
{
}

RDynamicLibLinker::~RDynamicLibLinker()
{
}

bool RDynamicLibLinker::Load()
{
	if (mHandle)
		return false;

	mHandle = (DYNLIB_HANDLE)DYNLIB_LOAD(mName.c_str());

	if (!mHandle)
	{
		RB_EXCEPT(InternalErrorException,
			"Could not load dynamic library " + mName + ".  System Error: " + GetDllError());

		return false;
	}

	return true;
}

void RDynamicLibLinker::Unload()
{
	if (!mHandle)
		return;

	if (!DYNLIB_UNLOAD(mHandle))
	{
		RB_EXCEPT(InternalErrorException,
			"Could not unload dynamic library " + mName + ".  System Error: " + GetDllError());
		RB_ASSERT(0);
	}

	mHandle = nullptr;
}

void* RDynamicLibLinker::GetSymbol(const String& name) const
{
	if (!mHandle)
		return nullptr;

	return (void*)DYNLIB_GETSYM(mHandle, name.c_str());
}

String RDynamicLibLinker::GetDllError()
{
#if RB_PLATFORM == RB_PLATFORM_MSW
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	String ret((char*)lpMsgBuf);

	// Free the buffer.
	LocalFree(lpMsgBuf);
	return ret;
#else
	return String();
#endif
}

//----------------------------------------------------------------------------------------------------
// DLL Manager

static bool operator<(const UniquePtr<RDynamicLibLinker>& lhs, const String& rhs)
{
	return lhs->GetName() < rhs;
}

static bool operator<(const String& lhs, const UniquePtr<RDynamicLibLinker>& rhs)
{
	return lhs < rhs->GetName();
}

static bool operator<(const UniquePtr<RDynamicLibLinker>& lhs, const UniquePtr<RDynamicLibLinker>& rhs)
{
	return lhs->GetName() < rhs->GetName();
}

RDynamicLibManager::~RDynamicLibManager()
{
	for (auto dll : mLoadedLibraries)
	{
		dll.second->Unload();
		RB_DELETE(RDynamicLibLinker, dll.second);
	}
	mLoadedLibraries.clear();
}

RDynamicLibLinker* RDynamicLibManager::Load(const String& name)
{
	String filename = name;

	const String::size_type length = filename.length();
	const String extension = String(".") + RDynamicLibLinker::EXTENSION;
	const String::size_type extLength = extension.length();
	if (length <= extLength || filename.substr(length - extLength) != extension)
		filename.append(extension);

	if (RDynamicLibLinker::PREFIX != nullptr)
		filename.insert(0, RDynamicLibLinker::PREFIX);

	const auto& iterFind = mLoadedLibraries.find(name);
	if (iterFind != mLoadedLibraries.end())
	{
		return iterFind->second;
	}
	else
	{
		RDynamicLibLinker* newLib = RB_NEW_ARGS(RDynamicLibLinker, std::move(filename));
		if (!newLib->Load())
		{
			RB_DELETE(RDynamicLibLinker, newLib);
			newLib = nullptr;
		}

		mLoadedLibraries.insert(std::make_pair(name, newLib));
		return newLib;
	}
}

void RDynamicLibManager::Unload(const String& name)
{
	auto iterFind = mLoadedLibraries.find(name);
	if (iterFind != mLoadedLibraries.end())
	{
		RDynamicLibLinker* DLL = iterFind->second;
		DLL->Unload();
		mLoadedLibraries.erase(iterFind);
		RB_DELETE(RDynamicLibLinker, DLL);
		DLL = nullptr;
	}
}

RB_NAMESPACE_END