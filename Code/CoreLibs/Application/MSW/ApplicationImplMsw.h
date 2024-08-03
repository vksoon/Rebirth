#pragma once
#include "Prerequisites.h"
#include "AppWindow.h"
#include "ApplicationMsw.h"

#include <Windows.h>

RB_NAMESPACE_BEGIN

class RDisplay;
class RApplication;
class RAppWindowImplMsw;

class RB_CORE_API RAppImplMsw : public RApplication
{
public:
	RAppImplMsw(RApplication* app, const RAppMsw::Settings& settings);
	~RAppImplMsw();

	void Run();
	void Quit();

	RApplication* GetApp() { return mApp; }
	float GetFrameRate() const { return mFrameRate; }
	void SetFrameRate(float frameRate);

	void DisableFrameRate() { mFrameRateEnabled = false; }
	bool IsFrameRateEnabled() const { return mFrameRateEnabled; }

	UINT32 GetNumWindows() const { return (UINT32)mWindows.size(); }
	RWindowRef GetWindowIndex(UINT32 index);
	RWindowRef GetForegroundWindow() const { return mForegroundWindow; }

	RWindowRef GetActiveWindow() const { return mActiveWindow; }
	void SetActiveWindow(RWindowRef window) { mActiveWindow = window; }

private:
	void Sleep(double seconds);
	RWindowRef CreateAppWindow(RAppWindow::Format format);
	void CloseWindow(RAppWindowImplMsw* windowImpl);
	void SetForegroundWindow(RWindowRef window) { mForegroundWindow = window; }

	bool SetupHasBeenCalled() const { return mSetupHasBeenCalled; }

private:
	RApplication* mApp;
	HINSTANCE mInstance;
	RWindowRef mActiveWindow;

	float mFrameRate;
	bool mSetupHasBeenCalled;
	bool mNeedsToRefreshDisplays;
	bool mActive;

	double mNextFrameTime;
	bool mFrameRateEnabled;
	bool mShouldQuit;
	bool mQuitOnLastWindowClosed;

	List<RAppWindowImplMsw*> mWindows;
	RWindowRef mForegroundWindow;

	friend RAppMsw;
	friend RAppWindowImplMsw;
	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

class RB_CORE_API RAppWindowImplMsw
{
public:
	RAppWindowImplMsw(const RAppWindow::Format& format, RAppImplMsw* appImpl);
	~RAppWindowImplMsw() {}

	bool IsFullScreen() { return mFullScreen; }
	void SetFullScreen(bool bFullScreen);

	int32 GetWidth() const { return mWindowWidth; }
	int32 GetHeight() const { return mWindowHeight; }
	float GetDPI() const { return mDPI; }
	void SetSize(int32 width, int32 height);

	int32 GetPosX() const { return mWindowedPosX; }
	int32 GetPosY() const { return mWindowedPosY; }
	void SetPos(int32 x, int32 y);

	void Close();
	String GetTitle() const;
	void SetTitle(const String& title);

	void Hide();
	void Show();
	bool IsHidden() const;

	bool IsBorderless() const { return mBorderless; }
	void SetBorderless(bool borderless);
	bool IsAlwaysOnTop() const { return mAlwaysOnTop; }
	void SetAlwaysOnTop(bool alwaysOnTop);

	RAppImplMsw* GetAppImpl() { return mAppImpl; }
	RWindowRef GetWindow() { return mWindow; }
	virtual void Resize();

	HWND GetWindowHandle() const { return mWnd; }
	HDC	GetDC() const { return mDC; }
	void PrivateClose();

private:
	void SetWindowStyleValues();
	void CreateAppWindow(int32 width, int32 height, const String& title);
	void CompleteCreation();
	static void RegisterWindowClass();
	void GetScreenSize(int32 clientWidth, int32 clientHeight, int32* resultWidth, int32* resultHeight);
	virtual void ToggleFullScreen();

private:
	RAppImplMsw* mAppImpl;
	RWindowRef mWindow;
	RDisplay* mDisplay;
	HWND mWnd;
	HDC mDC;
	DWORD mWindowStyle;
	DWORD mWindowExStyle;
	int32 mWindowOffsetX;
	int32 mWindowOffsetY;
	bool mHidden;
	int32 mWindowWidth;
	int32 mWindowHeight;
	float mDPI;
	bool mFullScreen;
	bool mBorderless;
	bool mAlwaysOnTop;
	bool mResizable;
	int32 mWindowedPosX;
	int32 mWindowedPosY;
	Vec2i mWindowedSizePx;
	bool mIsDragging;

	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

RB_NAMESPACE_END