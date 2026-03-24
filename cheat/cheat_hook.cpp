#include "cheat_mod.h"
#include "sdk/SDK/Engine_classes.hpp"
#include "gvalue.h"
#include "gdefine.h"
#include "kismet.h"
#include "cheat.h"
#include "input.h"
#include "minhook/include/MinHook.h"

#include <Windows.h>

void hk_post_render(void* thisptr, SDK::UCanvas* canvas)
{
    gvalue::def_post_render(thisptr, canvas);
    __try
    {
        gvalue::world = SDK::UWorld::GetWorld();
        gvalue::canvas = canvas;
        gvalue::engine = SDK::UEngine::GetEngine();
        gvalue::controller = SDK::UGameplayStatics::GetPlayerController(gvalue::world, 0);
        gvalue::delta_time = SDK::UGameplayStatics::GetWorldDeltaSeconds(gvalue::world);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        printf("hk_post_render error : %d\n", GetExceptionCode());
    }

    cheat::get()->module_main();
}

LRESULT CALLBACK hk_wnd_proc(HWND hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    const bool success = input::get()->main(hwnd, u_msg, w_param, l_param);
    if (!success)
    {
        return TRUE;
    }
    return CallWindowProcA(gvalue::def_wnd_proc, hwnd, u_msg, w_param, l_param);
}

void __fastcall hk_get_recoil(__int64 a1, __int64 a2)
{
    if (!a2)
    {
        return;
    }
    *(double*)(a2 + 0) = 0.0;
    *(double*)(a2 + 8) = 0.0;
    *(double*)(a2 + 16) = 0.0;
    gvalue::def_get_recoil(a1, a2);
    if (gvalue::aim_no_recoil)
    {
        *(double*)(a2 + 0) = 0.0;
        *(double*)(a2 + 8) = 0.0;
        *(double*)(a2 + 16) = 0.0;
    }
}

void cheat_mod::hook()
{
    do
    {
        gvalue::game_window = kismet::find_window();
        Sleep(100);
    } 
    while (!gvalue::game_window);

    do
    {
        gvalue::world = SDK::UWorld::GetWorld();
        Sleep(100);
    } 
    while (!gvalue::world);

    {
        gvalue::viewport_client_vtb = *(void***)gvalue::world->OwningGameInstance->LocalPlayers[0]->ViewportClient;
        DWORD protect = 0;
        VirtualProtect(&gvalue::viewport_client_vtb[POST_RENDER_INDEX], 8, PAGE_EXECUTE_READWRITE, &protect);
        gvalue::def_post_render = (fn_post_render)(gvalue::viewport_client_vtb[POST_RENDER_INDEX]);
        gvalue::viewport_client_vtb[POST_RENDER_INDEX] = &hk_post_render;
    }

    {
        gvalue::def_wnd_proc = (WNDPROC)SetWindowLongPtrA(kismet::get_window(), GWLP_WNDPROC, (LONG_PTR)hk_wnd_proc);
    }

    uintptr_t base_addr = (uintptr_t)GetModuleHandle(NULL);

    MH_Initialize();

    {
        uintptr_t target_addr = base_addr + 0x49AA7F0;
        MH_CreateHook((LPVOID)target_addr, &hk_get_recoil, reinterpret_cast<LPVOID*>(&gvalue::def_get_recoil));
        MH_EnableHook((LPVOID)target_addr);
    }

}