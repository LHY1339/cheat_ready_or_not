#pragma once
#include <Windows.h>
#include <string>

using fn_post_render = void(__thiscall*)(void*, void*);
using fn_wnd_proc = WNDPROC;

using fn_get_recoil = void(__fastcall*)(__int64 a1, __int64 a2);

namespace SDK
{
	class UWorld;
	class APlayerController;
	class UCanvas;
	class UEngine;
}

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

namespace gvalue
{
	inline HINSTANCE dll_inst;
	inline void** viewport_client_vtb;
	inline fn_post_render def_post_render;
	inline fn_wnd_proc def_wnd_proc;
	inline fn_get_recoil def_get_recoil;
	inline HWND game_window;

	inline SDK::UWorld* world;
	inline SDK::APlayerController* controller;
	inline SDK::UCanvas* canvas;
	inline SDK::UEngine* engine;
	inline float delta_time;

	inline bool menu_open = true;
	inline s_mouse mouse = {};
	inline std::string version;
	inline std::wstring tip_text = L"";

	inline s_esp_character esp_team;
	inline s_esp_character esp_suspect;
	inline s_esp_character esp_civilian;
	inline s_esp_item esp_trap;
	inline s_esp_item esp_report;
	inline s_esp_item esp_bomb;
	inline s_esp_item esp_weapon;
	inline s_esp_item esp_other;
	inline float esp_distance = 50;

	inline bool tpp = false;
	inline float tpp_camera_x = 0.0f;
	inline float tpp_camera_y = 0.0f;
	inline float tpp_camera_arm = 300.0f;
	inline bool tpp_camera_collision = true;
	inline float first_fov = 120.0f;
	inline float third_fov = 120.0f;
	inline bool disable_post = false;

	inline bool aim_enable = true;
	inline bool aim_wall_cond = true;
	inline bool aim_down = false;
	inline bool aim_use_smooth = true;
	inline float aim_smooth = 10;
	inline bool aim_use_radius = true;
	inline bool aim_radius_draw = true;
	inline float aim_radius = 200;

	inline bool aim_left_button = false;
	inline bool aim_right_button = true;
	inline bool aim_side_button = false;
	inline bool aim_alt = false;

	inline bool aim_no_recoil = true;
	inline bool aim_no_delta = true;
	inline bool aim_inf_bullet = false;
	inline bool aim_fire_rate = false;
	inline bool aim_magic_bullet = false;

	inline bool aim_player = false;
	inline bool aim_civilian = false;
	inline bool aim_suspect = true;

	inline bool aim_head = true;
	inline bool aim_spine = false;
	inline bool aim_pelvis = false;
	inline bool aim_hand = false;
	inline bool aim_foot = false;

	inline float global_speed = 1.0f;
}