#pragma once
#include "AppEvent.h"

RB_NAMESPACE_BEGIN

class RB_CORE_API RMouseEvent : public RAppEvent
{
public:
	enum
	{
		LEFT = 0x0001,
		RIGHT = 0x0002,
		MIDDLE = 0x0004,
		SHIFT = 0x0008,
		ALT = 0x0010,
		CTRL = 0x0020,
		META = 0x0040,
		ACCEL = CTRL
	};

public:
	RMouseEvent()
		: RAppEvent()
		, mInitiator(-1)
		, mPos(Vec2i(0, 0))
		, mModifiers(0)
		, mWheelIncrement(0.0f)
		, mNativeModifiers(0)
	{
	}

	RMouseEvent(const RAppWindow* Window, int32 initiator, int32 x, int32 y, uint32 modifiers, float wheelIncrement, uint32 nativeModifiers)
		: RAppEvent(Window)
		, mInitiator(initiator)
		, mPos(Vec2i(x, y))
		, mModifiers(modifiers)
		, mWheelIncrement(wheelIncrement)
		, mNativeModifiers(nativeModifiers)
	{
	}

	int32 GetX() const { return mPos.x; }
	int32 GetY() const { return mPos.y; }
	const Vec2i& GetPos() const { return mPos; }
	void SetPos(const Vec2i& pos) { mPos = pos; }
	bool IsLeft() const { return (mInitiator & LEFT) ? true : false; }
	bool IsRight() const { return (mInitiator & RIGHT) ? true : false; }
	bool IsMiddle() const { return (mInitiator & MIDDLE) ? true : false; }
	bool IsLeftDown() const { return (mModifiers & LEFT) ? true : false; }
	bool IsRightDown() const { return (mModifiers & RIGHT) ? true : false; }
	bool IsMiddleDown() const { return (mModifiers & MIDDLE) ? true : false; }
	bool IsShiftDown() const { return (mModifiers & SHIFT) ? true : false; }
	bool IsAltDown() const { return (mModifiers & ALT) ? true : false; }
	bool IsControlDown() const { return (mModifiers & CTRL) ? true : false; }
	bool IsMetaDown() const { return (mModifiers & META) ? true : false; }
	bool IsAccelDown() const { return (mModifiers & ACCEL) ? true : false; }
	float GetWheelIncrement() const { return mWheelIncrement; }
	uint32 GetNativeModifiers() const { return mNativeModifiers; }

protected:
	int32	mInitiator;
	Vec2i	mPos;
	uint32	mModifiers;
	float	mWheelIncrement;
	uint32	mNativeModifiers;
};

RB_NAMESPACE_END