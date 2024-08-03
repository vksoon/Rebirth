#include "ApplicationMsw.h"
#include "ApplicationImplMsw.h"

RB_NAMESPACE_BEGIN

RAppMsw::RAppMsw()
{
	mImpl = RB_New<RAppImplMsw>(this, StartUpSettings);
}

RAppMsw::~RAppMsw()
{
	if (mImpl)
	{
		RB_Delete(mImpl);
		mImpl = nullptr;
	}
}

void RAppMsw::Launch()
{
	mImpl->Run();
}

RWindowRef RAppMsw::CreateAppWindow(const RAppWindow::Format& format)
{
	return mImpl->CreateAppWindow(format);
}

void RAppMsw::Quit()
{
	if (!GetLaunchCalled())
		SetQuitRequested();
	else
		mImpl->Quit();
}

float RAppMsw::GetFrameRate() const
{
	return mImpl->GetFrameRate();
}

void RAppMsw::SetFrameRate(float frameRate)
{
	mImpl->SetFrameRate(frameRate);
}

void RAppMsw::DisableFrameRate()
{
	mImpl->DisableFrameRate();
}

bool RAppMsw::IsFrameRateEnabled() const
{
	return mImpl->IsFrameRateEnabled();
}

RWindowRef RAppMsw::GetActiveWindow() const
{
	return mImpl->GetActiveWindow();
}

RB_NAMESPACE_END