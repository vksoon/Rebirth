#pragma once
#include "Prerequisites.h"

RB_NAMESPACE_BEGIN

class RRenderHardwareKernel;

class RB_CORE_API RRenderHardwareKernelFactory
{
public:
	virtual ~RRenderHardwareKernelFactory() = default;
	virtual RRenderHardwareKernel* CreateRenderHardwareKernel() = 0;
	virtual const char* GetName() const = 0;
};

RB_NAMESPACE_END