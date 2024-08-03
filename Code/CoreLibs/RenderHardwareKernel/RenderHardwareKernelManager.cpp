#include "RenderHardwareKernelManager.h"
#include "Exceptions.h"
#include "DynamicLibLinker.h"
#include "RenderHardwareKernel.h"
#include "RenderHardwareKernelFactory.h"

RB_NAMESPACE_BEGIN

RRenderHardwareKernelManager::RRenderHardwareKernelManager()
	: mIsInitialized(false)
	, mGraphicsAPI(EGraphicsAPI::MAX_NUM)
	, mAvailableFactory(nullptr)
	, mRenderHardwareKernel(nullptr)
{
}

RRenderHardwareKernelManager::~RRenderHardwareKernelManager()
{
	if (mRenderHardwareKernel)
	{
		RB_Delete<RRenderHardwareKernel>(mRenderHardwareKernel);
		mRenderHardwareKernel = nullptr;
	}

	if (mAvailableFactory)
	{
		RB_Delete<RRenderHardwareKernelFactory>(mAvailableFactory);
		mAvailableFactory = nullptr;
	}
}

bool RRenderHardwareKernelManager::Initialize(const String& RenderHarwareKernelPlugin)
{
	if (mIsInitialized)
	{
		return false;
	}

	const char* PluginNameFound = "";
	RDynamicLibLinker* LoadedDLL = RDynamicLibManager::Ref().Load(RenderHarwareKernelPlugin);
	if (LoadedDLL == nullptr)
	{
		return false;
	}

	typedef RRenderHardwareKernelFactory* (*FnCreateRenderHardwareKernelFactory)();

	FnCreateRenderHardwareKernelFactory RHKCreateFactory = (FnCreateRenderHardwareKernelFactory)LoadedDLL->GetSymbol("CreateRenderHardwareKernelFactory");
	if (RHKCreateFactory == nullptr)
	{
		return false;
	}

	mAvailableFactory = RHKCreateFactory();
	if (mAvailableFactory == nullptr)
	{
		return false;
	}

	mRenderHardwareKernel = mAvailableFactory->CreateRenderHardwareKernel();
	if (mRenderHardwareKernel == nullptr)
	{
		return false;
	}

	return true;
}

RB_NAMESPACE_END