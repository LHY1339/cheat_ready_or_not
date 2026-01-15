#include "kismet.h"

#include "gvalue.h"
#include "_sdk.h"

#include <Windows.h>

bool kismet::is_key_down(const int& key)
{
	return (GetAsyncKeyState(key) & 0x8000) && GetForegroundWindow() == get_window();
}

HWND kismet::get_window()
{
	return FindWindowW(L"UnrealWindow", L"Ready Or Not  ");
}

void kismet::print_function(SDK::UClass* uclass, const char* class_name, const char* func_name)
{
	SDK::UFunction* func = nullptr;
	func = uclass->GetFunction(class_name, func_name);

	uintptr_t base_addr = (uintptr_t)GetModuleHandle(nullptr);
	uintptr_t func_addr = (uintptr_t)func->ExecFunction;
	uintptr_t offset = func_addr - base_addr;

	printf("%s->%s : 0x%llX\n", class_name, func_name, offset);
}
