#pragma once
#include <Windows.h>
#include <string>

class kismet
{
public:
	static bool is_key_down(const int& key);
	static HWND get_window();
	static HWND find_window();

	static float fround(float x);
	static std::wstring fround_to_string(float value);
};