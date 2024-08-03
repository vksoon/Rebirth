#pragma once
#include "Prerequisites.h"
#include "RenderHardwareKernel.h"

RB_NAMESPACE_BEGIN

class RRenderHardwareKernel;
class RRenderHardwareKernelFactory;

class RB_CORE_API RRenderHardwareKernelManager
{
public:
	RRenderHardwareKernelManager();
	~RRenderHardwareKernelManager();

	bool Initialize(const String& RenderHarwareKernelPlugin);
	RRenderHardwareKernel* GetRenderHardwareKernel() { return mRenderHardwareKernel; }

private:
	bool mIsInitialized;
	EGraphicsAPI mGraphicsAPI;
	RRenderHardwareKernelFactory* mAvailableFactory;
	RRenderHardwareKernel* mRenderHardwareKernel;
};

RB_NAMESPACE_END