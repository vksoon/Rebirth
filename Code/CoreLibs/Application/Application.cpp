#include "Application.h"
#include "DynamicLibLinker.h"
#include "Timer.h"
#include "Log.h"

#include "GLFW/glfw3.h"

RB_NAMESPACE_BEGIN

RApplication::Settings* RApplication::StartUpSettings;

/// <summary>
/// Launcher
/// </summary>

void RApplication::PrepareLaunch()
{
	RDynamicLibManager::StartUp();
}

void RApplication::SetupLaunch(Settings* settings, const char* title, INT32 argc, char* const argv[])
{
	if (settings->GetTitle().empty())
	{
		settings->SetTitle(title);
	}

	StartUpSettings = settings;
	RDebug::GetOrCreate();
}

void RApplication::ExecuteLaunch()
{
	try
	{
		if (mQuitRequested)
			return;

		mLaunchCalled = true;
		Launch();
	}
	catch (std::exception& exc)
	{
		throw;
	}
}

void RApplication::CleanupLaunch()
{
	RDebug::Terminate();
}


RApplication::RApplication()
	: mTimer(RB_New<RTimer>())
	, mFrameCount(0)
	, mAverageFps(0)
	, mFpsSampleInterval(1)
	, mFpsLastSampleFrame(0)
	, mFpsLastSampleTime(0)
	, mLaunchCalled(false)
	, mQuitRequested(false)
{
}

RApplication::~RApplication()
{
	if (mTimer != nullptr)
	{
		RB_Delete<RTimer>(mTimer);
		mTimer = nullptr;
	}
}

void RApplication::Setup()
{
}

void RApplication::PrivateSetup()
{
	Setup();

	DisableFrameRate();
	SetFpsSampleInterval(1);
}

void RApplication::PrivateUpdate()
{
	mFrameCount++;

	float DeltaTime = float(std::max(0.0, mTimer->GetElapsed() * 0.001));
	mTimer->Record();

	Update(DeltaTime);

	static const int32 DeltaTimeCount = 20;
	static float DeltaTimes[DeltaTimeCount];
	DeltaTimes[mFrameCount % DeltaTimeCount] = DeltaTime;
	float AvgDeltaTime = DeltaTime;
	if (mFrameCount > DeltaTimeCount)
	{
		float AvgTime = 0;
		for (int32 i = 0; i < DeltaTimeCount; ++i)
		{
			AvgTime += DeltaTimes[i];
		}
		AvgDeltaTime = AvgTime / DeltaTimeCount;
	}
	mAverageFps = 1.0f / AvgDeltaTime;

	//double Now = (double)mTimer->GetMicroSeconds();
	//if (Now > mFpsLastSampleTime + mFpsSampleInterval)
	//{
	//	//calculate average Fps over sample interval
	//	uint32 FramesPassed = mFrameCount - mFpsLastSampleFrame;
	//	mAverageFps = (float)(FramesPassed / (Now - mFpsLastSampleTime));

	//	mFpsLastSampleTime = Now;
	//	mFpsLastSampleFrame = mFrameCount;
	//}
}

void RApplication::Update(float DeltaTime)
{
}

double RApplication::GetElapsedSeconds() const
{
	return (double)mTimer->GetElapsedSecond();
}

void RApplication::Resize()
{

}

void RApplication::CreateWindow(RAppWindow::Format Format)
{
}

RB_NAMESPACE_END