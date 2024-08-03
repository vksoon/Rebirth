#pragma once
#include "Prerequisites.h"
#include "Area.h"

RB_NAMESPACE_BEGIN

class RB_CORE_API RAppDisplay
{
public:
	RAppDisplay()
		: mArea(RArea::Zero())
		, mBitsPerPixel(0)
		, mContentScale(1.0f)
		, mName("")
		, mNameDirty(true)
	{
	}

	virtual ~RAppDisplay() {}

	int32 GetWidth() const { return mArea.GetWidth(); }
	int32 GetHeight() const { return mArea.GetHeight(); }
	Vec2i GetSize() const { return Vec2i(GetWidth(), GetHeight()); }
	float GetAspectRatio() const { return GetWidth() / (float)GetHeight(); }
	RArea GetBounds() const { return mArea; }
	Vec2i GetSystemCoordinate(const Vec2i& displayRelativeCoordinate) const { return mArea.GetUL() + displayRelativeCoordinate; }
	int32 GetBitsPerPixel() const { return mBitsPerPixel; }
	float GetContentScale() const { return mContentScale; }

	bool Contains(const Vec2i& pt) const { return mArea.Contains(pt); }

	virtual String GetName() const { return mName; }

protected:
	RArea mArea;
	int32 mBitsPerPixel;
	float mContentScale;
	mutable String mName;
	mutable bool mNameDirty;
};

RB_NAMESPACE_END