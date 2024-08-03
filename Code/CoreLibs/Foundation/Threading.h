#pragma once
#include "Prerequisites.h"

#define RB_AUTO_MUTEX_NAME mutex

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

RB_NAMESPACE_BEGIN

#define BP_THREAD_HARDWARE_CONCURRENCY std::thread::hardware_concurrency()
#define BP_THREAD_CURRENT_ID std::this_thread::get_id()
#define BP_THREAD_SLEEP(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms));

/** Wrapper for the C++ std::mutex. */
using Mutex = std::mutex;

/** Wrapper for the C++ std::recursive_mutex. */
using RecursiveMutex = std::recursive_mutex;

/** Wrapper for the C++ std::condition_variable. */
using Signal = std::condition_variable;

/** Wrapper for the C++ std::thread. */
using Thread = std::thread;

/** Wrapper for the C++ std::thread::id. */
using ThreadId = std::thread::id;

/** Wrapper for the C++ std::unique_lock<std::mutex>. */
using Lock = std::unique_lock<Mutex>;

/** Wrapper for the C++ std::unique_lock<std::recursive_mutex>. */
using RecursiveLock = std::unique_lock<RecursiveMutex>;

/** Wrapper for the C++ std::scoped_lock<std::mutex>. */
using ScopedLock = std::scoped_lock<Mutex>;

RB_NAMESPACE_END