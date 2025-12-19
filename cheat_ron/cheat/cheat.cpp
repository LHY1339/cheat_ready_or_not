#include "cheat.h"

#include <iostream>
#include <Windows.h>

#include "_sdk.h"
#include "gui.h"
#include "gdefine.h"
#include "gvalue.h"
#include "gconst.h"
#include "menu.h"
#include "visual.h"
#include "kismet.h"
#include "network.h"
#include "aim.h"
#include "player.h"
#include "level.h"
#include "_hook.h"

#pragma warning(disable:4996)

cheat* cheat::get()
{
    static cheat inst;
    return &inst;
}

void cheat::main()
{
	console();
	init();
	hook();
    exit();
}

void cheat::console()
{
#ifdef CHEAT_DEBUG
    AllocConsole();
    FILE* value = freopen("CONOUT$", "w+", stdout);
#endif
}

void cheat::init()
{
    menu::get()->init();
    visual::get()->init();
    network::get()->init();
    aim::get()->init();
    player::get()->init();
    level::get()->init();
}

LONG hk_get_recoil(_EXCEPTION_POINTERS* Info)
{
    printf("hook on fire\n");
    return EXCEPTION_CONTINUE_EXECUTION;
}

void cheat::hook()
{
    do
    {
        gvalue::world = SDK::UWorld::GetWorld();
    } 
    while (!gvalue::world);

    gvalue::vtb = (void**)gvalue::world->OwningGameInstance->LocalPlayers[0]->ViewportClient->VTable;
    DWORD protect = 0;
    VirtualProtect(&gvalue::vtb[gconst::post_render_index], 8, PAGE_EXECUTE_READWRITE, &protect);

    gvalue::def_post_render = (fn_post_render)(gvalue::vtb[gconst::post_render_index]);
    gvalue::vtb[gconst::post_render_index] = &hk_post_render;

    gvalue::def_wnd_proc = (WNDPROC)SetWindowLongPtrA(kismet::get_window(), GWLP_WNDPROC, (LONG_PTR)hk_wnd_proc);
}

void cheat::exit()
{
}

void cheat::hk_post_render(void* thisptr, SDK::UCanvas* canvas)
{
    __try
    {
        gvalue::def_post_render(thisptr, canvas);

        gvalue::world = SDK::UWorld::GetWorld();
        gvalue::controller = SDK::UGameplayStatics::GetPlayerController(gvalue::world, 0);
        gvalue::canvas = canvas;
        gvalue::engine = SDK::UEngine::GetEngine();
        gvalue::delta_time = SDK::UGameplayStatics::GetWorldDeltaSeconds(gvalue::world);

        gui::main();

        network::get()->main();
        visual::get()->main();
        aim::get()->main();
        player::get()->main();
        level::get()->main();
        menu::get()->main();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("error code : %d", GetExceptionCode());
    }
}

LRESULT cheat::hk_wnd_proc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    switch (u_msg)
    {
    case WM_KEYDOWN:
        if (w_param == VK_INSERT)
        {
            gvalue::menu_open = !gvalue::menu_open;
            break;
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
            return TRUE;
        }
    }
    return CallWindowProcA(gvalue::def_wnd_proc, hwnd, u_msg, w_param, l_param);
}