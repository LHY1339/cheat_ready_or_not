#include "render.h"

#include "gvalue.h"
#include "sdk/SDK/Engine_classes.hpp"

void render::draw_box(const SDK::FVector2D& pos, const SDK::FVector2D& size, float thick, const SDK::FLinearColor& color)
{
	gvalue::canvas->K2_DrawBox(
		pos,
		size,
		thick,
		color
	);
}

void render::fill_box(const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FLinearColor& color)
{
	const double min = size.X < size.Y ? size.X : size.Y;
	gvalue::canvas->K2_DrawBox(
		SDK::FVector2D(pos.X + min / 2, pos.Y + min / 2),
		SDK::FVector2D(size.X - min + 1, size.Y - min + 1),
		static_cast<float>(min),
		color
	);
}

void render::draw_text(SDK::UFont* font, const UC::FString& text, const SDK::FVector2D& pos, const SDK::FVector2D& size, const SDK::FLinearColor& color, float kerning, const SDK::FLinearColor& shadow_color, const SDK::FVector2D& shadow_offset, bool center_x, bool center_y, bool outline, const SDK::FLinearColor& outline_color)
{
	gvalue::canvas->K2_DrawText(
		font,
		text,
		pos,
		size,
		color,
		kerning,
		shadow_color,
		shadow_offset,
		center_x,
		center_y,
		outline,
		outline_color
	);
}

void render::draw_line(const SDK::FVector2D& pos1, const SDK::FVector2D& pos2, float thick, const SDK::FLinearColor& color)
{
	gvalue::canvas->K2_DrawLine(
		pos1,
		pos2,
		thick,
		color
	);
}

void render::draw_circle(const SDK::FVector2D& center, const float& radius, const float& thick, const int& line, const SDK::FLinearColor& color)
{
	if (line <= 0)
	{
		return;
	}

#define PI 3.14159265358979
	const float step = 2.0f * static_cast<float>(PI) / line;
#undef PI

	for (int i = 0; i < line; ++i)
	{
		float angle1 = step * i;
		float angle2 = step * (i + 1);
		const SDK::FVector2D p1(center.X + radius * cosf(angle1), center.Y + radius * sinf(angle1));
		const SDK::FVector2D p2(center.X + radius * cosf(angle2), center.Y + radius * sinf(angle2));
		draw_line(p1, p2, thick, color);
	}
}