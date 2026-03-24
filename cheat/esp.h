#pragma once
#include <vector>
#include <unordered_map>
#include <string>

namespace SDK
{
	struct FVector2D;
	struct FLinearColor;

	class AActor;
	class USceneComponent;
	class USkeletalMeshComponent;
	class FName;
}

namespace UC
{
	class FString;
}

struct s_esp_character;
struct s_esp_item;
struct actor_info;

class esp
{
public:
	static esp* get();
	void init();
	void main();
private:

	//void handle(SDK::AActor* actor);
	void handle(const actor_info& info);

	bool get_box(SDK::USceneComponent* comp, SDK::FVector2D& min, SDK::FVector2D& max);
	bool get_origin(SDK::AActor* actor, SDK::FVector2D& pos);

	void draw_box(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color);
	void draw_line(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color);
	void draw_skeleton(SDK::USkeletalMeshComponent* comp, const SDK::FLinearColor& color);

	void draw_character(SDK::AActor* actor,SDK::FLinearColor color,const std::wstring& name,const s_esp_character& esp);
	void draw_item(SDK::AActor* actor,const SDK::FLinearColor& color,const std::wstring& name,const std::wstring& status,const s_esp_item& esp);

	void handle_team(SDK::AActor* actor);
	void handle_civilian(SDK::AActor* actor);
	void handle_suspect(SDK::AActor* actor);

	void handle_trap(SDK::AActor* actor);
	void handle_report(SDK::AActor* actor);
	void handle_bomb(SDK::AActor* actor);
	void handle_weapon(SDK::AActor* actor);
	void handle_other(SDK::AActor* actor);

	void handle_test(SDK::AActor* actor);
};

