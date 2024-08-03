#pragma once
#include "Prerequisites.h"

RB_NAMESPACE_BEGIN

class RAppWindow;

class RB_CORE_API RAppEvent
{
public:
	virtual ~RAppEvent() = default;

	const RAppWindow* GetWindow() const { return mWindow; }

	bool IsHandled() const { return mHandled; }
	void SetHandled(bool handled = true) { mHandled = handled; }

protected:
	RAppEvent() : mHandled(false) {}
	RAppEvent(const RAppWindow* Window) : mHandled(false), mWindow(Window) {}

protected:
	bool mHandled;
	const RAppWindow* mWindow;
};

RB_NAMESPACE_END