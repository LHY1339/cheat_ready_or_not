#pragma once

namespace SDK
{
	struct FVector;
	struct FVector2D;
	struct FRotator;
}

class aim
{
public:
	static aim* get();

	void init();
	void main();

private:
	aim() = default;

	bool get_target(SDK::FVector& pos_3d, SDK::FVector2D& pos_2d);
	bool is_visible(const SDK::FVector& pos_3d);
	void mem_change();
};

