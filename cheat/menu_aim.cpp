#include "menu_mod.h"
#include "menu_lib.h"
#include "gvalue.h"

namespace aim
{
	void draw_legal()
	{
		menu_lib::set_font(12);

		menu_lib::pice(SDK::FVector2D(110, 10), SDK::FVector2D(280, 310), true);

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 20), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_enable, L"自瞄总开关，不影响无后座等功能，可以通过下方选项配置具体细节");
			menu_lib::text(SDK::FVector2D(150, 20), L"启用自瞄");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 50), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_wall_cond, L"开启后仅自瞄可以看见的对象，不会自瞄墙后的对象");
			menu_lib::text(SDK::FVector2D(150, 50), L"可视性检测");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 80), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_down, L"开启后将自瞄倒地的对象，也就是死亡后仍然自瞄");
			menu_lib::text(SDK::FVector2D(150, 80), L"自瞄倒地");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 110), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_use_smooth, L"开启后平滑自瞄速度，关闭后即直接暴力瞄准");
			menu_lib::text(SDK::FVector2D(150, 110), L"启用平滑");
		}

		{
			const std::wstring wstr = L"平滑：" + std::to_wstring((int)gvalue::aim_smooth);
			menu_lib::text(SDK::FVector2D(120, 140), wstr.c_str());
			menu_lib::drag_bar("aim_smooth", SDK::FVector2D(200, 140), SDK::FVector2D(180, 20), SDK::FVector2D(10, 20), &gvalue::aim_smooth, 0, 30, L"平滑速度，越高瞄准越快");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 170), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_use_radius, L"开启后自瞄范围内对象，不开则自瞄所有");
			menu_lib::text(SDK::FVector2D(150, 170), L"启用范围");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 200), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_radius_draw, L"开启后绘制自瞄范围和自瞄对象");
			menu_lib::text(SDK::FVector2D(150, 200), L"绘制范围");
		}

		{
			const std::wstring wstr = L"范围：" + std::to_wstring((int)gvalue::aim_radius);
			menu_lib::text(SDK::FVector2D(120, 230), wstr.c_str());
			menu_lib::drag_bar("aim_radius", SDK::FVector2D(220, 230), SDK::FVector2D(160, 20), SDK::FVector2D(10, 20), &gvalue::aim_radius, 0, 900, L"自瞄范围，越大范围越广");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 260), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_left_button, L"按下鼠标左键时开启自瞄");
			menu_lib::text(SDK::FVector2D(150, 260), L"左键");

			menu_lib::check_box(" ", SDK::FVector2D(220, 260), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_right_button, L"按下鼠标右键时开启自瞄");
			menu_lib::text(SDK::FVector2D(250, 260), L"右键");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 290), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_side_button, L"按下鼠标侧键时开启自瞄");
			menu_lib::text(SDK::FVector2D(150, 290), L"侧键");

			menu_lib::check_box(" ", SDK::FVector2D(220, 290), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_alt, L"按下ALT键时开启自瞄");
			menu_lib::text(SDK::FVector2D(250, 290), L"ALT");
		}
	}

	void draw_mem()
	{
		menu_lib::pice(SDK::FVector2D(110, 330), SDK::FVector2D(280, menu_lib::size.Y - 340), true);

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 340), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_no_recoil, L"关闭枪械后坐力");
			menu_lib::text(SDK::FVector2D(150, 340), L"无后座");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 370), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_no_delta, L"关闭枪械跟手时的瞄准滞后");
			menu_lib::text(SDK::FVector2D(150, 370), L"关闭滞后");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 400), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_inf_bullet, L"枪械无限子弹，如果你不是房主，在你正常子弹打光后，其他人视角你没有开枪，但是你仍然可以造成伤害");
			menu_lib::text(SDK::FVector2D(150, 400), L"无限子弹");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 430), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_fire_rate, L"增加枪械射速，也就是将枪械开火间隔设置为0.01s");
			menu_lib::text(SDK::FVector2D(150, 430), L"射速");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 460), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_magic_bullet, L"无视任何阻挡打人，测试中可能会无效");
			menu_lib::text(SDK::FVector2D(150, 460), L"魔发子弹[测试]");
		}
	}

	void draw_aim_option()
	{
		menu_lib::pice(SDK::FVector2D(400, 10), SDK::FVector2D(190, 120), true);

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 20), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_player);
			menu_lib::text(SDK::FVector2D(440, 20), L"自瞄队友");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 50), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_civilian);
			menu_lib::text(SDK::FVector2D(440, 50), L"自瞄平民");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 80), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_suspect);
			menu_lib::text(SDK::FVector2D(440, 80), L"自瞄嫌疑人");
		}
	}

	void draw_body()
	{
		menu_lib::pice(SDK::FVector2D(400, 140), SDK::FVector2D(190, menu_lib::size.Y - 150), true);

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 150), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_head, L"如果与其他自瞄部位一起开启可能会降低帧数");
			menu_lib::text(SDK::FVector2D(440, 150), L"自瞄头部");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 180), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_spine, L"如果与其他自瞄部位一起开启可能会降低帧数");
			menu_lib::text(SDK::FVector2D(440, 180), L"自瞄胸部");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 210), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_pelvis, L"如果与其他自瞄部位一起开启可能会降低帧数");
			menu_lib::text(SDK::FVector2D(440, 210), L"自瞄腹部");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 240), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_hand, L"如果与其他自瞄部位一起开启可能会降低帧数");
			menu_lib::text(SDK::FVector2D(440, 240), L"自瞄手部");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(410, 270), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::aim_foot, L"如果与其他自瞄部位一起开启可能会降低帧数");
			menu_lib::text(SDK::FVector2D(440, 270), L"自瞄足部");
		}
	}
}

void menu_mod::aim()
{
	aim::draw_legal();
	aim::draw_mem();
	aim::draw_aim_option();
	aim::draw_body();
}