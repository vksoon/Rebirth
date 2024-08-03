#pragma once

#include "Prerequisites.h"
#include "AppWindow.h"
#include "AppMouseEvent.h"
#include "AppKeyboardEvent.h"

RB_NAMESPACE_BEGIN

class RB_CORE_API RApplication
{
public:
	class RB_CORE_API Settings
	{
	public:
		Settings() = default;
		virtual ~Settings() = default;

		void SetDefaultWindowFormat(const RAppWindow::Format& format) { mDefaultWindowFormat = format; }
		RAppWindow::Format GetDefaultWindowFormat() const { return mDefaultWindowFormat; }

		void SetWindowSize(int32 width, int32 height) { mDefaultWindowFormat.SetSize(width, height); }
		int32 GetWindowWidth() const { return mDefaultWindowFormat.GetWidth(); }
		int32 GetWindowHeight() const { return mDefaultWindowFormat.GetHeight(); }

		void SetWindowPos(int32 windowPosX, int32 windowPosY) { mDefaultWindowFormat.SetPos(windowPosX, windowPosY); }
		int32 GetWindowPosX() const { return mDefaultWindowFormat.GetPosX(); }
		int32 GetWindowPosY() const { return mDefaultWindowFormat.GetPosY(); }

		void SetWindowPosUnspecified() { mDefaultWindowFormat.SetPosUnspecified(); }
		bool IsWindowPosSpecified() const { return mDefaultWindowFormat.IsPosSpecified(); }

		void SetQuitOnLastWindowCloseEnabled(bool enable = true) { mQuitOnLastWindowClose = enable; }
		bool IsQuitOnLastWindowCloseEnabled() const { return mQuitOnLastWindowClose; }

		void SetFullScreen(bool fullScreen = true) { mDefaultWindowFormat.SetFullScreen(fullScreen); }
		bool IsFullScreen() const { return mDefaultWindowFormat.IsFullScreen(); }

		void SetResizable(bool resizable = true) { mDefaultWindowFormat.SetResizable(resizable); }
		bool IsResizable() const { return mDefaultWindowFormat.IsResizable(); }

		void SetBorderless(bool borderless = true) { mDefaultWindowFormat.SetBorderless(borderless); }
		bool IsBorderless() const { return mDefaultWindowFormat.IsBorderless(); }

		void SetAlwaysOnTop(bool alwaysOnTop = true) { mDefaultWindowFormat.SetAlwaysOnTop(alwaysOnTop); }
		bool IsAlwaysOnTop() const { return mDefaultWindowFormat.IsAlwaysOnTop(); }

		void SetTitle(const String& title) { mTitle = title; mDefaultWindowFormat.SetTitle(title); }
		const String& GetTitle() const { return mTitle; }

		void SetFrameRate(float frameRate) { mFrameRate = frameRate; }
		float GetFrameRate() const { return mFrameRate; }

		void EnableFrameRate() { mFrameRateEnabled = true; }
		void DisableFrameRate() { mFrameRateEnabled = false; }
		bool IsFrameRateEnabled() const { return mFrameRateEnabled; }

		void SetShouldQuit(bool shouldQuit = true) { mShouldQuit = shouldQuit; }
		bool ShouldQuit() const { return mShouldQuit; }

		void SetRenderHardwareKernelPlugin(const String& pluginName) { mRenderHardwareKernelPlugin = pluginName; }
		const String& GetRenderHardwareKernelPlugin() const { return mRenderHardwareKernelPlugin; }

		void SetShaderCompilerPlugin(const String& pluginName) { mShaderCompilerPlugin = pluginName; }
		const String& GetShaderCompilerPlugin() const { return mShaderCompilerPlugin; }

	protected:
		RAppWindow::Format mDefaultWindowFormat;
		String			  mTitle;
		bool			  mFrameRateEnabled = true;
		float			  mFrameRate = 60;
		bool			  mQuitOnLastWindowClose = true;
		bool			  mShouldQuit = false;

		// Plugins
		String			  mRenderHardwareKernelPlugin;
		String			  mShaderCompilerPlugin;

		friend RApplication;
	};

public:
	RApplication();
	~RApplication();

	virtual void Setup();
	virtual void Update(float DeltaTime);

	virtual void MouseDown(RMouseEvent event) {}
	virtual void MouseUp(RMouseEvent event) {}
	virtual void MouseWheel(RMouseEvent event) {}
	virtual void MouseMove(RMouseEvent event) {}
	virtual void MouseDrag(RMouseEvent event) {}
	virtual void KeyDown(RKeyboardEvent event) {}
	virtual void KeyUp(RKeyboardEvent event) {}
	virtual void Resize();
	virtual void CleanUp() {}
	virtual void Quit();

	RAppWindow* CreateAppWindow(const RAppWindow::Format& format = RAppWindow::Format());
	RAppWindow* GetActiveWindow() const;

	float GetFrameRate() const;
	void SetFrameRate(float frameRate);
	void DisableFrameRate();
	bool IsFrameRateEnabled() const;
	float GetAverageFps() const { return mAverageFps; }
	double GetFpsSampleInterval() const { return mFpsSampleInterval; }
	void SetFpsSampleInterval(double sampleInterval) { mFpsSampleInterval = sampleInterval; }
	bool IsFullScreen() const { return GetActiveWindow()->IsFullScreen(); }
	void SetFullScreen(bool fullScreen) { GetActiveWindow()->SetFullScreen(fullScreen); }

	double GetElapsedSeconds() const;
	uint32 GetElapsedFrames() const { return mFrameCount; }

	bool GetQuitRequested() const { return mQuitRequested; }
	void SetQuitRequested() { mQuitRequested = true; }

	void PrivateSetup();
	void PrivateUpdate();

	// Launcher
	void Launch();

	bool GetLaunchCalled() const { return mLaunchCalled; }
	void ExecuteLaunch();

	void CreateWindow(RAppWindow::Format Format);
	void CloseWindow();

public:
	static void	PrepareLaunch();
	static void	SetupLaunch(Settings* settings, const char* title, INT32 argc, char* const argv[]);
	static void CleanupLaunch();

private:
	static Settings* StartUpSettings;



protected:
	class RTimer* mTimer;
	uint32 mFrameCount;
	float mAverageFps;
	uint32 mFpsLastSampleFrame;
	double mFpsLastSampleTime;
	double mFpsSampleInterval;
	bool mLaunchCalled;
	bool mQuitRequested;
};

typedef std::function<void(RApplication::Settings* settings)> RAppSettingsCallback;

RB_NAMESPACE_END