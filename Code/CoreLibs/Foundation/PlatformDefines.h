//------------------------------------------------------------
// Copyright (c) 2021 Eric Kim								  
//------------------------------------------------------------
#pragma once

// Microsoft Window Platform 
#define RB_PLATFORM_MSW				1
// Universal Window platform
#define RB_PLATFORM_UWP				2

#define RB_COMPILER_MSVC			1

#define RB_ARCHITECTURE_x86_32		1
#define RB_ARCHITECTURE_x86_64		2

#define RB_ENDIAN_LITTLE			1
#define RB_ENDIAN_BIG				2
#define RB_ENDIAN RB_ENDIAN_LITTLE

#if defined(_MSC_VER) 
#	define RB_COMPILER RB_COMPILER_MSVC
#	define RB_COMP_VER _MSC_VER
#	define RB_THREADLOCAL __declspec(thread)
#	define RB_STDCALL __stdcall
#	define RB_CDECL __cdecl
#	define RB_FALLTHROUGH
#	undef __PRETTY_FUNCTION__
#	define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#	pragma error "No known compiler. "
#endif

// Find the platform
#if defined( __WIN32__ ) || defined( _WIN32 )
#	define RB_PLATFORM RB_PLATFORM_MSW
#endif

// Find the architecture type
#if defined(__x86_64__) || defined(_M_X64)
#	define RB_ARCH_TYPE RB_ARCHITECTURE_x86_64
#else
#	define RB_ARCH_TYPE RB_ARCHITECTURE_x86_32
#endif

// DLL export
#ifndef _RB_STATICLIB_
#	if defined(_RB_CORE_EXPORT_)
#		if RB_COMPILER == RB_COMPILER_MSVC
#			define RB_CORE_EXTERN_API extern "C" __declspec(dllexport)
#			define RB_CORE_API __declspec(dllexport)
#		endif
#	else 
#		if RB_COMPILER == RB_COMPILER_MSVC
#			define RB_CORE_EXTERN_API extern "C" __declspec(dllimport)
#			define RB_CORE_API __declspec(dllimport)
#		endif
#endif 
#else 
#	define RB_CORE_EXTERN_API 
#	define RB_CORE_API 
#endif

// DLL export for plugins
#if RB_PLATFORM == RB_PLATFORM_MSW 
#	if RB_COMPILER == RB_COMPILER_MSVC
#		define RB_PLUGIN_EXTERN_API extern "C" __declspec(dllexport)
#		define RB_PLUGIN_EXPORT __declspec(dllexport)
#	else
#		define RB_PLUGIN_EXPORT __attribute__ ((dllexport))
#	endif
#else // Linux/Mac settings
#	define RB_PLUGIN_EXPORT __attribute__ ((visibility ("default")))
#endif

// Windows Settings
#if RB_PLATFORM == RB_PLATFORM_MSW
#	if defined(_DEBUG) || defined(DEBUG)
#		define RB_DEBUG_MODE 1
#	else
#		define RB_DEBUG_MODE 0
#	endif
#endif

#ifdef _WIN32
#define RB_CALL __fastcall
#else
#define RB_CALL
#endif

