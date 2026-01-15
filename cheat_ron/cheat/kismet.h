#pragma once
#include <Windows.h>

namespace SDK
{
	class UClass;
}

class kismet
{
public:
	static bool is_key_down(const int& key);
	static HWND get_window();
	static void print_function(SDK::UClass* uclass, const char* class_name, const char* func_name);
};