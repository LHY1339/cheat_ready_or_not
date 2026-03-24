#include "menu_mod.h"
#include "menu_lib.h"
#include "gvalue.h"

namespace visual
{
	void draw_tpp()
	{
		menu_lib::set_font(12);

		menu_lib::pice(SDK::FVector2D(110, 10), SDK::FVector2D(280, 200), true);

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 20), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::tpp, L"是否开启第三人称视角，可以通过下方选项配置具体细节");
			menu_lib::text(SDK::FVector2D(150, 20), L"第三人称");
		}

		{
			const std::wstring wstr = L"X偏移：" + std::to_wstring((int)gvalue::tpp_camera_x);
			menu_lib::text(SDK::FVector2D(120, 50), wstr.c_str());
			menu_lib::drag_bar("tpp_camera_x", SDK::FVector2D(220, 50), SDK::FVector2D(160, 20), SDK::FVector2D(10, 20), &gvalue::tpp_camera_x, -900, 900, L"第三人称视角的X轴偏移");
		}

		{
			const std::wstring wstr = L"Y偏移：" + std::to_wstring((int)gvalue::tpp_camera_y);
			menu_lib::text(SDK::FVector2D(120, 80), wstr.c_str());
			menu_lib::drag_bar("tpp_camera_y", SDK::FVector2D(220, 80), SDK::FVector2D(160, 20), SDK::FVector2D(10, 20), &gvalue::tpp_camera_y, -900, 900, L"第三人称视角的Y轴偏移");
		}

		{
			const std::wstring wstr = L"摇臂长度：" + std::to_wstring((int)gvalue::tpp_camera_arm);
			menu_lib::text(SDK::FVector2D(120, 110), wstr.c_str());
			menu_lib::drag_bar("tpp_camera_arm", SDK::FVector2D(240, 110), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::tpp_camera_arm, 0, 900, L"第三人称视角距离角色的长度");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 140), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::tpp_camera_collision, L"是否启用碰撞检测，如果开启视角会模拟被墙壁阻挡");
			menu_lib::text(SDK::FVector2D(150, 140), L"摇臂碰撞");
		}

		if (menu_lib::button_color_text(" ", SDK::FVector2D(120, 170), SDK::FVector2D(260, 30), L"重置第三人称位置", L"将第三人称视角的XY偏移还原，不会还原摇臂长度"))
		{
			gvalue::tpp_camera_x = 0;
			gvalue::tpp_camera_y = 0;
		}
	}

	void draw_camera()
	{
		menu_lib::set_font(12);

		menu_lib::pice(SDK::FVector2D(110, 220), SDK::FVector2D(280, menu_lib::size.Y - 230), true);

		{
			const std::wstring wstr = L"一称FOV：" + std::to_wstring((int)gvalue::first_fov);
			menu_lib::text(SDK::FVector2D(120, 230), wstr.c_str());
			menu_lib::drag_bar("first_fov", SDK::FVector2D(240, 230), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::first_fov, 0, 180, L"第一人称的视野范围，不建议调0或者180");
		}

		{
			const std::wstring wstr = L"三称FOV：" + std::to_wstring((int)gvalue::third_fov);
			menu_lib::text(SDK::FVector2D(120, 260), wstr.c_str());
			menu_lib::drag_bar("third_fov", SDK::FVector2D(240, 260), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::third_fov, 0, 180, L"第三人称的视野范围，不建议调0或者180");
		}

		{
			menu_lib::check_box(" ", SDK::FVector2D(120, 290), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::disable_post, L"禁用第一人称的后期处理，例如调色、视觉效果、镜头黑边等");
			menu_lib::text(SDK::FVector2D(150, 290), L"禁用后处理");
		}
	}

	void draw_other()
	{
		menu_lib::set_font(10);

		menu_lib::pice(SDK::FVector2D(400, 10), SDK::FVector2D(190, menu_lib::size.Y - 20), true);

		menu_lib::text(SDK::FVector2D(410, 20), L"这里不知道放啥了\n有建议可以在群里告诉我\n(>_<)");
	}
}

void menu_mod::visual()
{
	visual::draw_tpp();
	visual::draw_camera();
	visual::draw_other();
}