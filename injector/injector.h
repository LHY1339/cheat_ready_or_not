#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>

class injector
{
public:
	static injector* get();
	int main();
private:
	void init();
	DWORD find_process();
	BOOL inject(const wchar_t* dll_path, DWORD proc_id);
	std::wstring get_abs_path(const std::filesystem::path& rel_path);
	bool is_load(DWORD pid, const wchar_t* dll_name);
};

