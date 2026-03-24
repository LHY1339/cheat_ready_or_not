#include "menu.h"
#include "menu_lib.h"
#include "gui.h"
#include "render.h"
#include "gvalue.h"
#include "gdefine.h"
#include "color.h"
#include "menu_mod.h"

enum class menu_page
{
	esp,
	visual,
	aim,
	player,
	level,
	misc,
	config,
};

menu_page page = menu_page::esp;
float current_y = 0.0f;

menu* menu::get()
{
	static menu inst;
	return &inst;
}

void menu::main()
{
	menu_lib::size = SDK::FVector2D(600, 500);
	gvalue::tip_text = L"";
	if (gvalue::menu_open)
	{
		background();
		basic();
		left();
		tip();
		cursor();
	}
	lable();
}

void menu::background()
{
	gui::drag(
		menu_lib::pos,
		menu_lib::size,
		menu_lib::drag_pos,
		menu_lib::is_drag
	);

	render::fill_box(
		SDK::FVector2D(menu_lib::pos.X - 2, menu_lib::pos.Y - 2),
		SDK::FVector2D(menu_lib::size.X + 4, menu_lib::size.Y + 4),
		color::get()->outline_col
	);

	render::fill_box(
		SDK::FVector2D(menu_lib::pos.X, menu_lib::pos.Y),
		SDK::FVector2D(menu_lib::size.X, menu_lib::size.Y),
		color::get()->back_col
	);
}

void menu::basic()
{
	switch (page)
	{
	case menu_page::esp:
		menu_mod::esp();
		break;
	case menu_page::visual:
		menu_mod::visual();
		break;
	case menu_page::aim:
		menu_mod::aim();
		break;
	case menu_page::player:
		menu_mod::player();
		break;
	case menu_page::level:
		menu_mod::level();
		break;
	case menu_page::misc:
		menu_mod::misc();
		break;
	case menu_page::config:
		menu_mod::config();
		break;
	}
}

void menu::left()
{
	menu_lib::set_font(12);

	menu_lib::pice(SDK::FVector2D(0, 0), SDK::FVector2D(100, menu_lib::size.Y));

	const float target_y = 0 + (int)page * 40;
	current_y = current_y + (target_y - current_y) * 20 * gvalue::delta_time;
	render::fill_box(menu_lib::attach(0, current_y), SDK::FVector2D(100, 40), color::get()->back_col);

#define BUTTON(_page_,_name_,_ypos_) \
if (menu_lib::button_text(#_page_, SDK::FVector2D(10, _ypos_), SDK::FVector2D(80, 40), L#_name_)) \
{ \
	page = menu_page::_page_; \
}

	BUTTON(esp, 透视, 0);
	BUTTON(visual, 视觉, 40);
	BUTTON(aim, 瞄准, 80);
	BUTTON(player, 玩家, 120);
	BUTTON(level, 关卡, 160);
	BUTTON(misc, 杂项, 200);
	BUTTON(config, 配置, 240);

#undef BUTTON

	if (menu_lib::button_text("", SDK::FVector2D(10, menu_lib::size.Y - 40), SDK::FVector2D(80, 30), L"LHY1339 ",L"点击此处赞助作者爱发电 QwQ"))
	{
		system("start https://ifdian.net/a/lhy1339");
	}
}

void menu::tip()
{
	menu_lib::set_font(12);

	auto one_tip = [&](const int& y_pos, const int& x_size, const UC::FString& str)
		{
			render::fill_box(
				SDK::FVector2D(menu_lib::pos.X - 2, menu_lib::pos.Y + menu_lib::size.Y + y_pos - 2), 
				SDK::FVector2D(x_size + 4, 39),
				SDK::FLinearColor(0.4f, 0.1f, 0.0f, 1.0f)
			);

			render::fill_box(
				SDK::FVector2D(menu_lib::pos.X, menu_lib::pos.Y + menu_lib::size.Y + y_pos),
				SDK::FVector2D(x_size, 35),
				SDK::FLinearColor(0.1f, 0.06f, 0.0f, 1.0f)
			);

			menu_lib::text(SDK::FVector2D(10, menu_lib::size.Y + y_pos + 8), str);
		};

	one_tip(10, menu_lib::size.X, L"本修改器永久免费开源，问题反馈QQ群：1071845133");
	if (gvalue::version != "" && gvalue::version != CHEAT_VERSION)
	{
		one_tip(55, 165, L"有新版本可以下载！");
	}
}

void menu::cursor()
{
	render::fill_box(
		SDK::FVector2D(gvalue::mouse.x - 2, gvalue::mouse.y - 2),
		SDK::FVector2D(4, 4),
		color::get()->cursor_col
	);

	if (gvalue::tip_text != L"")
	{
		menu_lib::set_font(10);

		const SDK::FVector2D tip_size = gvalue::canvas->K2_TextSize(gvalue::engine->TinyFont, gvalue::tip_text.c_str(), SDK::FVector2D(1, 1));

		constexpr float x_multi = 1.08;
		constexpr float x_add = 6;

		render::fill_box(
			SDK::FVector2D(gvalue::mouse.x + 8, gvalue::mouse.y - 12),
			SDK::FVector2D(tip_size.X * x_multi + x_add + 4, 24), 
			color::get()->outline_col
		);

		render::fill_box(
			SDK::FVector2D(gvalue::mouse.x + 10, gvalue::mouse.y - 10),
			SDK::FVector2D(tip_size.X * x_multi + x_add, 20),
			color::get()->back_col
		);

		menu_lib::text(
			SDK::FVector2D(gvalue::mouse.x + 14, gvalue::mouse.y - 7), 
			gvalue::tip_text.c_str(),
			false,
			false,
			false
		);
	}
}

void menu::lable()
{
	menu_lib::set_font(12);

	render::draw_text(
		gvalue::engine->TinyFont,
		L"按下 Ins（Insert）显示/隐藏菜单",
		SDK::FVector2D(10.0f, 10.0f),
		SDK::FVector2D(1.0f, 1.0f),
		SDK::FLinearColor(1, 1, 1, 1),
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
		SDK::FVector2D(0.0f, 0.0f),
		false,
		false,
		false,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
	);
}