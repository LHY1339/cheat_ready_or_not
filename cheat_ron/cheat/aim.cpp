#include "aim.h"

#include "gvalue.h"
#include "_sdk.h"
#include "render.h"
#include "kismet.h"

SDK::FVector2D center;

aim* aim::get()
{
	static aim inst;
	return &inst;
}

void aim::init()
{

}

void aim::main()
{
	if (!gvalue::world || !gvalue::controller || !gvalue::controller->PlayerCameraManager)
	{
		return;
	}

	static HWND hwnd = kismet::get_window();
	RECT rect;
	GetClientRect(hwnd, &rect);
	center = SDK::FVector2D((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

	mem_change();

	if (gvalue::aim_draw && gvalue::aim_use_radius)
	{
		render::draw_circle(
			center,
			gvalue::aim_radius * 1000,
			1,
			20,
			SDK::FLinearColor(1, 1, 1, 1)
		);
	}

	if (kismet::is_key_down(VK_RBUTTON))
	{
		if (!gvalue::aim_enable)
		{
			return;
		}

		SDK::FVector pos_3d;
		SDK::FVector2D pos_2d;
		const bool in_view = get_target(pos_3d, pos_2d);
		if (in_view)
		{
			if (gvalue::aim_draw)
			{
				render::draw_line(
					center,
					pos_2d,
					1,
					SDK::FLinearColor(1, 1, 1, 1)
				);
			}


			SDK::FRotator rot = SDK::UKismetMathLibrary::FindLookAtRotation(
				gvalue::controller->PlayerCameraManager->GetCameraLocation(),
				pos_3d
			);
			gvalue::controller->ControlRotation = SDK::UKismetMathLibrary::RLerp(
				gvalue::controller->ControlRotation,
				rot,
				gvalue::aim_use_smooth ? (gvalue::aim_smooth * 20) * gvalue::delta_time : 1, 
				true
			);
		}
	}
}

bool aim::get_target(SDK::FVector& pos_3d, SDK::FVector2D& pos_2d)
{
	bool find_target = false;
	float min_dist = 0.0f;

	SDK::TArray<SDK::AActor*> actor_list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::AReadyOrNotCharacter::StaticClass(), &actor_list);
	for (SDK::AActor* this_actor : actor_list)
	{
		if (this_actor->IsA(SDK::ABasePlayer_C::StaticClass()) && !gvalue::aim_player)
		{
			continue;
		}

		if (this_actor->IsA(SDK::ACivilianCharacter::StaticClass()) && !gvalue::aim_civilian)
		{
			continue;
		}

		if (this_actor->IsA(SDK::ASuspectCharacter::StaticClass()) && !gvalue::aim_suspect)
		{
			continue;
		}

		SDK::AReadyOrNotCharacter* character = static_cast<SDK::AReadyOrNotCharacter*>(this_actor);
		if (!gvalue::aim_down)
		{
			switch (character->CharacterHealth->HealthStatus)
			{
			case SDK::EPlayerHealthStatus::HS_Downed:
			case SDK::EPlayerHealthStatus::HS_Dead:
			case SDK::EPlayerHealthStatus::HS_Incapacitated:
			case SDK::EPlayerHealthStatus::HS_Arrested:
				continue;
				break;
			}
		}

		SDK::FVector head_pos = character->Mesh->GetSocketLocation(
			SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString::FString(L"head"))
		);

		if (!is_visible(head_pos))
		{
			continue;
		}

		SDK::FVector2D head_pos_2d;
		const bool in_view = gvalue::controller->ProjectWorldLocationToScreen(
			head_pos,
			&head_pos_2d,
			true
		);

		if (!in_view)
		{
			continue;
		}

		float dist = SDK::UKismetMathLibrary::Distance2D(center, head_pos_2d);
		if (!find_target || dist < min_dist)
		{
			find_target = true;
			pos_2d = head_pos_2d;
			pos_3d = head_pos;
			min_dist = dist;
		}
	}

	if (gvalue::aim_use_radius && min_dist > gvalue::aim_radius * 1000)
	{
		return false;
	}

	return find_target;
}

bool aim::is_visible(const SDK::FVector& pos_3d)
{
	if (!gvalue::aim_visibility)
	{
		return true;
	}

	SDK::FVector start_pos = gvalue::controller->PlayerCameraManager->GetCameraLocation();

	SDK::TArray<SDK::AActor*> ignore_actor;
	SDK::FHitResult result;

	SDK::UKismetSystemLibrary::LineTraceSingle(
		gvalue::world,
		start_pos,
		pos_3d,
		SDK::ETraceTypeQuery::TraceTypeQuery4,
		false,
		ignore_actor,
		SDK::EDrawDebugTrace::None,
		&result,
		true,
		SDK::FLinearColor(),
		SDK::FLinearColor(),
		0.0f
	);
	return !result.bBlockingHit;
}

void aim::mem_change()
{
	if (!gvalue::controller->Pawn ||
		!gvalue::controller->Pawn->IsA(SDK::ABasePlayer_C::StaticClass()))
	{
		return;
	}

	SDK::ABasePlayer_C* player = static_cast<SDK::ABasePlayer_C*>(gvalue::controller->Pawn);
	if (!player->GetEquippedWeapon())
	{
		return;
	}

	SDK::ABaseMagazineWeapon* cur_weapon = player->GetEquippedWeapon();
	SDK::ABaseMagazineWeapon* def_weapon = static_cast<SDK::ABaseMagazineWeapon*>(cur_weapon->Class->DefaultObject);

	if (gvalue::aim_fire_rate)
	{
		cur_weapon->FireRate = 0.01f;
	}
	else
	{
		cur_weapon->FireRate = def_weapon->FireRate;
	}

	if (gvalue::aim_inf_bullet)
	{
		cur_weapon->Magazines[cur_weapon->MagIndex].Ammo = (int)cur_weapon->AmmoMax;
	}

}
