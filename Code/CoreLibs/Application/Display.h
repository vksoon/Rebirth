#pragma once
#include "Prerequisites.h"
#include "Area.h"
#include "ApplicationSupports.h"

RB_NAMESPACE_BEGIN

class RB_CORE_API RDisplay 
{
public:
	RDisplay() 
		: mArea(RArea::Zero())
		, mBitsPerPixel(0)
		, mContentScale(1.0f)
		, mName("")
		, mNameDirty(true) 
	{
	}

	virtual ~RDisplay() 
	{
	}

	int32 GetWidth() const { return mArea.GetWidth(); }
	int32 GetHeight() const { return mArea.GetHeight(); }
	Vec2i GetSize() const { return Vec2i(GetWidth(), GetHeight()); }
	float GetAspectRatio() const { return GetWidth() / (float)GetHeight(); }
	const RArea& GetBounds() const { return mArea; }
	Vec2i GetSystemCoordinate(const Vec2i& displayRelativeCoordinate) const;

	int32 GetBitsPerPixel() const { return mBitsPerPixel; }
	float GetContentScale() const { return mContentScale; }

	bool Contains(const Vec2i& pt) const { return mArea.Contains(pt); }

	String GetName();

public :
	static RDisplay* GetMainDisplay();
	static RDisplay* GetDisplayForPoint(const Vec2i& pt);
	static RArea GetSpanningArea();

public :
	static const Array<RDisplay*>& GetDisplays();
	static void RefreshDisplays();
	static RDisplay* FindDisplayFromHmonitor(HMONITOR hMonitor);

private:
	static BOOL CALLBACK EnumMonitorProc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam);


private :
	HMONITOR mMonitor;
	bool mVisitedFlag;

	RArea mArea;
	int32 mBitsPerPixel;
	float mContentScale;
	String mName;
	bool mNameDirty;

	static bool mDisplaysInitialized;
	static Array<RDisplay*>	mDisplays;
};

RB_NAMESPACE_END
