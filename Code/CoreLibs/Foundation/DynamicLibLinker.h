#pragma once

#include "Prerequisites.h"
#include "GlobalInstance.h"

#if RB_PLATFORM == RB_PLATFORM_MSW
struct HINSTANCE__;
typedef struct HINSTANCE__* HINSTANCE;
#endif

RB_NAMESPACE_BEGIN

#if RB_PLATFORM == RB_PLATFORM_MSW
#    define DYNLIB_HANDLE HINSTANCE
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) FreeLibrary( a )
#endif

class RB_CORE_API RDynamicLibLinker final
{
public:
	RDynamicLibLinker(const String& name);
	~RDynamicLibLinker();

	bool Load();
	void Unload();

	const String& GetName() const { return mName; };

	void* GetSymbol(const String& name) const;

private:
	String GetDllError();

private:
	const String mName;
	DYNLIB_HANDLE mHandle;

public:
#if RB_PLATFORM == RB_PLATFORM_MSW
	static constexpr const char* EXTENSION = "dll";
	static constexpr const char* PREFIX = nullptr;
#endif
};

class RB_CORE_API RDynamicLibManager : public GlobalInstance<RDynamicLibManager>
{
public:
	RDynamicLibManager() = default;
	virtual ~RDynamicLibManager();

	RDynamicLibLinker* Load(const String& name);

	void Unload(const String& name);

protected:
	Map<String, RDynamicLibLinker*> mLoadedLibraries;
};

RB_NAMESPACE_END