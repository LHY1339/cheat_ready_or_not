#pragma once
#include "sdk/SDK/Engine_classes.hpp"

namespace menu_lib
{
	void set_font(const int& size);
	SDK::FVector2D attach(const float& ref_x, const float& ref_y);
	bool button_color(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const std::wstring& tip = L"",  bool is_attach = true);
	bool button_text(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FString& str, const std::wstring& tip = L"", bool is_attach = true);
	bool button_color_text(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FString& str, const std::wstring& tip = L"", bool is_attach = true);
	void check_box(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FVector2D& check_size, bool* ptr, const std::wstring& tip = L"", bool is_attach = true);
	void drag_bar(const std::string& name, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FVector2D& bar_size, float* ptr, const float& min, const float& max, const std::wstring& tip = L"", bool is_attach = true);
	void text(const SDK::FVector2D& pos, const SDK::FString& str, const bool& mid_x = false, const bool& mid_y = false, bool is_attach = true);
	void pice(const SDK::FVector2D& pos, const SDK::FVector2D& size, const bool& small = false);

	inline SDK::FVector2D pos = SDK::FVector2D(100.0f, 100.0f);
	inline SDK::FVector2D size = SDK::FVector2D(600.0f, 400.0f);
	inline SDK::FVector2D drag_pos = SDK::FVector2D(0.0f, 0.0f);
	inline bool is_drag = false;
};