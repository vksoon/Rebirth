#pragma once
#include "Prerequisites.h"
#include "Application.h"
#include "ApplicationSupports.h"
#include "AppWindow.h"

RB_NAMESPACE_BEGIN

class RAppImplMsw;
class RDisplay;
class RApplication;
class RAppWindowImplMsw;

class RB_CORE_API RAppMsw : public RApplication 
{
public :

public :
	RAppMsw();
	virtual ~RAppMsw();

	virtual RAppWindow* CreateAppWindow(const RAppWindow::Format& format = RAppWindow::Format()) override;
	virtual RAppWindow* GetActiveWindow() const override;
	virtual void Quit() override;

	virtual float GetFrameRate() const override;
	virtual void SetFrameRate(float frameRate) override;
	virtual void DisableFrameRate() override;
	virtual bool IsFrameRateEnabled() const override;

private:
	virtual void Launch() override;

private:
	RApplication* mApp;
	HINSTANCE mInstance;
	RAppWindow* mActiveWindow;

	float mFrameRate;
	bool mSetupHasBeenCalled;
	bool mNeedsToRefreshDisplays;
	bool mActive;

	double mNextFrameTime;
	bool mFrameRateEnabled;
	bool mShouldQuit;
	bool mQuitOnLastWindowClosed;

	List<RAppWindowImplMsw*> mWindows;

public:
	template<typename _AppType>
	static void Main(const char* title, const AppSettings& settings = AppSettings());
};

template<typename _AppType>
void RAppMsw::Main(const char* title, const AppSettings& settingsFunc)
{
	RApplication::PrepareLaunch();

	Settings entrySettings;
	RApplication::SetupLaunch(&entrySettings, title, 0, nullptr);

	if (settingsFunc)
		settingsFunc(&entrySettings);

	if (entrySettings.ShouldQuit())
		return;

	RAppMsw* app = static_cast<RAppMsw*>(RB_New<_AppType>());
	app->ExecuteLaunch();

	RB_Delete(app);

	RApplication::CleanupLaunch();
}

RB_NAMESPACE_END

#define RB_APP_LAUNCH_MSW( APP, Title, ... )																					\
int32_t CALLBACK WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ) \
{																														\
	Rebirth::RAppMsw::Main<APP>( Title, ##__VA_ARGS__ );																\
	return 0;																											\
}