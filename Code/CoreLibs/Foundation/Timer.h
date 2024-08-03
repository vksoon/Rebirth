#pragma once
#include "Prerequisites.h"
#include <chrono>

RB_NAMESPACE_BEGIN

class RB_CORE_API RTimer
{
public:
	RTimer();

	void Reset();

	void Record();

	uint64 GetSeconds() const;
	uint64 GetMilliSeconds() const;
	uint64 GetMicroSeconds() const;
	uint64 GetStartMilliSeconds() const;

	double GetElapsedSecond() const;
	double GetElapsedMilliSecond() const;
	double GetElapsed() const;

private:
	std::chrono::high_resolution_clock mHRClock;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> mRecordTime;
};

RB_NAMESPACE_END