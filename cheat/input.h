#pragma once
#include <Windows.h>

class input
{
public:
	static input* get();
	bool main(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
};

