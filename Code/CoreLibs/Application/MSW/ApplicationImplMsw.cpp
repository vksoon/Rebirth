#include "ApplicationImplMsw.h"
#include "Application.h"
#include "Display.h"

#include <windowsx.h>
#include <CommDlg.h>
#include <ShellAPI.h>
#include <Shlobj.h>

#define LOSHORT(l)           ((SHORT)(l))
#define HISHORT(l)           ((SHORT)(((DWORD)(l) >> 16) & 0xFFFF))

RB_NAMESPACE_BEGIN

LRESULT CALLBACK WndProc(HWND mWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static const char* WINDOWED_WIN_CLASS_NAME = TEXT("RebirthWndClass");

RAppImplMsw::RAppImplMsw(RApplication* app, const RAppMsw::Settings& settings)
	: mApp(app)
	, mSetupHasBeenCalled(false)
	, mActive(true)
	, mNeedsToRefreshDisplays(false)
	, mShouldQuit(false)
{
	mFrameRate = settings.GetFrameRate();
	mFrameRateEnabled = settings.IsFrameRateEnabled();
	mQuitOnLastWindowClosed = settings.IsQuitOnLastWindowCloseEnabled();

	auto format = settings.GetDefaultWindowFormat();
	if (!format.IsTitleSpecified())
		format.SetTitle(settings.GetTitle());

	CreateAppWindow(format);
}

RAppImplMsw::~RAppImplMsw()
{
}

void RAppImplMsw::Run()
{
	mApp->PrivateSetup();
	mSetupHasBeenCalled = true;

	for (auto& window : mWindows)
		window->Resize();

	// initialize our next frame time
	mNextFrameTime = mApp->GetElapsedSeconds();

	// inner loop
	while (!mShouldQuit)
	{
		// all of our Windows will have marked this as true if the user has unplugged, plugged or modified a Monitor
		if (mNeedsToRefreshDisplays)
		{
			mNeedsToRefreshDisplays = false;
			for (auto& window : mWindows)
				window->Resize();
		}

		// update and draw
		mApp->PrivateUpdate();

		// get current time in seconds
		double currentSeconds = mApp->GetElapsedSeconds();

		// calculate time per frame in seconds
		double secondsPerFrame = 1.0 / mFrameRate;

		// determine if application was frozen for a while and adjust next frame time
		double elapsedSeconds = currentSeconds - mNextFrameTime;
		if (elapsedSeconds > 1.0)
		{
			INT32 numSkipFrames = (INT32)(elapsedSeconds / secondsPerFrame);
			mNextFrameTime += (numSkipFrames * secondsPerFrame);
		}

		// determine when next frame should be drawn
		mNextFrameTime += secondsPerFrame;

		// sleep and process messages until next frame
		if ((mFrameRateEnabled) && (mNextFrameTime > currentSeconds))
			Sleep(mNextFrameTime - currentSeconds);
		else
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
}


void RAppImplMsw::Sleep(double seconds)
{
	// create waitable timer
	static HANDLE timer = ::CreateWaitableTimer(NULL, FALSE, NULL);

	// specify relative wait time in units of 100 nanoseconds
	LARGE_INTEGER waitTime;
	waitTime.QuadPart = (LONGLONG)(seconds * -10000000);
	if (waitTime.QuadPart >= 0)
		return;

	// activate waitable timer
	if (!::SetWaitableTimer(timer, &waitTime, 0, NULL, NULL, FALSE))
		return;

	// handle events until specified time has elapsed
	DWORD result;
	MSG msg;
	while (!mShouldQuit)
	{
		result = ::MsgWaitForMultipleObjects(1, &timer, false, INFINITE, QS_ALLINPUT);
		if (result == (WAIT_OBJECT_0 + 1))
		{
			// execute messages as soon as they arrive
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			// resume waiting
		}
		else return; // time has elapsed
	}
}

RWindowRef RAppImplMsw::CreateAppWindow(RAppWindow::Format format)
{
	mWindows.push_back(RB_New<RAppWindowImplMsw>(format, this));

	if (mSetupHasBeenCalled)
		mWindows.back()->GetWindow()->EmitResize();

	return mWindows.back()->GetWindow();
}

void RAppImplMsw::CloseWindow(RAppWindowImplMsw* windowImpl)
{
	auto winIt = find(mWindows.begin(), mWindows.end(), windowImpl);
	if (winIt != mWindows.end())
	{
		windowImpl->GetWindow()->EmitClose();
		windowImpl->PrivateClose();
		RB_Delete<RAppWindowImplMsw>(windowImpl);
		mWindows.erase(winIt);
	}

	if (mWindows.empty() && mQuitOnLastWindowClosed)
		mShouldQuit = true;
}

RWindowRef RAppImplMsw::GetWindowIndex(UINT32 index)
{
	if (index >= mWindows.size())
		return RWindowRef();

	auto winIt = mWindows.begin();
	std::advance(winIt, index);
	return (*winIt)->GetWindow();
}

void RAppImplMsw::Quit()
{
	mShouldQuit = true;
}

void RAppImplMsw::SetFrameRate(float frameRate)
{
	mFrameRate = frameRate;
	mFrameRateEnabled = true;
	mNextFrameTime = mApp->GetElapsedSeconds();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RAppWindowImplMsw::RAppWindowImplMsw(const RAppWindow::Format& format, RAppImplMsw* appImpl)
	: mAppImpl(appImpl)
	, mHidden(false)
{
	mFullScreen = format.IsFullScreen();
	mDisplay = format.GetDisplay();
	mResizable = format.IsResizable();
	mAlwaysOnTop = format.IsAlwaysOnTop();
	mBorderless = format.IsBorderless();

	mWindowWidth = format.GetWidth();
	mWindowHeight = format.GetHeight();

	mWindowOffsetX = mWindowedPosX = format.GetPosX();
	mWindowOffsetY = mWindowedPosY = format.GetPosY();

	CreateAppWindow(mWindowWidth, mWindowHeight, format.GetTitle());
	mWindow = RAppWindow::PrivateCreate(this, mAppImpl->GetApp());
	CompleteCreation();
}

void RAppWindowImplMsw::SetWindowStyleValues()
{
	if (mFullScreen)
	{
		mWindowExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		mWindowStyle = WS_POPUP;										// Windows Style
	}
	else if (mBorderless)
	{
		mWindowExStyle = WS_EX_APPWINDOW;
		mWindowStyle = WS_POPUP;
	}
	else
	{
		mWindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES;				// Window Extended Style
		mWindowStyle = (mResizable) ? WS_OVERLAPPEDWINDOW
			: (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);	// Windows Style
	}
}

void RAppWindowImplMsw::CreateAppWindow(int32 width, int32 height, const String& title)
{
	RDisplay::RefreshDisplays();
	mDisplay = RDisplay::FindDisplayFromHmonitor(::MonitorFromWindow(mWnd, MONITOR_DEFAULTTONEAREST));

	RECT windowRect = {};
	RArea DisplayArea = mDisplay->GetBounds();

	if (mFullScreen)
	{
		windowRect.left = DisplayArea.GetX1();
		windowRect.right = DisplayArea.GetX2();
		windowRect.top = DisplayArea.GetY1();
		windowRect.bottom = DisplayArea.GetY2();
	}
	else
	{
		windowRect.left = mWindowedPosX;
		windowRect.right = mWindowedPosX + width;
		windowRect.top = mWindowedPosY;
		windowRect.bottom = mWindowedPosY + height;
	}

	RegisterWindowClass();
	SetWindowStyleValues();
	::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	mWnd = ::CreateWindowEx(mWindowExStyle,						// Extended Style For The Window
		WINDOWED_WIN_CLASS_NAME,
		title.c_str(),						// Window Title
		mWindowStyle,					// Required Window Style
		windowRect.left, windowRect.top,								// Window Position
		windowRect.right - windowRect.left,	// Calculate Window Width
		windowRect.bottom - windowRect.top,	// Calculate Window Height
		nullptr,								// No Parent Window
		nullptr,								// No Menu
		::GetModuleHandle(nullptr),
		reinterpret_cast<LPVOID>(this));

	if (mWnd == 0)
	{
		return;
	}

	mDC = ::GetDC(mWnd);
	if (!mDC)
	{
		return;
	}

	mDPI = (float)GetDpiForWindow(mWnd);

	if (mAlwaysOnTop)
	{
		::SetWindowLongA(mWnd, GWL_EXSTYLE, ::GetWindowLongA(mWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
		::SetWindowPos(mWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

void RAppWindowImplMsw::CompleteCreation()
{
	::DragAcceptFiles(mWnd, TRUE);
	::ShowWindow(mWnd, SW_SHOW);
	::SetForegroundWindow(mWnd);
	::SetFocus(mWnd);
}

void RAppWindowImplMsw::RegisterWindowClass()
{
	static bool sRegistered = false;

	if (sRegistered)
		return;

	WNDCLASSEX	wc;
	HMODULE instance = ::GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = WndProc;						// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = instance;							// Set The Instance
	wc.hIcon = (HICON)::LoadImage(instance, MAKEINTRESOURCE(1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE); // Load The Default Cinder Icon
	wc.hIconSm = NULL;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);		// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = WINDOWED_WIN_CLASS_NAME;

	if (!::RegisterClassEx(&wc)) 
	{	
		DWORD err = ::GetLastError();
		return;
	}

	sRegistered = true;
}


void RAppWindowImplMsw::SetFullScreen(bool bFullScreen)
{
	if (mFullScreen != bFullScreen)
		ToggleFullScreen();
}

void RAppWindowImplMsw::ToggleFullScreen()
{
	bool prevFullScreen = mFullScreen;

	mFullScreen = !mFullScreen;
	SetWindowStyleValues();

	Vec2i NewWindowSize;

	RECT WindowRect;
	if (prevFullScreen)
	{
		WindowRect.left = mWindowedPosX;
		WindowRect.top = mWindowedPosY;
		WindowRect.right = mWindowedPosX + mWindowWidth;
		WindowRect.bottom = mWindowedPosY + mWindowHeight;
		::AdjustWindowRectEx(&WindowRect, mWindowStyle, FALSE, mWindowExStyle);
	}
	else
	{
		mWindowedPosX = mWindowOffsetX;
		mWindowedPosY = mWindowOffsetY;
		mWindowedSizePx = Vec2i(mWindowWidth, mWindowHeight);
		NewWindowSize = mDisplay->GetSize();

		RArea Area = mDisplay->GetBounds();
		WindowRect.left = Area.GetX1();
		WindowRect.top = Area.GetY1();
		WindowRect.right = Area.GetX2();
		WindowRect.bottom = Area.GetY2();
	}

	::SetWindowLongA(mWnd, GWL_STYLE, mWindowStyle);
	::SetWindowLongA(mWnd, GWL_EXSTYLE, mWindowExStyle);
	::SetWindowPos(mWnd, HWND_TOP, WindowRect.left, WindowRect.top, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
		SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOCOPYBITS | SWP_NOREDRAW);

	::ShowWindow(mWnd, SW_SHOW);
	::SetForegroundWindow(mWnd);
	::SetFocus(mWnd);
}


void RAppWindowImplMsw::GetScreenSize(int32 clientWidth, int32 clientHeight, int32* resultWidth, int32* resultHeight)
{
	RECT windowRect;
	windowRect.left = windowRect.top = 0;
	windowRect.right = clientWidth;
	windowRect.bottom = clientHeight;
	::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);
	*resultWidth = windowRect.right - windowRect.left;
	*resultHeight = windowRect.bottom - windowRect.top;
}

void RAppWindowImplMsw::SetSize(int32 width, int32 height)
{
	int32 screenWidthPx, screenHeightPx;
	GetScreenSize(width, height, &screenWidthPx, &screenHeightPx);
	::SetWindowPos(mWnd, HWND_TOP, 0, 0, screenWidthPx, screenHeightPx, SWP_NOMOVE | SWP_NOZORDER);
}

void RAppWindowImplMsw::SetPos(int32 x, int32 y)
{
	RECT clientArea;
	clientArea.left = x; clientArea.top = y;
	clientArea.right = x + 1; clientArea.bottom = y + 1; 
	::AdjustWindowRectEx(&clientArea, mWindowStyle, FALSE, mWindowExStyle);
	::SetWindowPos(mWnd, HWND_TOP, clientArea.left, clientArea.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	POINT upperLeft;
	upperLeft.x = upperLeft.y = 0;
	::ClientToScreen(mWnd, &upperLeft);
	mWindowOffsetX = upperLeft.x;
	mWindowOffsetY = upperLeft.y;
}

void RAppWindowImplMsw::Hide()
{
	::ShowWindow(mWnd, SW_HIDE);
	mHidden = true;
}

void RAppWindowImplMsw::Show()
{
	::ShowWindow(mWnd, SW_SHOW);
	mHidden = false;
}

bool RAppWindowImplMsw::IsHidden() const
{
	return mHidden;
}


void RAppWindowImplMsw::Close()
{
	GetAppImpl()->CloseWindow(this);
}

String RAppWindowImplMsw::GetTitle() const
{
	INT32 numChars = ::GetWindowTextLength(mWnd);
	char* title = (char*)RB_ALLOCATE( sizeof(char) * (numChars + 1) );
	::GetWindowText(mWnd, &title[0], numChars + 1);
	title[numChars] = 0;
	return String(title);
}

void RAppWindowImplMsw::SetTitle(const String& title)
{
	if (title.empty())
		::SetWindowText(mWnd, "");
	else
		::SetWindowText(mWnd, title.c_str());
}


void RAppWindowImplMsw::SetBorderless(bool borderless)
{
	if (mBorderless != borderless)
	{
		mBorderless = borderless;
		if (mBorderless)
		{
			mWindowExStyle = WS_EX_APPWINDOW;
			mWindowStyle = WS_POPUP;
		}
		else
		{
			mWindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES;	// Window Extended Style
			mWindowStyle = (mResizable) ? WS_OVERLAPPEDWINDOW
				: (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);							// Windows Style
		}

		POINT upperLeft;
		upperLeft.x = upperLeft.y = 0;
		::ClientToScreen(mWnd, &upperLeft);

		RECT windowRect;
		::GetClientRect(mWnd, &windowRect);
		windowRect.left += upperLeft.x; windowRect.right += upperLeft.x;
		windowRect.top += upperLeft.y; windowRect.bottom += upperLeft.y;
		::AdjustWindowRectEx(&windowRect, mWindowStyle, FALSE, mWindowExStyle);		// Adjust Window To True Requested Size

		::SetWindowLongA(mWnd, GWL_STYLE, mWindowStyle);
		::SetWindowLongA(mWnd, GWL_EXSTYLE, mWindowExStyle);
		::SetWindowPos(mWnd, HWND_TOP, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOZORDER);
		if (mBorderless)
			::InvalidateRect(0, NULL, TRUE);
	}
}

void RAppWindowImplMsw::SetAlwaysOnTop(bool alwaysOnTop)
{
	if (mAlwaysOnTop != alwaysOnTop)
	{
		mAlwaysOnTop = alwaysOnTop;
		LONG oldExStyle = ::GetWindowLongA(mWnd, GWL_EXSTYLE);
		if (mAlwaysOnTop)
		{
			::SetWindowLongA(mWnd, GWL_EXSTYLE, oldExStyle | WS_EX_TOPMOST);
			::SetWindowPos(mWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
		else
		{
			::SetWindowLongA(mWnd, GWL_EXSTYLE, oldExStyle &= (~WS_EX_TOPMOST));
			::SetWindowPos(mWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
	}
}

void RAppWindowImplMsw::Resize()
{
	mAppImpl->SetActiveWindow(mWindow);
	mWindow->EmitResize();
}

void RAppWindowImplMsw::PrivateClose()
{
	if (mDC)
		::ReleaseDC(mWnd, mDC);

	if (mWnd)
		::DestroyWindow(mWnd);

	mWnd = 0;
}

uint32 PrepMouseEventModifiers(WPARAM wParam)
{
	uint32 result = 0;
	if (wParam & MK_CONTROL) result |= RMouseEvent::CTRL;
	if (wParam & MK_LBUTTON) result |= RMouseEvent::LEFT;
	if (wParam & MK_MBUTTON) result |= RMouseEvent::MIDDLE;
	if (wParam & MK_RBUTTON) result |= RMouseEvent::RIGHT;
	if (wParam & MK_SHIFT) result |= RMouseEvent::SHIFT;
	if (::GetKeyState(VK_MENU) < 0) result |= RMouseEvent::ALT;
	if ((::GetKeyState(VK_LWIN) < 0) || (::GetKeyState(VK_RWIN) < 0)) result |= RMouseEvent::META;
	return result;
}

uint32 PrepKeyEventModifiers()
{
	uint32 result = 0;
	if (::GetKeyState(VK_CONTROL) & 0x8000) result |= RKeyboardEvent::CTRL;
	if (::GetKeyState(VK_SHIFT) & 0x8000) result |= RKeyboardEvent::SHIFT;
	if ((::GetKeyState(VK_LMENU) & 0x8000) || (::GetKeyState(VK_RMENU) & 0x8000)) result |= RKeyboardEvent::ALT;
	if ((::GetKeyState(VK_LWIN) < 0) || (::GetKeyState(VK_RWIN) < 0)) result |= RKeyboardEvent::META;
	return result;
}

int32 PrepNativeKeyCode(WPARAM wParam)
{
	uint32 result = (int32)wParam;
	if (wParam == VK_MENU) {
		result = (::GetKeyState(VK_RMENU)) ? VK_RMENU : VK_LMENU;
	}
	else if (wParam == VK_SHIFT) {
		result = (::GetKeyState(VK_RSHIFT)) ? VK_RSHIFT : VK_LSHIFT;
	}
	else if (wParam == VK_CONTROL) {
		result = (::GetKeyState(VK_RCONTROL)) ? VK_RCONTROL : VK_LCONTROL;
	}
	return result;
}

WCHAR MapVirtualKey(WPARAM wParam)
{
	BYTE keyboardState[256];
	::GetKeyboardState(keyboardState);
	WCHAR result[4] = { 0, 0, 0, 0 };

	// the control key messes up the ToAscii result, so we zero it out
	keyboardState[VK_CONTROL] = 0;

	int resultLength = ::ToUnicode(static_cast<UINT>(wParam), ::MapVirtualKey(static_cast<UINT>(wParam), 0), keyboardState, result, 4, 0);
	if (resultLength >= 1)
		return result[0];
	else
		return 0;
}

LRESULT CALLBACK WndProc(
	HWND	mWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	RAppWindowImplMsw* impl;

	// if the message is WM_NCCREATE we need to hide 'this' in the window long
	if (uMsg == WM_NCCREATE)
	{
		impl = reinterpret_cast<RAppWindowImplMsw*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
		::SetWindowLongPtr(mWnd, GWLP_USERDATA, (__int3264)(LONG_PTR)impl);
	}
	else // the warning on this line is harmless
		impl = reinterpret_cast<RAppWindowImplMsw*>(::GetWindowLongPtr(mWnd, GWLP_USERDATA));

	if (!impl)
		return DefWindowProc(mWnd, uMsg, wParam, lParam);

	RAppImplMsw* appImpl = impl->GetAppImpl();
	appImpl->SetActiveWindow(impl->mWindow);
	RApplication* App = appImpl->GetApp();
	RWindowRef ActiveWindow = impl->GetWindow();

	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			if (!appImpl->mActive)
			{
				appImpl->mActive = true;
			}
		}
		else
		{
			if (appImpl->mActive)
			{
				appImpl->mActive = false;
			}
		}
		break;
	case WM_ACTIVATE:
		if ((wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE))
			appImpl->SetForegroundWindow(impl->GetWindow());
		break;
	case WM_CLOSE:								// Did We Receive A Close Message?
		appImpl->CloseWindow(impl);
		// be careful not to do anything with 'impl' after here
		return 0;
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		WCHAR MappedVirtualKeyCode = MapVirtualKey(wParam);
		RKeyboardEvent KeyEvent(ActiveWindow.get(), RKeyboardEvent::TranslateNativeKeyCode(PrepNativeKeyCode(wParam)),
			MappedVirtualKeyCode, static_cast<int8>(MappedVirtualKeyCode), PrepKeyEventModifiers(), static_cast<uint32>(wParam));
		if (KeyEvent.IsHandled())
		{
			return 0;
		}
		App->KeyDown(KeyEvent);
	}break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		WCHAR MappedVirtualKeyCode = MapVirtualKey(wParam);
		RKeyboardEvent KeyEvent(ActiveWindow.get(), RKeyboardEvent::TranslateNativeKeyCode(PrepNativeKeyCode(wParam)),
			MappedVirtualKeyCode, static_cast<int8>(MappedVirtualKeyCode), PrepKeyEventModifiers(), static_cast<uint32>(wParam));
		if (KeyEvent.IsHandled())
		{
			return 0;
		}
		App->KeyUp(KeyEvent);
	}break;

	// mouse events
	case WM_LBUTTONDOWN:
	{
		::SetCapture(mWnd);
		impl->mIsDragging = true;
		RMouseEvent Event(ActiveWindow.get(), RMouseEvent::LEFT, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseDown(Event);
	}break;

	case WM_RBUTTONDOWN:
	{
		::SetCapture(mWnd);
		impl->mIsDragging = true;
		RMouseEvent Event(ActiveWindow.get(), RMouseEvent::RIGHT, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseDown(Event);
	}break;

	case WM_MBUTTONDOWN:
	{
		::SetCapture(mWnd);
		impl->mIsDragging = true;
		RMouseEvent Event(ActiveWindow.get(), RMouseEvent::MIDDLE, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseDown(Event);
	}break;

	case WM_LBUTTONUP:
	{
		::ReleaseCapture();
		impl->mIsDragging = false;
		RMouseEvent Event(ActiveWindow.get(), RMouseEvent::LEFT, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseUp(Event);
	}break;

	case WM_RBUTTONUP:
	{
		::ReleaseCapture();
		impl->mIsDragging = false;
		RMouseEvent Event(ActiveWindow.get(), RMouseEvent::RIGHT, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseUp(Event);
	}break;

	case WM_MBUTTONUP:
	{
		::ReleaseCapture();
		impl->mIsDragging = false;
		RMouseEvent Event(ActiveWindow.get(), RMouseEvent::MIDDLE, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseUp(Event);
	}break;

	case WM_MOUSEWHEEL:
	{
		POINT pt = { ((int32)(int16)LOWORD(lParam)), ((int32)(int16)HIWORD(lParam)) };
		::MapWindowPoints(NULL, mWnd, &pt, 1);
		RMouseEvent Event(ActiveWindow.get(), 0, (int32)pt.x, (int32)pt.y, PrepMouseEventModifiers(wParam),
			GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f, static_cast<uint32>(wParam));
		if (Event.IsHandled())
		{
			return 0;
		}
		App->MouseWheel(Event);
	}break;

	case WM_KILLFOCUS:
		if (impl->mIsDragging) 
		{
			RMouseEvent Event(ActiveWindow.get(), 0, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
			App->MouseUp(Event);
		}
		impl->mIsDragging = false;
		break;
	case WM_MOUSEMOVE:
	{
		if (impl->mIsDragging) 
		{
			RMouseEvent Event(ActiveWindow.get(), 0, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
			App->MouseDrag(Event);
		}
		else 
		{
			RMouseEvent Event(ActiveWindow.get(), 0, LOSHORT(lParam), HISHORT(lParam), PrepMouseEventModifiers(wParam), 0.0f, static_cast<uint32>(wParam));
			App->MouseMove(Event);
		}
	}break;

	case WM_SIZE:
	{
		impl->mWindowWidth = LOWORD(lParam);
		impl->mWindowHeight = HIWORD(lParam);
		if (appImpl->SetupHasBeenCalled())
		{
			App->Resize();
		}
		return 0;
	}break;

	case WM_MOVE:
	{
		impl->mWindowOffsetX = LOSHORT(lParam);
		impl->mWindowOffsetY = HISHORT(lParam);
		return 0;
	}break;

	case WM_PAINT:
		break;

	case WM_DISPLAYCHANGE:
		appImpl->mNeedsToRefreshDisplays = true;
		break;
	case WM_DEVICECHANGE:
		appImpl->mNeedsToRefreshDisplays = true;
		break;
	case 0x02E0 /*WM_DPICHANGED*/:
		if (appImpl->SetupHasBeenCalled()) 
		{
			appImpl->mNeedsToRefreshDisplays = true;
		}
		break;
	}

	// unhandled messages To DefWindowProc
	return DefWindowProc(mWnd, uMsg, wParam, lParam);
}

RB_NAMESPACE_END