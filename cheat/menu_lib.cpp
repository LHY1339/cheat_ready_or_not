#include "menu_lib.h"

#include "gvalue.h"
#include "gui.h"
#include "color.h"
#include "render.h"

void menu_lib::set_font(const int& size)
{
	gvalue::engine->TinyFont->LegacyFontSize = size;
}

SDK::FVector2D menu_lib::attach(const float& ref_x, const float& ref_y)
{
	return SDK::FVector2D(pos.X + ref_x, pos.Y + ref_y);
}

bool menu_lib::button_color(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const std::wstring& tip, bool is_attach)
{
	return gui::button_color(
		name,
		is_attach ? attach(pos.X, pos.Y) : pos,
		SDK::FVector2D(size.X, size.Y),
		color::get()->normal_col,
		color::get()->hover_col,
		color::get()->press_col,
		tip
	);
}

bool menu_lib::button_text(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FString& str, const std::wstring& tip, bool is_attach)
{
	return gui::button_text(
		name,
		is_attach ? attach(pos.X, pos.Y) : pos,
		SDK::FVector2D(size.X, size.Y),
		str,
		gvalue::engine->TinyFont,
		color::get()->text_col,
		color::get()->text_col,
		color::get()->text_col,
		tip
	);
}

bool menu_lib::button_color_text(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FString& str, const std::wstring& tip, bool is_attach)
{
	return gui::button_color_text(
		name,
		is_attach ? attach(pos.X, pos.Y) : pos,
		SDK::FVector2D(size.X, size.Y),
		str,
		gvalue::engine->TinyFont,
		color::get()->text_col,
		color::get()->normal_col,
		color::get()->hover_col,
		color::get()->press_col,
		tip
	);
}

void menu_lib::check_box(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FVector2D& check_size, bool* ptr, const std::wstring& tip, bool is_attach)
{
	gui::check_box(
		name,
		is_attach ? attach(pos.X, pos.Y) : pos,
		SDK::FVector2D(size.X, size.Y),
		SDK::FVector2D(check_size.X, check_size.Y),
		color::get()->check_col,
		color::get()->normal_col,
		color::get()->hover_col,
		color::get()->press_col,
		ptr,
		tip
	);
}

void menu_lib::drag_bar(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FVector2D& bar_size, float* ptr, const float& min, const float& max, const std::wstring& tip, bool is_attach)
{
	gui::drag_bar(
		name,
		is_attach ? attach(pos.X, pos.Y) : pos,
		SDK::FVector2D(size.X, size.Y),
		SDK::FVector2D(bar_size.X, bar_size.Y),
		color::get()->bar_col,
		color::get()->normal_col,
		color::get()->hover_col,
		color::get()->press_col,
		ptr,
		min,
		max,
		tip
	);
}

void menu_lib::text(const SDK::FVector2D& pos, const SDK::FString& str, const bool& mid_x, const bool& mid_y, bool is_attach)
{
	render::draw_text(
		gvalue::engine->TinyFont,
		str,
		is_attach ? attach(pos.X, pos.Y) : pos,
		SDK::FVector2D(1.0f, 1.0f),
		color::get()->text_col,
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
		SDK::FVector2D(0.0f, 0.0f),
		mid_x,
		mid_y,
		false,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
	);
}

void menu_lib::pice(const SDK::FVector2D& pos, const SDK::FVector2D& size, const bool& small)
{
	render::fill_box(
		attach(pos.X, pos.Y),
		size,
		small ? color::get()->small_pice_col : color::get()->pice_col
	);
}


