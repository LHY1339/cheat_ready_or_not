#pragma once
#include <Windows.h>
#include <string>

using fn_post_render = void(__thiscall*)(void*, void*);
using fn_wnd_proc = WNDPROC;

struct s_mouse
{
	int x = 0;
	int y = 0;
	bool left = false;
};

struct s_esp
{
	bool enable = true;
	bool name = true;
	bool distance = true;
	bool status = true;
	bool line = false;
	bool extent = false;
};

struct s_esp_character :public s_esp
{
	bool box = true;
	bool skeleton = true;
};

struct s_esp_item :public s_esp
{
	bool circle = true;
};

namespace SDK
{
	class UWorld;
	class APlayerController;
	class UCanvas;
	class UEngine;
}

namespace gvalue
{
	inline void** vtb;
	inline HINSTANCE dll_inst;
	inline fn_post_render def_post_render;
	inline fn_wnd_proc def_wnd_proc;

	inline SDK::UWorld* world;
	inline SDK::APlayerController* controller;
	inline SDK::UCanvas* canvas;
	inline SDK::UEngine* engine;
	inline float delta_time = 0.0f;

	inline bool menu_open = true;
	inline bool is_exit = false;
	inline bool is_clean = false;
	inline s_mouse mouse = {};

	inline int color_index = 0;
	inline std::string max_version = "";
	inline std::string log = "";
	inline bool draw_lable = true;

	inline s_esp_character esp_player;
	inline s_esp_character esp_suspect;
	inline s_esp_character esp_civilian;
	inline s_esp_item esp_trap;
	inline s_esp_item esp_report;
	inline s_esp_item esp_bomb;
	inline s_esp_item esp_weapon;
	inline float esp_distance = 0.2f;
	inline float fov = 0.67f;
	inline bool disable_post = false;
	inline bool third_person = false;
	inline float cam_x = 0.5f;
	inline float cam_y = 0.5f;
	inline float cam_distance = 0.2f;

	inline bool aim_enable = true;
	inline bool aim_visibility = true;
	inline bool aim_down = false;
	inline bool aim_use_smooth = true;
	inline float aim_smooth = 0.5f;
	inline bool aim_use_radius = true;
	inline float aim_radius = 0.3f;
	inline bool aim_draw = true;
	inline bool aim_no_recoil = false;
	inline bool aim_inf_bullet = false;
	inline bool aim_fire_rate = false;

	inline bool aim_player = false;
	inline bool aim_civilian = false;
	inline bool aim_suspect = true;
}