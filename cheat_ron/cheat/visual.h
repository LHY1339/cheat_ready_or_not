#pragma once
#include <string>

namespace SDK
{
	struct FVector2D;
	struct FLinearColor;
	class AActor;
	class USceneComponent;
	class USkeletalMeshComponent;
}

namespace UC
{
	class FString;
}

struct s_esp_character;
struct s_esp_item;

class visual
{
public:
	static visual* get();

	void init();
	void main();
	std::wstring find_name(const std::wstring& in_name);
private:
	visual() = default;

	void get_all();
	void camera();

	bool get_box(SDK::USceneComponent* comp, SDK::FVector2D& min, SDK::FVector2D& max);
	bool get_origin(SDK::AActor* actor, SDK::FVector2D& pos);

	void draw_box(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color);
	void draw_line(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color);
	void draw_extent(SDK::USceneComponent* comp, const SDK::FLinearColor& color);
	void draw_skeleton(SDK::USkeletalMeshComponent* comp, const SDK::FLinearColor& color);

	void draw_character(
		SDK::AActor* actor, 
		SDK::FLinearColor color, 
		const std::wstring& name,
		const s_esp_character& esp
	);

	void draw_item(
		SDK::AActor* actor,
		const SDK::FLinearColor& color,
		const std::wstring& name,
		const std::wstring& status,
		const s_esp_item& esp
	);
};

