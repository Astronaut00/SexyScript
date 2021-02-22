#pragma once
#include <Windows.h>

struct screen_size {
	int w;
	int h;
};

namespace globals {
	extern HWND overlay;
	extern HWND rust;
	extern bool menu_active;
	extern screen_size small_screen;
	extern screen_size full_screen;
}