#pragma once
#include "Application.h"

template<typename _AppType>
void Main(const char* Title, const Rebirth::RAppSettingsCallback& SettingsCallback)
{
	Rebirth::RApplication::PrepareLaunch();

	Rebirth::RApplication::Settings EntrySettings;
	if (SettingsCallback)
	{
		SettingsCallback(&EntrySettings);
	}

	Rebirth::RApplication::SetupLaunch(&EntrySettings, Title, 0, nullptr);

	if (EntrySettings.ShouldQuit())
	{
		return;
	}

	Rebirth::RApplication* Application = static_cast<Rebirth::RApplication*>(Rebirth::RB_New<_AppType>());
	Application->ExecuteLaunch();
	Rebirth::RB_Delete(Application);
	Rebirth::RApplication::CleanupLaunch();
}

#define RB_APP_LAUNCH( APP, Title, ... )																				\
int32_t CALLBACK WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ) \
{																														\
	Main<APP>( Title, ##__VA_ARGS__ );																					\
	return 0;																											\
}