#include "menu.h"

#include "render.h"
#include "_sdk.h"
#include "gvalue.h"
#include "glanguage.h"
#include "gui.h"

enum class e_page
{
	visual,
	aim,
	player,
	misc
};

float menu_x = 100.0f;
float menu_y = 100.0f;
float menu_w = 600.0f;
float menu_h = 400.0f;

bool left_down = false;
bool menu_drag = false;
float drag_x = 0.0f;
float drag_y = 0.0f;

e_page page = e_page::visual;

void menu::main()
{
	lable();
	base();
}

void menu::lable()
{
	render::draw_text(
		gvalue::engine->MediumFont,
		glanguage::lable,
		SDK::FVector2D(10.0f, 10.0f),
		SDK::FVector2D(1.0f, 1.0f),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
		SDK::FVector2D(0.0f, 0.0f),
		false,
		false,
		false,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
	);
}

void menu::base()
{
	if (!gvalue::menu_open)
	{
		return;
	}

	base_logic();
	base_draw();
	base_cursor();
}

void menu::base_logic()
{
	if (gvalue::mouse_input.left)
	{
		if (!left_down)
		{
			if (gvalue::mouse_input.x > menu_x &&
				gvalue::mouse_input.y > menu_y &&
				gvalue::mouse_input.x < menu_x + menu_w &&
				gvalue::mouse_input.y < menu_y + menu_h)
			{
				menu_drag = true;
				drag_x = gvalue::mouse_input.x - menu_x;
				drag_y = gvalue::mouse_input.y - menu_y;
			}
			left_down = true;
		}
	}
	else
	{
		left_down = false;
		menu_drag = false;
	}

	if (menu_drag)
	{
		menu_x = gvalue::mouse_input.x - drag_x;
		menu_y = gvalue::mouse_input.y - drag_y;
	}
}

void menu::base_draw()
{
	render::fill_box(
		SDK::FVector2D(menu_x - 2, menu_y - 2),
		SDK::FVector2D(menu_w + 4, menu_h + 4),
		SDK::FLinearColor(0.2f, 0.2f, 0.2f, 1.0f)
	);
	render::fill_box(
		SDK::FVector2D(menu_x, menu_y),
		SDK::FVector2D(menu_w, menu_h),
		SDK::FLinearColor(0.01f, 0.01f, 0.01f, 1.0f)
	);

	left_bar();
	switch (page)
	{
	case e_page::visual:
		visual();
		break;
	case e_page::aim:
		aim();
		break;
	case e_page::player:
		player();
		break;
	case e_page::misc:
		misc();
		break;
	}
}

void menu::base_cursor()
{
	render::fill_box(
		SDK::FVector2D(gvalue::mouse_input.x - 2, gvalue::mouse_input.y - 2),
		SDK::FVector2D(4, 4),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
	);
}

void menu::left_bar()
{
	auto button_01 = [](const UC::FString& text, const SDK::FVector2D& pos)
		{
			const SDK::FLinearColor normal_col(0.04f, 0.04f, 0.04f, 1.0f);
			const SDK::FLinearColor hover_col(0.06f, 0.06f, 0.06f, 1.0f);
			const SDK::FLinearColor press_col(0.1f, 0.1f, 0.1f, 1.0f);
			const SDK::FLinearColor text_col(1.0f, 1.0f, 1.0f, 1.0f);

			return gui::button_color_text(
				pos,
				SDK::FVector2D(60, 20),
				text,
				gvalue::engine->MediumFont,
				text_col,
				normal_col,
				hover_col,
				press_col
			);
		};

	render::fill_box(
		SDK::FVector2D(menu_x, menu_y),
		SDK::FVector2D(80, menu_h),
		SDK::FLinearColor(0.02f, 0.02f, 0.02f, 1.0f)
	);

	render::draw_text(
		gvalue::engine->MediumFont,
		L"LHY1339",
		SDK::FVector2D(menu_x + 38, menu_y + menu_h - 20),
		SDK::FVector2D(1.0f, 1.0f),
		SDK::FLinearColor(0.5f, 0.5f, 0.5f, 1.0f),
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
		SDK::FVector2D(0.0f, 0.0f),
		true,
		true,
		false,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
	);

	if (gui::button(SDK::FVector2D(menu_x, menu_y + menu_h - 30), SDK::FVector2D(80, 20)))
	{
		system("start https://space.bilibili.com/669462200");
	}

	if (button_01(glanguage::visual, SDK::FVector2D(menu_x + 10, menu_y + 10)))
	{
		page = e_page::visual;
	}

	if (button_01(glanguage::aim, SDK::FVector2D(menu_x + 10, menu_y + 40)))
	{
		page = e_page::aim;
	}

	if (button_01(glanguage::player, SDK::FVector2D(menu_x + 10, menu_y + 70)))
	{
		page = e_page::player;
	}

	if (button_01(glanguage::misc, SDK::FVector2D(menu_x + 10, menu_y + 100)))
	{
		page = e_page::misc;
	}
}

void menu::visual()
{
}

void menu::aim()
{
}

void menu::player()
{
}

void menu::misc()
{
}
