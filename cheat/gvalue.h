#pragma once
#include <Windows.h>

using fn_post_render = void(__thiscall*)(void*, void*);
using fn_wnd_proc = WNDPROC;

struct s_character_visual
{
	bool enable = true;
	bool box = true;
	bool skeleton = true;
	bool name = true;
	bool status = true;
	bool distance = true;
	bool line = false;
	bool extent = false;
};

struct s_item_visual
{
	bool enable = true;
	bool circle = true;
	bool name = true;
	bool status = true;
	bool distance = true;
	bool line = false;
};

struct s_mouse
{
	int x = 0;
	int y = 0;
	bool left = false;
	bool right = false;
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
	inline fn_post_render def_post_render;
	inline fn_wnd_proc def_wnd_proc;
	inline HINSTANCE dll_inst;

	inline bool menu_open = true;
	inline s_mouse mouse_input = {};

	inline SDK::UWorld* world;
	inline SDK::APlayerController* controller;
	inline SDK::UCanvas* canvas;
	inline SDK::UEngine* engine;

	inline bool draw_mesh = true;
	inline int visual_distance = 100;
	inline s_character_visual visual_player;
	inline s_character_visual visual_suspect;
	inline s_character_visual visual_civilian;
}