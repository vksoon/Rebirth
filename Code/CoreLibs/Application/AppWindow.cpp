#include "AppWindow.h"
#include "Application.h"
#include "ApplicationSupports.h"

#if RB_PLATFORM == RB_PLATFORM_MSW
#include "MSW/ApplicationImplMsw.h"
#endif

RB_NAMESPACE_BEGIN

void* RAppWindow::GetWindowHandle() const
{
#if RB_PLATFORM == RB_PLATFORM_MSW
	return mImpl->GetWindowHandle();
#endif
}

bool RAppWindow::IsFullScreen() const
{
	return mImpl->IsFullScreen();
}

void RAppWindow::SetFullScreen(bool fullScreen /* = true */)
{
	mImpl->SetFullScreen(fullScreen);
}


int32 RAppWindow::GetWidth() const
{
	return mImpl->GetWidth();
}

int32 RAppWindow::GetHeight() const
{
	return mImpl->GetHeight();
}

float RAppWindow::GetDPI() const
{
	return mImpl->GetDPI();
}

void RAppWindow::SetSize(int32 width, int32 height)
{
	mImpl->SetSize(width, height);
}

int32 RAppWindow::GetPosX() const
{
	return mImpl->GetPosX();
}

int32 RAppWindow::GetPosY() const
{
	return mImpl->GetPosY();
}

void RAppWindow::SetPos(int32 x, int32 y)
{
	mImpl->SetPos(x, y);
}

String RAppWindow::GetTitle() const
{
	return mImpl->GetTitle();
}

void RAppWindow::SetTitle(const String& title)
{
	mImpl->SetTitle(title);
}

bool RAppWindow::IsBorderless() const
{
	return mImpl->IsBorderless();
}

void RAppWindow::SetBorderless(bool borderless)
{
	mImpl->SetBorderless(borderless);
}

bool RAppWindow::IsAlwaysOnTop() const
{
	return mImpl->IsAlwaysOnTop();
}

void RAppWindow::SetAlwaysOnTop(bool alwaysOnTop)
{
	mImpl->SetAlwaysOnTop(alwaysOnTop);
}

void RAppWindow::Hide()
{
	mImpl->Hide();
}

void RAppWindow::Show()
{
	mImpl->Show();
}

bool RAppWindow::IsHidden() const
{
	return mImpl->IsHidden();
}

void RAppWindow::Close()
{
	mImpl->Close();
}

// Signal Emitters
void RAppWindow::EmitClose()
{
}

void RAppWindow::EmitResize()
{
	GetApp()->Resize();
}

RB_NAMESPACE_END