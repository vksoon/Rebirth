//------------------------------------------------------------
// Copyright (c) 2021 Eric Kim								  
//------------------------------------------------------------
#pragma once

// Platform-specific stuff
#include "PlatformDefines.h"

#if RB_COMPILER == RB_COMPILER_MSVC

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#	pragma warning (disable: 4251)
#	pragma warning(disable: 4996)
#	pragma warning(disable: 4503)
#	pragma warning(disable: 4530)

#endif

#if RB_DEBUG_MODE
#	define RB_DEBUG_ONLY(x) x
#	define RB_ASSERT(x) assert(x)
#	define RB_MEM_USE_TRACE 0
#else
#	define RB_DEBUG_ONLY(x)
#	define RB_ASSERT(x)
#	define RB_MEM_USE_TRACE 0
#endif

#define RB_NAMESPACE_BEGIN namespace Rebirth {
#define RB_NAMESPACE_END };

#define RB_NAMESPACE_BEGIN_EX(NS) namespace Rebirth { namespace NS {
#define RB_NAMESPACE_END_EX }; };

#define RB_NAME(name) name
#define RB_STRUCT(name) struct name
#define RB_UNION(name) union name
#define RB_FORWARD_STRUCT(name) struct R##name
#define RB_REF(arg) arg&
#define RB_REF_ACCESS(arg) (&arg)
#define RB_DEFAULT_VALUE(arg) = arg
#define RB_ZERO_INIT {}
#define RB_SET_BIT(index) (1 << (index))

// Short-hand names for various built-in types
#include "Types.h"

// Standard memory allocators
#include "MemoryAllocationBase.h"

// Commonly used standard headers
#include "StandardHeaders.h"

#include "StringBase.h"
#include "MathLibs.h"
