#include "menu_mod.h"
#include "menu_lib.h"
#include "gvalue.h"

namespace esp
{
	void draw_pice()
	{
		menu_lib::pice(SDK::FVector2D(110, 10), SDK::FVector2D(480, 170), true);
		menu_lib::pice(SDK::FVector2D(110, 190), SDK::FVector2D(480, 250), true);
		menu_lib::pice(SDK::FVector2D(110, 450), SDK::FVector2D(480, 40), true);
	}

	void draw_character()
	{

#define RON_TEXT(_x_,_text_) \
menu_lib::text(SDK::FVector2D(_x_ - 5, 20), L#_text_);

		RON_TEXT(180, 启用);
		RON_TEXT(240, 方框);
		RON_TEXT(300, 骨骼);
		RON_TEXT(360, 名称);
		RON_TEXT(420, 状态);
		RON_TEXT(480, 距离);
		RON_TEXT(540, 连线);

#undef RON_TEXT

#define RON_TEXT(_y_,_text_) \
menu_lib::text(SDK::FVector2D(120, _y_), L#_text_);

		RON_TEXT(60, 队友);
		RON_TEXT(100, 嫌疑人);
		RON_TEXT(140, 平民);

#undef RON_TEXT;

#define RON_CHECK(_y_,_type_) \
	menu_lib::check_box(" ", SDK::FVector2D(180, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.enable); \
	menu_lib::check_box(" ", SDK::FVector2D(240, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.box); \
	menu_lib::check_box(" ", SDK::FVector2D(300, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.skeleton); \
	menu_lib::check_box(" ", SDK::FVector2D(360, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.name); \
	menu_lib::check_box(" ", SDK::FVector2D(420, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.status); \
	menu_lib::check_box(" ", SDK::FVector2D(480, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.distance); \
	menu_lib::check_box(" ", SDK::FVector2D(540, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.line); 

		RON_CHECK(60, team);
		RON_CHECK(100, suspect);
		RON_CHECK(140, civilian);

#undef RON_CHECK
	}

	void draw_item()
	{
#define RON_TEXT(_x_,_text_) \
menu_lib::text(SDK::FVector2D(_x_ - 5, 200), L#_text_);

		RON_TEXT(180, 启用);
		RON_TEXT(240, 圆圈);
		RON_TEXT(300, 名称);
		RON_TEXT(360, 状态);
		RON_TEXT(420, 距离);
		RON_TEXT(480, 连线);

#undef RON_TEXT

#define RON_TEXT(_y_,_text_) \
menu_lib::text(SDK::FVector2D(120, _y_), L#_text_);

		RON_TEXT(240, 陷阱);
		RON_TEXT(280, 报告);
		RON_TEXT(320, 炸弹);
		RON_TEXT(360, 武器);
		RON_TEXT(400, 其他);

#undef RON_TEXT;

#define RON_CHECK(_y_,_type_) \
	menu_lib::check_box(" ", SDK::FVector2D(180, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.enable); \
	menu_lib::check_box(" ", SDK::FVector2D(240, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.circle); \
	menu_lib::check_box(" ", SDK::FVector2D(300, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.name); \
	menu_lib::check_box(" ", SDK::FVector2D(360, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.status); \
	menu_lib::check_box(" ", SDK::FVector2D(420, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.distance); \
	menu_lib::check_box(" ", SDK::FVector2D(480, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.line); 

		RON_CHECK(240, trap);
		RON_CHECK(280, report);
		RON_CHECK(320, bomb);
		RON_CHECK(360, weapon);
		RON_CHECK(400, other);

#undef RON_CHECK
	}

	void draw_distance()
	{
		const std::wstring wstr = L"透视距离：" + std::to_wstring((int)gvalue::esp_distance);
		menu_lib::text(SDK::FVector2D(120, 460), wstr.c_str());
		menu_lib::drag_bar("esp_distance", SDK::FVector2D(250, 460), SDK::FVector2D(330, 20), SDK::FVector2D(10, 20), &gvalue::esp_distance, 0, 999, L"修改透视距离，不建议在较大的地图拉高，否则可能会卡顿");
	}
}

void menu_mod::esp()
{
	esp::draw_pice();
	esp::draw_character();
	esp::draw_item();
	esp::draw_distance();
}