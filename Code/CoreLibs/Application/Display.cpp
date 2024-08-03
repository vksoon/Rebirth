#include "Display.h"

#include <windows.h>
#include <Shlwapi.h>
#include <shlobj.h>

RB_NAMESPACE_BEGIN

bool RDisplay::mDisplaysInitialized = false;
Array<RDisplay*> RDisplay::mDisplays;

namespace 
{
	HMODULE sShcoreDll = nullptr;
	typedef HRESULT(WINAPI* GetDpiForMonitorFn)(HMONITOR hmonitor, DWORD /*MONITOR_DPI_TYPE*/ dpiType, UINT* dpiX, UINT* dpiY);
	GetDpiForMonitorFn sGetDpiForMonitorFnPtr = nullptr;

	int32 GetMonitorBitsPerPixel(HMONITOR hMonitor)
	{
		int32 result = 0;
		MONITORINFOEX mix;
		memset(&mix, 0, sizeof(MONITORINFOEX));
		mix.cbSize = sizeof(MONITORINFOEX);
		::GetMonitorInfo(hMonitor, &mix);
		HDC hMonitorDC = ::CreateDC(TEXT("DISPLAY"), mix.szDevice, NULL, NULL);
		if (hMonitorDC) 
		{
			result = ::GetDeviceCaps(hMonitorDC, BITSPIXEL);
			::DeleteDC(hMonitorDC);
		}

		return result;
	}

	void GetMonitorName(HMONITOR hMonitor, String& OutName)
	{
		MONITORINFOEX mix;
		memset(&mix, 0, sizeof(MONITORINFOEX));
		mix.cbSize = sizeof(MONITORINFOEX);
		::GetMonitorInfo(hMonitor, &mix);
		DISPLAY_DEVICE dispDev;
		dispDev.cb = sizeof(DISPLAY_DEVICE);
		::EnumDisplayDevices(mix.szDevice, 0, &dispDev, 0);
		OutName = dispDev.DeviceString;
	}

}

String RDisplay::GetName()
{
	if (mNameDirty) 
	{
		GetMonitorName(mMonitor, mName);
		mNameDirty = false;
	}

	return mName;
}

BOOL CALLBACK RDisplay::EnumMonitorProc(HMONITOR hMonitor, HDC /*hdc*/, LPRECT rect, LPARAM lParam)
{
	Array<RDisplay*>* Displays = reinterpret_cast<Array<RDisplay*>*>(lParam);

	RDisplay* newDisplay = new RDisplay();
	newDisplay->mArea = RArea(rect->left, rect->top, rect->right, rect->bottom);
	newDisplay->mMonitor = hMonitor;
	newDisplay->mBitsPerPixel = GetMonitorBitsPerPixel(hMonitor);
	newDisplay->mContentScale = 1.0f; // default value
	
	// dynamic function resoluion for ::GetDpiForMonitor()
	if (sShcoreDll != (HMODULE)INVALID_HANDLE_VALUE) 
	{
		if (!sGetDpiForMonitorFnPtr)
		{
			sGetDpiForMonitorFnPtr = (GetDpiForMonitorFn)::GetProcAddress(sShcoreDll, "GetDpiForMonitor");
		}

		if (sGetDpiForMonitorFnPtr) 
		{
			UINT x, y;
			HRESULT hr = sGetDpiForMonitorFnPtr(hMonitor, (DWORD)0 /*MDT_Effective_DPI*/, &x, &y);
			if (SUCCEEDED(hr))
			{
				newDisplay->mContentScale = x / 96.0f;
			}
		}
	}

	Displays->push_back(newDisplay);
	return TRUE;
}

const Array<RDisplay*>& RDisplay::GetDisplays()
{
	if (!mDisplaysInitialized)
	{
		if (!sShcoreDll)
		{
			sShcoreDll = ::LoadLibrary("Shcore.dll");
			if (!sShcoreDll)
			{
				sShcoreDll = (HMODULE)INVALID_HANDLE_VALUE;
			}
		}

		if (sShcoreDll)
		{
			typedef HRESULT(WINAPI* SetProcessDpiAwarenessFn)(DWORD /*PROCESS_DPI_AWARNESS*/ value);
			SetProcessDpiAwarenessFn setProcessDpiAwarenessFnPtr = nullptr;
			setProcessDpiAwarenessFnPtr = (SetProcessDpiAwarenessFn)::GetProcAddress(sShcoreDll, "SetProcessDpiAwareness");
			if (setProcessDpiAwarenessFnPtr)
				setProcessDpiAwarenessFnPtr(2 /*PROCESS_PER_MONITOR_DPI_AWARE*/);
		}

		::EnumDisplayMonitors(NULL, NULL, RDisplay::EnumMonitorProc, (LPARAM)&mDisplays);

		// ensure that the primary display is sDisplay[0]
		const POINT ptZero = { 0, 0 };
		HMONITOR primMon = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);

		size_t m;
		for (m = 0; m < mDisplays.size(); ++m)
		{
			if (mDisplays[m]->mMonitor == primMon)
			{
				break;
			}
		}

		if ((m != 0) && (m < mDisplays.size()))
		{
			std::swap(mDisplays[0], mDisplays[m]);
		}

		mDisplaysInitialized = true;
	}

	return mDisplays;
}

void RDisplay::RefreshDisplays()
{
	Array<RDisplay*> NewDisplays;

	::EnumDisplayMonitors(NULL, NULL, RDisplay::EnumMonitorProc, (LPARAM)&NewDisplays);

	Array<RDisplay*> ConnectedDisplays; // displays we need to issue a connected signal to
	Array<RDisplay*> ChangedDisplays; // displays we need to issue a changed signal to
	Array<RDisplay*> DisconnectedDisplays; // displays we need to issue a disconnected signal to

	for (auto Display : mDisplays)
	{
		Display->mVisitedFlag = false;
	}

	// find any changed or new displays
	for (auto NewDisplayIt = NewDisplays.begin(); NewDisplayIt != NewDisplays.end(); ++NewDisplayIt) 
	{
		RDisplay* NewDisplay = *NewDisplayIt;

		// find the old display with the same mMonitor
		bool bFound = false;
		for (auto displayIt = mDisplays.begin(); displayIt != mDisplays.end(); ++displayIt) 
		{
			RDisplay* OldDisplay = *displayIt;
			if (OldDisplay->mMonitor == NewDisplay->mMonitor) 
			{
				// found this display; see if anything changed
				if ((OldDisplay->mArea != NewDisplay->mArea) 
					|| (OldDisplay->mBitsPerPixel != NewDisplay->mBitsPerPixel) 
					|| (OldDisplay->mContentScale != NewDisplay->mContentScale))
				{
					ChangedDisplays.push_back(*displayIt);
				}
				*OldDisplay = *NewDisplay;
				OldDisplay->mVisitedFlag = true;
				bFound = true;
				break;
			}
		}
		if (!bFound) 
		{
			NewDisplay->mVisitedFlag = true; // don't want to later consider this display disconnected
			ConnectedDisplays.push_back(*NewDisplayIt);
			mDisplays.push_back(*NewDisplayIt);
		}
	}

	// deal with any displays which have been disconnected
	for (auto displayIt = mDisplays.begin(); displayIt != mDisplays.end(); ) 
	{
		if (!(*displayIt)->mVisitedFlag) 
		{
			DisconnectedDisplays.push_back(*displayIt);
			displayIt = mDisplays.erase(displayIt);
		}
		else
			++displayIt;
	}
}

RDisplay* RDisplay::FindDisplayFromHmonitor(HMONITOR hMonitor)
{
	for (auto& display : mDisplays)
	{
		if (display->mMonitor == hMonitor)
		{
			return display;
		}
	}

	if (!mDisplays.empty())
		return mDisplays[0];
	
	return nullptr;
}

RDisplay* RDisplay::GetDisplayForPoint(const Vec2i& pt)
{
	const Array<RDisplay*>& Displays = GetDisplays();
	for (Array<RDisplay*>::const_iterator DisplayIt = Displays.begin(); DisplayIt != Displays.end(); ++DisplayIt) 
	{
		if ((*DisplayIt)->Contains(pt))
		{
			return *DisplayIt;
		}
	}

	return nullptr;
}

RArea RDisplay::GetSpanningArea()
{
	RArea Result = (*RDisplay::GetDisplays().begin())->GetBounds();
	for (Array<RDisplay*>::const_iterator DisplayIt = (RDisplay::GetDisplays().begin())++; DisplayIt != RDisplay::GetDisplays().end(); ++DisplayIt) 
	{
		Result.Include((*DisplayIt)->GetBounds());
	}

	return Result;
}

Vec2i RDisplay::GetSystemCoordinate(const Vec2i& DisplayRelativeCoordinate) const
{
	int32 X = mArea.GetUL().x + DisplayRelativeCoordinate.x;
	int32 Y = mArea.GetUL().y + DisplayRelativeCoordinate.y;
	return Vec2i(X, Y);
}

RDisplay* RDisplay::GetMainDisplay()
{
	auto Displays = GetDisplays();
	if (Displays.empty())
	{
		return nullptr;
	}

	return Displays[0];
}

RB_NAMESPACE_END
