#include "menu.h"

#include "render.h"
#include "_sdk.h"
#include "gvalue.h"
#include "gui.h"
#include "color.h"
#include "gconst.h"

#include <format>

enum class e_page
{
	visual,
	aim,
	player,
	level,
	misc,
};

class param
{
public:
	inline static SDK::FVector2D pos = SDK::FVector2D(100.0f, 100.0f);
	inline static SDK::FVector2D size = SDK::FVector2D(600.0f, 400.0f);
	inline static SDK::FVector2D drag_pos = SDK::FVector2D(0.0f, 0.0f);
	inline static bool is_drag = false;

	inline static e_page page = e_page::visual;

	inline static SDK::UTexture2D* visual_texture = nullptr;
};

class function
{
public:
	static void set_font(const int& size)
	{
		gvalue::engine->TinyFont->LegacyFontSize = size;
	}

	static SDK::FVector2D attach(const float& ref_x, const float& ref_y)
	{
		return SDK::FVector2D(param::pos.X + ref_x, param::pos.Y + ref_y);
	}

	static bool button_color(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, bool is_attach = true)
	{
		return gui::button_color(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(size.X, size.Y),
			color::get()->normal_col,
			color::get()->hover_col,
			color::get()->press_col
		);
	}

	static bool button_text(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FString& str, bool is_attach = true)
	{
		return gui::button_text(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(size.X, size.Y),
			str,
			gvalue::engine->TinyFont,
			color::get()->text_col,
			color::get()->text_col,
			color::get()->text_col
		);
	}

	static bool button_color_text(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FString& str, bool is_attach = true)
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
			color::get()->press_col
		);
	}

	static void check_box(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FVector2D& check_size, bool* ptr, bool is_attach = true)
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
			ptr
		);
	}

	static void drag_bar(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FVector2D& bar_size, float* ptr, bool is_attach = true)
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
			ptr
		);
	}

	static void text(const SDK::FVector2D& pos, const SDK::FString& str, const bool& mid_x = false, const bool& mid_y = false, bool is_attach = true)
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

	static void pice(const SDK::FVector2D& pos, const SDK::FVector2D& size)
	{
		render::fill_box(
			attach(pos.X, pos.Y),
			size,
			color::get()->pice_col
		);
	}
};

menu* menu::get()
{
	static menu inst;
	return &inst;
}

void menu::init()
{

}

void menu::main()
{
	if (gvalue::menu_open)
	{
		pre_base();
		base();
		cursor();
	}
	lable();
}

void menu::lable()
{
	if (!gvalue::draw_lable)
	{
		return;
	}
	const std::wstring lab_text =
		std::wstring(L"按下 Ins（Insert）显示/隐藏菜单\n");

	render::draw_text(
		gvalue::engine->TinyFont,
		lab_text.c_str(),
		SDK::FVector2D(10.0f, 10.0f),
		SDK::FVector2D(1.0f, 1.0f),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
		SDK::FVector2D(0.0f, 0.0f),
		false,
		false,
		true,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

void menu::pre_base()
{
	gui::drag(
		param::pos,
		param::size,
		param::drag_pos,
		param::is_drag
	);
}

void menu::base()
{
	render::fill_box(
		SDK::FVector2D(param::pos.X - 2, param::pos.Y - 2),
		SDK::FVector2D(param::size.X + 4, param::size.Y + 4),
		color::get()->outline_col
	);
	render::fill_box(
		SDK::FVector2D(param::pos.X, param::pos.Y),
		SDK::FVector2D(param::size.X, param::size.Y),
		color::get()->back_col
	);

	{
		render::fill_box(
			SDK::FVector2D(param::pos.X - 2, param::pos.Y + param::size.Y + 8),
			SDK::FVector2D(param::size.X + 4, 39),
			SDK::FLinearColor(0.3f, 0.1f, 0.0f, 1.0f)
		);
		render::fill_box(
			SDK::FVector2D(param::pos.X, param::pos.Y + param::size.Y + 10),
			SDK::FVector2D(param::size.X, 35),
			SDK::FLinearColor(0.1f, 0.06f, 0.0f, 1.0f)
		);
		function::text(SDK::FVector2D(10, param::size.Y + 18), L"本修改器永久免费开源，问题反馈QQ群：1071845133");
	}

	if (gvalue::max_version != gconst::version && gvalue::max_version != "")
	{
		render::fill_box(
			SDK::FVector2D(param::pos.X - 2, param::pos.Y + param::size.Y + 58),
			SDK::FVector2D(170 + 4, 39),
			SDK::FLinearColor(0.3f, 0.1f, 0.0f, 1.0f)
		);
		render::fill_box(
			SDK::FVector2D(param::pos.X, param::pos.Y + param::size.Y + 60),
			SDK::FVector2D(170, 35),
			SDK::FLinearColor(0.1f, 0.06f, 0.0f, 1.0f)
		);
		function::text(SDK::FVector2D(10, param::size.Y + 68), L"有新版本可以下载！");
	}

	function::set_font(12);
	param::size = SDK::FVector2D(600, 400);

	left();

#define SWITCH(_name_) \
case e_page::_name_: \
	_name_(); \
	break; 

	switch (param::page)
	{
		SWITCH(visual);
		SWITCH(aim);
		SWITCH(player);
		SWITCH(level);
		SWITCH(misc);
	}

#undef SWITCH
}

void menu::cursor()
{
	render::fill_box(
		SDK::FVector2D(gvalue::mouse.x - 3, gvalue::mouse.y - 3),
		SDK::FVector2D(6, 6),
		color::get()->normal_col
	);
	render::fill_box(
		SDK::FVector2D(gvalue::mouse.x - 2, gvalue::mouse.y - 2),
		SDK::FVector2D(4, 4),
		color::get()->text_col
	);
}

void menu::left()
{
	function::pice(SDK::FVector2D(0, 0), SDK::FVector2D(100, param::size.Y));

	static float current_y = 10.0f;
	const float target_y = 10 + (int)param::page * 40;
	current_y = current_y + (target_y - current_y) * 15 * gvalue::delta_time;
	render::fill_box(function::attach(10, current_y), SDK::FVector2D(80, 30), color::get()->normal_col);

#define RON_BUTTON(_page_,_name_,_ypos_) \
if (function::button_text(#_page_, SDK::FVector2D(10, _ypos_), SDK::FVector2D(80, 30), L#_name_)) \
{ \
	param::page = e_page::_page_; \
}
	RON_BUTTON(visual, 视觉, 10);
	RON_BUTTON(aim, 瞄准, 50);
	RON_BUTTON(player, 玩家, 90);
	RON_BUTTON(level, 关卡, 130);
	RON_BUTTON(misc, 杂项, 170);

#undef RON_BUTTON

	if (function::button_text("", SDK::FVector2D(10, param::size.Y - 40), SDK::FVector2D(80, 30), L"LHY1339 "))
	{
		system("start https://afdian.com/a/lhy1339");
	}
}

void menu::visual()
{
	param::size.X = 900;

	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(540, 170));
	function::pice(SDK::FVector2D(110, 190), SDK::FVector2D(540, param::size.Y - 200));
	function::pice(SDK::FVector2D(660, 10), SDK::FVector2D(230, 130));
	function::pice(SDK::FVector2D(660, 150), SDK::FVector2D(230, param::size.Y - 160));

#define RON_TEXT(_x_,_text_) \
function::text(SDK::FVector2D(_x_ - 5, 20), L#_text_);

	RON_TEXT(180, 启用);
	RON_TEXT(240, 方框);
	RON_TEXT(300, 骨骼);
	RON_TEXT(360, 名称);
	RON_TEXT(420, 状态);
	RON_TEXT(480, 距离);
	RON_TEXT(540, 连线);
	RON_TEXT(600, 范围);

#undef RON_TEXT

#define RON_TEXT(_y_,_text_) \
function::text(SDK::FVector2D(120, _y_), L#_text_);

	RON_TEXT(60, 玩家);
	RON_TEXT(100, 嫌疑人);
	RON_TEXT(140, 平民);

#undef RON_TEXT;

#define RON_CHECK(_y_,_type_) \
	function::check_box(" ", SDK::FVector2D(180, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.enable); \
	function::check_box(" ", SDK::FVector2D(240, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.box); \
	function::check_box(" ", SDK::FVector2D(300, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.skeleton); \
	function::check_box(" ", SDK::FVector2D(360, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.name); \
	function::check_box(" ", SDK::FVector2D(420, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.status); \
	function::check_box(" ", SDK::FVector2D(480, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.distance); \
	function::check_box(" ", SDK::FVector2D(540, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.line); \
	function::check_box(" ", SDK::FVector2D(600, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.extent);

	RON_CHECK(60, player);
	RON_CHECK(100, suspect);
	RON_CHECK(140, civilian);

#undef RON_CHECK

#define RON_TEXT(_x_,_text_) \
function::text(SDK::FVector2D(_x_ - 5, 200), L#_text_);

	RON_TEXT(180, 启用);
	RON_TEXT(240, 圆圈);
	RON_TEXT(300, 名称);
	RON_TEXT(360, 状态);
	RON_TEXT(420, 距离);
	RON_TEXT(480, 连线);
	RON_TEXT(540, 范围);

#undef RON_TEXT

#define RON_TEXT(_y_,_text_) \
function::text(SDK::FVector2D(120, _y_), L#_text_);

	RON_TEXT(240, 陷阱);
	RON_TEXT(280, 报告);
	RON_TEXT(320, 炸弹);
	RON_TEXT(360, 武器);

#undef RON_TEXT;

#define RON_CHECK(_y_,_type_) \
	function::check_box(" ", SDK::FVector2D(180, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.enable); \
	function::check_box(" ", SDK::FVector2D(240, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.circle); \
	function::check_box(" ", SDK::FVector2D(300, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.name); \
	function::check_box(" ", SDK::FVector2D(360, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.status); \
	function::check_box(" ", SDK::FVector2D(420, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.distance); \
	function::check_box(" ", SDK::FVector2D(480, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.line); \
	function::check_box(" ", SDK::FVector2D(540, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.extent);

	RON_CHECK(240, trap);
	RON_CHECK(280, report);
	RON_CHECK(320, bomb);
	RON_CHECK(360, weapon);

#undef RON_CHECK

	{
		const int value = 1000 * gvalue::esp_distance;
		const std::wstring wstr = L"透视距离：" + std::to_wstring(value) + L"米";
		function::text(SDK::FVector2D(670, 20), wstr.c_str());
		function::drag_bar("visual_distance", SDK::FVector2D(670, 50), SDK::FVector2D(210, 20), SDK::FVector2D(10, 20), &gvalue::esp_distance);
	}

	{
		const int value = 180 * gvalue::fov;
		const std::wstring wstr = L"视野：" + std::to_wstring(value);
		function::text(SDK::FVector2D(670, 80), wstr.c_str());
		function::drag_bar("fov", SDK::FVector2D(670, 110), SDK::FVector2D(210, 20), SDK::FVector2D(10, 20), &gvalue::fov);
	}

	{
		function::check_box(" ", SDK::FVector2D(670, 160), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::third_person);
		function::text(SDK::FVector2D(700, 160), L"第三人称");

		function::text(SDK::FVector2D(670, 190), L"X");
		function::drag_bar("cam_x", SDK::FVector2D(690, 190), SDK::FVector2D(80, 20), SDK::FVector2D(10, 20), &gvalue::cam_x);

		function::text(SDK::FVector2D(780, 190), L"Y");
		function::drag_bar("cam_y", SDK::FVector2D(800, 190), SDK::FVector2D(80, 20), SDK::FVector2D(10, 20), &gvalue::cam_y);

		const int cam_distance = 1000 * gvalue::cam_distance;
		const std::wstring wstr = L"距离：" + std::to_wstring(cam_distance);
		function::text(SDK::FVector2D(670, 220), wstr.c_str());
		function::drag_bar("cam_distance", SDK::FVector2D(770, 220), SDK::FVector2D(110, 20), SDK::FVector2D(10, 20), &gvalue::cam_distance);

		if (function::button_color_text(" ", SDK::FVector2D(670, 250), SDK::FVector2D(210, 30), L"重置第三人称视角"))
		{
			gvalue::cam_x = 0.5;
			gvalue::cam_y = 0.5;
			gvalue::cam_distance = 0.2;
		}
	}
}

void menu::aim()
{
	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(300, param::size.Y - 20));
	function::pice(SDK::FVector2D(420, 10), SDK::FVector2D(170, 100));
	function::pice(SDK::FVector2D(420, 120), SDK::FVector2D(170, param::size.Y - 130));

#define RON_CHECK(_y_,_param_,_text_) \
function::check_box(" ", SDK::FVector2D(120, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::_param_); \
function::text(SDK::FVector2D(150, _y_), L#_text_);

	RON_CHECK(20, aim_enable, 启用自瞄);
	RON_CHECK(50, aim_visibility, 仅自瞄可视);
	RON_CHECK(80, aim_down, 自瞄倒地);
	RON_CHECK(110, aim_use_smooth, 使用平滑自瞄);

	{
		const std::wstring wstr = L"平滑：" + std::format(L"{:.2f}", (gvalue::aim_smooth * 20));
		function::text(SDK::FVector2D(120, 140), wstr.c_str());
		function::drag_bar("aim_smooth", SDK::FVector2D(230, 140), SDK::FVector2D(170, 20), SDK::FVector2D(10, 20), &gvalue::aim_smooth);
	}

	RON_CHECK(170, aim_use_radius, 使用自瞄范围);

	{
		const std::wstring wstr = L"范围：" + std::format(L"{:.2f}", (gvalue::aim_radius * 1000));
		function::text(SDK::FVector2D(120, 200), wstr.c_str());
		function::drag_bar("aim_radius", SDK::FVector2D(240, 200), SDK::FVector2D(160, 20), SDK::FVector2D(10, 20), &gvalue::aim_radius);
	}

	RON_CHECK(230, aim_draw, 绘制自瞄提示);

	RON_CHECK(260, aim_no_recoil, 无后座);
	RON_CHECK(290, aim_inf_bullet, 无限子弹);
	RON_CHECK(320, aim_fire_rate, 射速);

#undef RON_CHECK

#define RON_CHECK(_y_,_param_,_text_) \
function::check_box(" ", SDK::FVector2D(430, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::_param_); \
function::text(SDK::FVector2D(460, _y_), L#_text_);

	RON_CHECK(20, aim_player, 自瞄玩家);
	RON_CHECK(50, aim_civilian, 自瞄平民);
	RON_CHECK(80, aim_suspect, 自瞄嫌疑人);

#undef RON_CHECK
}

void menu::player()
{
}

void menu::level()
{
}

void menu::misc()
{
}
