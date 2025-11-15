#pragma once
#include <string>

namespace SDK
{
	struct FVector2D;
	struct FLinearColor;
	class USceneComponent;
	class AActor;
	class USkeletalMeshComponent;
	class FName;
}

namespace UC
{
	class FString;
}

struct s_character_visual;

class visual
{
public:
	static void init();
	static void main();

private:
	static void get_all();
	static void camera();

	static bool get_box(SDK::USceneComponent* comp, SDK::FVector2D& min, SDK::FVector2D& max);
	static void draw_extent(SDK::USceneComponent* comp, const SDK::FLinearColor& color);
	static void draw_skeleton(SDK::USkeletalMeshComponent* mesh, const SDK::FLinearColor& color);
	static void draw_character(SDK::USceneComponent* comp, const SDK::FLinearColor& color, const UC::FString& name, const s_character_visual& visual);
	static std::wstring find_name(const std::wstring& in_name);
	static SDK::FName to_name(std::wstring str);
};

