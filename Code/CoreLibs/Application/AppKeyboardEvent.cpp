#include "AppKeyboardEvent.h"
#include <Windows.h>

RB_NAMESPACE_BEGIN

static bool sTableInited = false;
static const int32 MAX_KEYCODE = 256;
int32 sKeyTable[MAX_KEYCODE];

static void InitKeyTable()
{
	for (int c = 0; c < MAX_KEYCODE; ++c)
		sKeyTable[c] = RKeyboardEvent::KEY_UNKNOWN;

	sKeyTable[VK_BACK]		= RKeyboardEvent::KEY_BACKSPACE;
	sKeyTable[VK_TAB]		= RKeyboardEvent::KEY_TAB;
	sKeyTable[VK_CLEAR]		= RKeyboardEvent::KEY_CLEAR;
	sKeyTable[VK_RETURN]	= RKeyboardEvent::KEY_RETURN;
	sKeyTable[VK_PAUSE]		= RKeyboardEvent::KEY_PAUSE;
	sKeyTable[VK_ESCAPE]	= RKeyboardEvent::KEY_ESCAPE;
	sKeyTable[VK_SPACE]		= RKeyboardEvent::KEY_SPACE;
	sKeyTable[0xDE]			= RKeyboardEvent::KEY_QUOTE;
	sKeyTable[0xBC]			= RKeyboardEvent::KEY_COMMA;
	sKeyTable[0xBD]			= RKeyboardEvent::KEY_MINUS;
	sKeyTable[0xBE]			= RKeyboardEvent::KEY_PERIOD;
	sKeyTable[0xBF]			= RKeyboardEvent::KEY_SLASH;
	sKeyTable['0']			= RKeyboardEvent::KEY_0;
	sKeyTable['1']			= RKeyboardEvent::KEY_1;
	sKeyTable['2']			= RKeyboardEvent::KEY_2;
	sKeyTable['3']			= RKeyboardEvent::KEY_3;
	sKeyTable['4']			= RKeyboardEvent::KEY_4;
	sKeyTable['5']			= RKeyboardEvent::KEY_5;
	sKeyTable['6']			= RKeyboardEvent::KEY_6;
	sKeyTable['7']			= RKeyboardEvent::KEY_7;
	sKeyTable['8']			= RKeyboardEvent::KEY_8;
	sKeyTable['9']			= RKeyboardEvent::KEY_9;
	sKeyTable[0xBA]			= RKeyboardEvent::KEY_SEMICOLON;
	sKeyTable[0xBB]			= RKeyboardEvent::KEY_EQUALS;
	sKeyTable[0xDB]			= RKeyboardEvent::KEY_LEFTBRACKET;
	sKeyTable[0xDC]			= RKeyboardEvent::KEY_BACKSLASH;
	sKeyTable[VK_OEM_102]	= RKeyboardEvent::KEY_LESS;
	sKeyTable[0xDD]			= RKeyboardEvent::KEY_RIGHTBRACKET;
	sKeyTable[0xC0]			= RKeyboardEvent::KEY_BACKQUOTE;
	sKeyTable[0xDF]			= RKeyboardEvent::KEY_BACKQUOTE;
	sKeyTable['A']			= RKeyboardEvent::KEY_a;
	sKeyTable['B']			= RKeyboardEvent::KEY_b;
	sKeyTable['C']			= RKeyboardEvent::KEY_c;
	sKeyTable['D']			= RKeyboardEvent::KEY_d;
	sKeyTable['E']			= RKeyboardEvent::KEY_e;
	sKeyTable['F']			= RKeyboardEvent::KEY_f;
	sKeyTable['G']			= RKeyboardEvent::KEY_g;
	sKeyTable['H']			= RKeyboardEvent::KEY_h;
	sKeyTable['I']			= RKeyboardEvent::KEY_i;
	sKeyTable['J']			= RKeyboardEvent::KEY_j;
	sKeyTable['K']			= RKeyboardEvent::KEY_k;
	sKeyTable['L']			= RKeyboardEvent::KEY_l;
	sKeyTable['M']			= RKeyboardEvent::KEY_m;
	sKeyTable['N']			= RKeyboardEvent::KEY_n;
	sKeyTable['O']			= RKeyboardEvent::KEY_o;
	sKeyTable['P']			= RKeyboardEvent::KEY_p;
	sKeyTable['Q']			= RKeyboardEvent::KEY_q;
	sKeyTable['R']			= RKeyboardEvent::KEY_r;
	sKeyTable['S']			= RKeyboardEvent::KEY_s;
	sKeyTable['T']			= RKeyboardEvent::KEY_t;
	sKeyTable['U']			= RKeyboardEvent::KEY_u;
	sKeyTable['V']			= RKeyboardEvent::KEY_v;
	sKeyTable['W']			= RKeyboardEvent::KEY_w;
	sKeyTable['X']			= RKeyboardEvent::KEY_x;
	sKeyTable['Y']			= RKeyboardEvent::KEY_y;
	sKeyTable['Z']			= RKeyboardEvent::KEY_z;
	sKeyTable[VK_DELETE]	= RKeyboardEvent::KEY_DELETE;

	sKeyTable[VK_NUMPAD0]	= RKeyboardEvent::KEY_KP0;
	sKeyTable[VK_NUMPAD1]	= RKeyboardEvent::KEY_KP1;
	sKeyTable[VK_NUMPAD2]	= RKeyboardEvent::KEY_KP2;
	sKeyTable[VK_NUMPAD3]	= RKeyboardEvent::KEY_KP3;
	sKeyTable[VK_NUMPAD4]	= RKeyboardEvent::KEY_KP4;
	sKeyTable[VK_NUMPAD5]	= RKeyboardEvent::KEY_KP5;
	sKeyTable[VK_NUMPAD6]	= RKeyboardEvent::KEY_KP6;
	sKeyTable[VK_NUMPAD7]	= RKeyboardEvent::KEY_KP7;
	sKeyTable[VK_NUMPAD8]	= RKeyboardEvent::KEY_KP8;
	sKeyTable[VK_NUMPAD9]	= RKeyboardEvent::KEY_KP9;
	sKeyTable[VK_DECIMAL]	= RKeyboardEvent::KEY_KP_PERIOD;
	sKeyTable[VK_DIVIDE]	= RKeyboardEvent::KEY_KP_DIVIDE;
	sKeyTable[VK_MULTIPLY]  = RKeyboardEvent::KEY_KP_MULTIPLY;
	sKeyTable[VK_SUBTRACT]  = RKeyboardEvent::KEY_KP_MINUS;
	sKeyTable[VK_ADD]		= RKeyboardEvent::KEY_KP_PLUS;

	sKeyTable[VK_UP]		= RKeyboardEvent::KEY_UP;
	sKeyTable[VK_DOWN]		= RKeyboardEvent::KEY_DOWN;
	sKeyTable[VK_RIGHT]		= RKeyboardEvent::KEY_RIGHT;
	sKeyTable[VK_LEFT]		= RKeyboardEvent::KEY_LEFT;
	sKeyTable[VK_INSERT]	= RKeyboardEvent::KEY_INSERT;
	sKeyTable[VK_HOME]		= RKeyboardEvent::KEY_HOME;
	sKeyTable[VK_END]		= RKeyboardEvent::KEY_END;
	sKeyTable[VK_PRIOR]		= RKeyboardEvent::KEY_PAGEUP;
	sKeyTable[VK_NEXT]		= RKeyboardEvent::KEY_PAGEDOWN;

	sKeyTable[VK_F1]		= RKeyboardEvent::KEY_F1;
	sKeyTable[VK_F2]		= RKeyboardEvent::KEY_F2;
	sKeyTable[VK_F3]		= RKeyboardEvent::KEY_F3;
	sKeyTable[VK_F4]		= RKeyboardEvent::KEY_F4;
	sKeyTable[VK_F5]		= RKeyboardEvent::KEY_F5;
	sKeyTable[VK_F6]		= RKeyboardEvent::KEY_F6;
	sKeyTable[VK_F7]		= RKeyboardEvent::KEY_F7;
	sKeyTable[VK_F8]		= RKeyboardEvent::KEY_F8;
	sKeyTable[VK_F9]		= RKeyboardEvent::KEY_F9;
	sKeyTable[VK_F10]		= RKeyboardEvent::KEY_F10;
	sKeyTable[VK_F11]		= RKeyboardEvent::KEY_F11;
	sKeyTable[VK_F12]		= RKeyboardEvent::KEY_F12;
	sKeyTable[VK_F13]		= RKeyboardEvent::KEY_F13;
	sKeyTable[VK_F14]		= RKeyboardEvent::KEY_F14;
	sKeyTable[VK_F15]		= RKeyboardEvent::KEY_F15;

	sKeyTable[VK_NUMLOCK]	= RKeyboardEvent::KEY_NUMLOCK;
	sKeyTable[VK_CAPITAL]	= RKeyboardEvent::KEY_CAPSLOCK;
	sKeyTable[VK_SCROLL]	= RKeyboardEvent::KEY_SCROLLOCK;
	sKeyTable[VK_RSHIFT]	= RKeyboardEvent::KEY_RSHIFT;
	sKeyTable[VK_LSHIFT]	= RKeyboardEvent::KEY_LSHIFT;
	sKeyTable[VK_RCONTROL]  = RKeyboardEvent::KEY_RCTRL;
	sKeyTable[VK_LCONTROL]  = RKeyboardEvent::KEY_LCTRL;
	sKeyTable[VK_RMENU]		= RKeyboardEvent::KEY_RALT;
	sKeyTable[VK_LMENU]		= RKeyboardEvent::KEY_LALT;
	sKeyTable[VK_RWIN]		= RKeyboardEvent::KEY_RSUPER;
	sKeyTable[VK_LWIN]		= RKeyboardEvent::KEY_LSUPER;

	sKeyTable[VK_HELP]		= RKeyboardEvent::KEY_HELP;
	sKeyTable[VK_PRINT]		= RKeyboardEvent::KEY_PRINT;
	sKeyTable[VK_SNAPSHOT]	= RKeyboardEvent::KEY_PRINT;
	sKeyTable[VK_CANCEL]	= RKeyboardEvent::KEY_BREAK;
	sKeyTable[VK_APPS]		= RKeyboardEvent::KEY_MENU;

	sTableInited = true;
}

int32 RKeyboardEvent::TranslateNativeKeyCode(int32 nativeKeyCode)
{
	if (!sTableInited)
		InitKeyTable();

	if (nativeKeyCode >= MAX_KEYCODE)
		return RKeyboardEvent::KEY_UNKNOWN;
	else
		return sKeyTable[nativeKeyCode];
}

RB_NAMESPACE_END