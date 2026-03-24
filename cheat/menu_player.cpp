#include "menu_mod.h"
#include "menu_lib.h"
#include "gvalue.h"
#include "kismet.h"

namespace player
{
	void draw_speed()
	{
		{
			menu_lib::pice(SDK::FVector2D(110, 10), SDK::FVector2D(280, 70), true);

			const std::wstring wstr = L"全局加速：" + kismet::fround_to_string(gvalue::global_speed);
			menu_lib::text(SDK::FVector2D(120, 20), wstr.c_str());
			menu_lib::drag_bar("global_speed", SDK::FVector2D(120, 50), SDK::FVector2D(260, 20), SDK::FVector2D(10, 20), &gvalue::global_speed, 0, 20, L"全局加速你的角色时间，包括运动速度等，其他玩家和世界时间不受影响");
		}
	}
}

void menu_mod::player()
{
	player::draw_speed();
}