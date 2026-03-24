#include "kismet.h"

#include "gvalue.h"
#include "gdefine.h"

#include <iostream>
#include <sstream>
#include <iomanip>

bool kismet::is_key_down(const int& key)
{
	return (GetAsyncKeyState(key) & 0x8000) && GetForegroundWindow() == get_window();
}

HWND kismet::get_window()
{
	return gvalue::game_window;
}

HWND kismet::find_window()
{
	return FindWindowW(WINDOW_CLASS_NAME, WINDOW_NAME);
}

float kismet::fround(float x)
{
	return std::round(x * 10.0f) / 10.0f;
}

std::wstring kismet::fround_to_string(float value)
{
	std::wstringstream wss;
	wss << std::fixed << std::setprecision(1) << value;
	return wss.str();
}