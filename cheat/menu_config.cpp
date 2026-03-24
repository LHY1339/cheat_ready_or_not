#include "menu_mod.h"
#include "menu_lib.h"
#include "save.h"
#include "gdefine.h"

void menu_mod::config()
{
	menu_lib::pice(SDK::FVector2D(110, 10), SDK::FVector2D(180, menu_lib::size.Y - 20), true);

	if (menu_lib::button_color_text(" ", SDK::FVector2D(120, 20), SDK::FVector2D(160, 30), L"保存配置", L"保存你的配置，下次开启修改器自动加载"))
	{
		save::get()->save_mem(SAVE_PATH);
	}

	if (menu_lib::button_color_text(" ", SDK::FVector2D(120, 60), SDK::FVector2D(160, 30), L"加载配置", L"加载你保存过的配置"))
	{
		save::get()->load_mem(SAVE_PATH);
	}
}