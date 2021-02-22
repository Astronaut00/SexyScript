#include "Utils.h"
#include "Imgui.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm>
#include "Settings.h"
#include "Globals.h"
#include "skCrypter.h"
#include <string>

void utils::SetWindowVisible(bool visible, HWND hWnd)
{
	if (visible)
	{
		ShowWindow(hWnd, SW_SHOW);
	}
	else if (!visible)
	{
		ShowWindow(hWnd, SW_HIDE);
	}
}

void utils::SetWindowPosition(HWND top, HWND bottom)
{
	SetWindowPos(top, bottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void utils::SetWindowClickable(bool clickable, HWND hWnd)
{
	long style_inactive = WS_EX_LAYERED | WS_EX_TOOLWINDOW;
	long style_active = WS_EX_LAYERED | WS_EX_TOOLWINDOW;

	if (clickable)
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, style_active);
		MoveWindow(globals::overlay, 0, 0, globals::full_screen.w, globals::full_screen.h, TRUE);
	}
	else
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, style_inactive);
		MoveWindow(globals::overlay, 0, 0, globals::small_screen.w, globals::small_screen.h, TRUE);
	}
}

bool utils::key_down(int virtual_keycode)
{
	return GetAsyncKeyState(virtual_keycode);
}

void utils::query_sleep(int ms) // Sleep / Delay
{
	LONGLONG timerResolution;
	LONGLONG wantedTime;
	LONGLONG currentTime;

	QueryPerformanceFrequency((LARGE_INTEGER*)&timerResolution);
	timerResolution /= 1000;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	wantedTime = currentTime / timerResolution + ms;
	currentTime = 0;
	while (currentTime < wantedTime)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		currentTime /= timerResolution;
	}
}

float utils::random_float(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

void mouse_move(int x, int y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(input));
}

void utils::relative_mouse_move(double delay, double control_time, float x, float y)
{
	int x_ = 0, y_ = 0, t_ = 0;

	for (int i = 1; i <= (int)control_time; ++i)
	{
		int xI = i * x / (int)control_time;
		int yI = i * y / (int)control_time;
		int tI = i * (int)control_time / (int)control_time;

		int delta_x = xI - x_;
		int delta_y = yI - y_;

		mouse_move(delta_x, delta_y);
		query_sleep((int)tI - (int)t_);
		x_ = xI; y_ = yI; t_ = tI;
	}
	query_sleep((int)delay - (int)control_time);
}

void utils::echo_error(const std::exception& e, int code)
{
	std::string error_message = std::string(skCrypt("Error code: ")) + std::to_string(code) + std::string("\n") + std::string(e.what());
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR | MB_OK);
	static HWND TaskBar = ::FindWindow(skCrypt(L"Shell_TrayWnd"), NULL);
	SetWindowVisible(true, TaskBar);
	exit(1);
}

int utils::rust_key_to_virtual_key(const char* rust_key)
{
	for (int virtual_key = 0; virtual_key < RustKeys.size(); virtual_key++)
	{
		std::string this_key = RustKeys[virtual_key];

		if (this_key == std::string(rust_key))
		{
			return virtual_key;
		}
	}
}