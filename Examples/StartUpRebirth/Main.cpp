#include "TestApplication.h"
#include "AppLauncher.h"

void PrepareSettings(Rebirth::RApplication::Settings* Settings)
{
	// Window 
	Settings->SetWindowPos(0, 0);
	Settings->SetWindowSize(1280, 720);
	Settings->SetFrameRate(60.0f);
	Settings->SetResizable(false);
	Settings->SetFullScreen(false);
}

RB_APP_LAUNCH(TestApplication, "Rebirth App", PrepareSettings)