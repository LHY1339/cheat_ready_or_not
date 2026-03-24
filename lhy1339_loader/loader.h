#pragma once
#include <Windows.h>
#include <filesystem>

class loader
{
public:
	static loader* get();
	int main();
private:
	void init_console();
	void init_file(const char* file_path);
	void unzip_resource(const LPCWSTR& res_name, const LPCWSTR& res_type, const char* file_path);
	void set_console_color(WORD color);
	DWORD find_process();
	BOOL inject(const wchar_t* dll_path, DWORD proc_id);
	std::wstring get_abs_path(const std::filesystem::path& rel_path);
	bool is_load(DWORD pid, const wchar_t* dll_name);
};

