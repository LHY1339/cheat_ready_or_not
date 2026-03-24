#include "input.h"
#include "gvalue.h"

input* input::get()
{
	static input inst;
	return &inst;
}

bool input::main(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    switch (u_msg)
    {
    case WM_KEYDOWN:
        if (w_param == VK_INSERT)
        {
            gvalue::menu_open = !gvalue::menu_open;
        }
        break;
    case WM_MOUSEMOVE:
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);
        gvalue::mouse.x = pt.x;
        gvalue::mouse.y = pt.y;
        break;
    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
        gvalue::mouse.left = true;
        break;
    case WM_LBUTTONUP:
        gvalue::mouse.left = false;
        break;
    }

    if (gvalue::menu_open)
    {
        switch (u_msg)
        {
        case WM_INPUT:
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MOUSEWHEEL:
            return false;
        }
    }
	return true;
}
