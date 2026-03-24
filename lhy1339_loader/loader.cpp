#include "loader.h"
#include "gdefine.h"
#include "resource.h"

#include <TlHelp32.h>
#include <tchar.h>
#include <iostream>
#include <fstream>

loader* loader::get()
{
    static loader inst;
    return &inst;
}

int loader::main()
{
    init_console();
    init_file(FILE_PATH);
    unzip_resource(MAKEINTRESOURCE(IDR_DLL1), L"DLL", DLL_PATH);

    while (true)
    {
        Sleep(1000);
        const DWORD pid = find_process();
        if (!pid)
        {
            continue;
        }
        std::wstring dll_path = get_abs_path(DLL_PATH);
        if (is_load(pid, L"cheat.dll"))
        {
            set_console_color(FOREGROUND_RED | FOREGROUND_GREEN);
            printf("[pid : %d] 请勿重复加载\n", static_cast<int>(pid));
            break;
        }
        if (inject(dll_path.c_str(), pid))
        {
            set_console_color(FOREGROUND_GREEN);
            printf("[pid : %d] 加载成功\n", static_cast<int>(pid));
            break;
        }
    }
    Sleep(1000);
    return 0;
}

void loader::init_console()
{
    SetConsoleTitle(L"[LHY1339] 修改器");

    HWND hwnd = GetConsoleWindow();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX;
    style &= ~WS_SIZEBOX;
    SetWindowLong(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, NULL, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

    printf(R"(
 /##       /##   /## /##     /##  /##    /######   /######   /###### 
| ##      | ##  | ##|  ##   /##//####   /##__  ## /##__  ## /##__  ##
| ##      | ##  | ## \  ## /##/|_  ##  |__/  \ ##|__/  \ ##| ##  \ ##
| ##      | ########  \  ####/   | ##     /#####/   /#####/|  #######
| ##      | ##__  ##   \  ##/    | ##    |___  ##  |___  ## \____  ##
| ##      | ##  | ##    | ##     | ##   /##  \ ## /##  \ ## /##  \ ##
| ########| ##  | ##    | ##    /######|  ######/|  ######/|  ######/
|________/|__/  |__/    |__/   |______/ \______/  \______/  \______/ 


)");
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("- 游戏：");
    set_console_color(FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf(GAME_NAME);
    printf("\n\n");
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("--------------------------------------------------------\n\n");
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN);
    printf("- 免责声明：本修改器仅供学习与娱乐使用，切勿破坏游戏环境，使用产生的任何后果作者概不负责\n\n");
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("--------------------------------------------------------\n\n");
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN);
    printf("- 请以管理员身份运行此程序！！！\n\n");
    printf("- 请关闭或卸载所有杀毒软件（包括Windows自带的Defender）\n\n");
    printf("- 请关闭或卸载任何反作弊软件（EAC、ACE、BE、完美、5E等），否则可能导致加载失败或误封\n\n");
    set_console_color(FOREGROUND_GREEN);
    printf("- 任何问题加QQ群：1071845133\n\n");
    set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("--------------------------------------------------------\n\n");
    printf("- 请运行游戏，修改器会自动加载\n\n");
}

void loader::init_file(const char* file_path)
{
    std::filesystem::path out_path(file_path);
    std::filesystem::path dir = out_path.parent_path();

    if (!dir.empty() && !std::filesystem::exists(dir))
    {
        if (!std::filesystem::create_directories(dir))
        {
            return;
        }
    }
}

void loader::unzip_resource(const LPCWSTR& res_name, const LPCWSTR& res_type, const char* file_path)
{
    HMODULE handle = GetModuleHandle(nullptr);
    HRSRC h_res = FindResourceW(handle, res_name, res_type);
    HGLOBAL h_data = LoadResource(handle, h_res);
    void* p_data = LockResource(h_data);
    DWORD size = SizeofResource(handle, h_res);
    std::ofstream out(file_path, std::ios::binary);
    out.write(reinterpret_cast<char*>(p_data), size);
    out.close();
}

void loader::set_console_color(WORD color)
{
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_console, color);
}

DWORD loader::find_process()
{
    DWORD pid = 0;
    HWND hwnd = FindWindowW(WINDOW_CLASS_NAME, WINDOW_NAME);
    if (!hwnd)
    {
        return FALSE;
    }
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

BOOL loader::inject(const wchar_t* dll_path, DWORD proc_id)
{
    HANDLE h_proc = OpenProcess(PROCESS_ALL_ACCESS, TRUE, proc_id);
    if (!h_proc)
    {
        return FALSE;
    }
    SIZE_T dll_path_len = (_tcslen(dll_path) + 1) * sizeof(wchar_t);
    LPVOID write_addr = VirtualAllocEx(h_proc, NULL, dll_path_len, MEM_COMMIT, PAGE_READWRITE);
    if (!write_addr)
    {
        return FALSE;
    }
    if (!WriteProcessMemory(h_proc, write_addr, dll_path, dll_path_len, NULL))
    {
        return FALSE;
    }
    PTHREAD_START_ROUTINE fn_addr = (PTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(L"kernel32.dll"),
        "LoadLibraryW"
    );
    if (!fn_addr)
    {
        return FALSE;
    }
    HANDLE h_thread = CreateRemoteThreadEx(h_proc, NULL, NULL, fn_addr, write_addr, NULL, NULL, NULL);
    if (!h_thread)
    {
        return FALSE;
    }
    WaitForSingleObject(h_thread, INFINITE);
    CloseHandle(h_thread);
    CloseHandle(h_proc);
    return TRUE;
}

std::wstring loader::get_abs_path(const std::filesystem::path& rel_path)
{
    return std::filesystem::absolute(rel_path).wstring();
}

bool loader::is_load(DWORD pid, const wchar_t* dll_name)
{
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (h_snap == INVALID_HANDLE_VALUE) return false;

    MODULEENTRY32W me32;
    me32.dwSize = sizeof(me32);

    if (Module32FirstW(h_snap, &me32))
    {
        do
        {
            if (_wcsicmp(me32.szModule, dll_name) == 0)
            {
                CloseHandle(h_snap);
                return true;
            }
        } while (Module32NextW(h_snap, &me32));
    }
    CloseHandle(h_snap);
    return false;
}
