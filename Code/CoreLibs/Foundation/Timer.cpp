#include "Timer.h"

using namespace std::chrono;

RB_NAMESPACE_BEGIN

RTimer::RTimer()
{
	Reset();
	Record();
}

void RTimer::Reset()
{
	mStartTime = mHRClock.now();
}

void RTimer::Record()
{
	mRecordTime = mHRClock.now();
}

uint64 RTimer::GetSeconds() const
{
	auto newTime = mHRClock.now();
	duration<double> d = newTime - mStartTime;

	return duration_cast<seconds>(d).count();
}

uint64 RTimer::GetMilliSeconds() const
{
	auto newTime = mHRClock.now();
	duration<double> d = newTime - mStartTime;

	return duration_cast<milliseconds>(d).count();
}

uint64 RTimer::GetMicroSeconds() const
{
	auto newTime = mHRClock.now();
	duration<double> d = newTime - mStartTime;

	return duration_cast<microseconds>(d).count();
}

uint64 RTimer::GetStartMilliSeconds() const
{
	nanoseconds startTimeNs = mStartTime.time_since_epoch();

	return duration_cast<microseconds>(startTimeNs).count();
}

double RTimer::GetElapsedSecond() const
{
	auto CurrentTime = mHRClock.now();
	duration<double> TimeSpan = duration_cast<duration<double>>(CurrentTime - mRecordTime);
	return TimeSpan.count();
}

double RTimer::GetElapsedMilliSecond() const
{
	return GetElapsedSecond() * 1000.0;
}

double RTimer::GetElapsed() const
{
	return GetElapsedMilliSecond();
}

RB_NAMESPACE_END