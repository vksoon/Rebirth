#pragma once
#include "Prerequisites.h"
#include "AppKeyboardEvent.h"
#include "AppMouseEvent.h"

RB_NAMESPACE_BEGIN

#if RB_PLATFORM == RB_PLATFORM_MSW
class RAppWindowImplMsw;
#endif

class RDisplay;
class RApplication;

class RB_CORE_API RAppWindow
{
public:
	struct Format
	{
		Format(bool bFullscreen = false, RDisplay* Display = nullptr, int32 width = 1280, int32 height = 720, int32 posX = 0, int32 posY = 0)
			: mFullScreen(bFullscreen)
			, mDisplay(Display)
			, mWidth(width)
			, mHeight(height)
			, mPosX(posX)
			, mPosY(posY)
			, mPosSpecified(false)
			, mResizable(true)
			, mBorderless(false)
			, mAlwaysOnTop(false)
			, mFullScreenButtonEnabled(false)
			, mTitleSpecified(false)
			, mTitle("")
		{}

		bool IsFullScreen() const { return mFullScreen; }
		void SetFullScreen(bool bFullScreen) { mFullScreen = bFullScreen; }

		RDisplay* GetDisplay() const { return mDisplay; }

		int32 GetWidth() const { return mWidth; }
		int32 GetHeight() const { return mHeight; }
		void SetSize(int32 width, int32 height) { mWidth = width; mHeight = height; }

		int32 GetPosX() const { return mPosX; }
		int32 GetPosY() const { return mPosY; }
		void SetPos(int32 X, int32 Y) { mPosX = X; mPosY = Y; mPosSpecified = true; }
		bool IsPosSpecified() const { return mPosSpecified; }
		void SetPosUnspecified() { mPosSpecified = false; }

		bool IsResizable() const { return mResizable; }
		void SetResizable(bool resizable = true) { mResizable = resizable; }

		bool IsBorderless() const { return mBorderless; }
		void SetBorderless(bool borderless = true) { mBorderless = borderless; }

		bool IsAlwaysOnTop() const { return mAlwaysOnTop; }
		void SetAlwaysOnTop(bool alwaysOnTop = true) { mAlwaysOnTop = alwaysOnTop; }

		bool IsFullScreenButtonEnabled() const { return mFullScreenButtonEnabled; }
		void EnableFullScreenButton(bool enabled = true) { mFullScreenButtonEnabled = enabled; }

		const String& GetTitle() const { return mTitle; }
		void SetTitle(const String& title) { mTitle = title; mTitleSpecified = true; }
		bool IsTitleSpecified() const { return mTitleSpecified; }
		void SetTitleUnspecified() { mTitleSpecified = false; }

	private:
		bool		mFullScreen;
		RDisplay*	mDisplay;
		int32		mWidth;
		int32		mHeight;
		int32		mPosX;
		int32		mPosY;
		bool		mPosSpecified;
		bool		mResizable;
		bool		mBorderless;
		bool		mAlwaysOnTop;
		bool		mFullScreenButtonEnabled;
		String		mTitle;
		bool		mTitleSpecified;
	};

	RAppWindow()
		: mApp(nullptr)
		, mImpl(nullptr)
	{
	}

	virtual ~RAppWindow() = default;

	RApplication* GetApp() const { return mApp; }

	void* GetWindowHandle() const;

	bool IsFullScreen() const;
	void SetFullScreen(bool fullScreen = true);

	int32 GetWidth() const;
	int32 GetHeight() const;
	float GetDPI() const;
	float GetAspectRatio() const { return GetWidth() / (float)GetHeight(); }
	void SetSize(int32 width, int32 height);

	int32 GetPosX() const;
	int32 GetPosY() const;
	void SetPos(int32 x, int32 y);

	float GetCenterX() const { return (float)GetWidth() / 2.0f; }
	float GetCenterY() const { return (float)GetHeight() / 2.0f; }

	String GetTitle() const;
	void SetTitle(const String& title);

	bool IsBorderless() const;
	void SetBorderless(bool borderless = true);
	bool IsAlwaysOnTop() const;
	void SetAlwaysOnTop(bool alwaysOnTop = true);

	void Hide();
	void Show();
	bool IsHidden() const;

	void Close();

public:
	void EmitResize();
	void EmitClose();

public:
#if RB_PLATFORM == RB_PLATFORM_MSW
	static RAppWindow* PrivateCreate(RAppWindowImplMsw* impl, RApplication* app)
#endif
	{
		RAppWindow* ret = RB_New<RAppWindow>();
		ret->SetImpl(impl);
		ret->SetApp(app);

		return ret;
	}

protected:
	void SetApp(RApplication* app) { mApp = app; }

protected:
	RApplication* mApp;

private:
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