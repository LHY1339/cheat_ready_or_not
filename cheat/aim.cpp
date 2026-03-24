#include "aim.h"
#include "sdk/SDK/Engine_classes.hpp"
#include "sdk/SDK/ReadyOrNot_classes.hpp"
#include "sdk/SDK/BasePlayer_classes.hpp"
#include "gvalue.h"
#include "kismet.h"
#include "render.h"
#include "manager.h"

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

	calculate();
	legal_aim();
	mem_change();
}

void aim::calculate()
{
	RECT rect;
	GetClientRect(kismet::get_window(), &rect);
	center = SDK::FVector2D((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);
}

void aim::legal_aim()
{
	if (gvalue::aim_radius_draw && gvalue::aim_use_radius)
	{
		render::draw_circle(
			center,
			gvalue::aim_radius,
			1,
			20,
			SDK::FLinearColor(1, 1, 1, 1)
		);
	}

	if (!gvalue::aim_enable)
	{
		return;
	}

	if ((kismet::is_key_down(VK_LBUTTON) && gvalue::aim_left_button) ||
		(kismet::is_key_down(VK_RBUTTON) && gvalue::aim_right_button) ||
		(kismet::is_key_down(VK_MENU) && gvalue::aim_alt) ||
		(kismet::is_key_down(VK_RMENU) && gvalue::aim_side_button) ||
		(kismet::is_key_down(VK_LMENU) && gvalue::aim_side_button))
	{
		SDK::FVector pos_3d;
		SDK::FVector2D pos_2d;
		const bool in_view = get_target(pos_3d, pos_2d);
		if (in_view)
		{
			if (gvalue::aim_radius_draw)
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
				gvalue::aim_use_smooth ? gvalue::aim_smooth * gvalue::delta_time : 1,
				true
			);
		}
	}
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
	SDK::ABaseMagazineWeapon* def_weapon = static_cast<SDK::ABaseMagazineWeapon*>(cur_weapon->Class->ClassDefaultObject);

	if (gvalue::aim_fire_rate)
	{
		cur_weapon->FireRate = 0.01f;
	}
	else
	{
		cur_weapon->FireRate = def_weapon->FireRate;
	}

	if (gvalue::aim_no_delta)
	{
		cur_weapon->InertiaDragAimRotation = 0;
		cur_weapon->InertiaDragStrafeRotation = 0;
		cur_weapon->FreeAimInterpSpeed = 9999;
	}
	else
	{
		cur_weapon->InertiaDragAimRotation = def_weapon->InertiaDragAimRotation;
		cur_weapon->InertiaDragStrafeRotation = def_weapon->InertiaDragStrafeRotation;
		cur_weapon->FreeAimInterpSpeed = def_weapon->FreeAimInterpSpeed;;
	}

	if (gvalue::aim_inf_bullet)
	{
		cur_weapon->Magazines[cur_weapon->MagIndex].Ammo = (int)cur_weapon->AmmoMax;
	}

	if (gvalue::aim_no_recoil)
	{
		cur_weapon->RecoilInterpSpeed = 0;
		cur_weapon->FirstShotRecoil = 0;
		cur_weapon->RecoilFireTime = 0;
	}
	else
	{
		cur_weapon->RecoilInterpSpeed = def_weapon->RecoilInterpSpeed;
		cur_weapon->FirstShotRecoil = def_weapon->FirstShotRecoil;
	}
}

bool aim::get_target(SDK::FVector& pos_3d, SDK::FVector2D& pos_2d)
{
	bool find_target = false;
	float min_dist = 0.0f;

	auto trace_socket = [&](SDK::AReadyOrNotCharacter* character, const std::string& str)
		{
			SDK::FVector head_pos = character->Mesh->GetSocketLocation(manager::get()->bone_list[str]);

			if (!is_visible(head_pos))
			{
				return;
			}

			SDK::FVector2D head_pos_2d;
			const bool in_view = gvalue::controller->ProjectWorldLocationToScreen(
				head_pos,
				&head_pos_2d,
				true
			);

			if (!in_view)
			{
				return;
			}

			float dist = SDK::UKismetMathLibrary::Distance2D(center, head_pos_2d);
			if (!find_target || dist < min_dist)
			{
				find_target = true;
				pos_2d = head_pos_2d;
				pos_3d = head_pos;
				min_dist = dist;
			}
		};

	//SDK::TArray<SDK::AActor*> actor_list;
	//SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::AReadyOrNotCharacter::StaticClass(), &actor_list);
	//for (SDK::AActor* this_actor : actor_list)
	//{
	//	if ((this_actor->IsA(SDK::ABasePlayer_C::StaticClass()) || this_actor->IsA(SDK::ASWATCharacter::StaticClass())) && !gvalue::aim_player)
	//	{
	//		continue;
	//	}

	//	if (this_actor->IsA(SDK::ACivilianCharacter::StaticClass()) && !gvalue::aim_civilian)
	//	{
	//		continue;
	//	}

	//	if (this_actor->IsA(SDK::ASuspectCharacter::StaticClass()) && !gvalue::aim_suspect)
	//	{
	//		continue;
	//	}

	//	SDK::AReadyOrNotCharacter* character = static_cast<SDK::AReadyOrNotCharacter*>(this_actor);
	//	if (!gvalue::aim_down)
	//	{
	//		switch (character->CharacterHealth->HealthStatus)
	//		{
	//		case SDK::EPlayerHealthStatus::HS_Downed:
	//		case SDK::EPlayerHealthStatus::HS_Dead:
	//		case SDK::EPlayerHealthStatus::HS_Incapacitated:
	//		case SDK::EPlayerHealthStatus::HS_Arrested:
	//			continue;
	//			break;
	//		}
	//	}

	//	if (gvalue::aim_head)
	//	{
	//		trace_socket(character, L"head");
	//	}

	//	if (gvalue::aim_spine)
	//	{
	//		trace_socket(character, L"spine_1");
	//		trace_socket(character, L"spine_2");
	//		trace_socket(character, L"neck_1");
	//	}

	//	if (gvalue::aim_pelvis)
	//	{
	//		trace_socket(character, L"pelvis");
	//	}

	//	if (gvalue::aim_hand)
	//	{
	//		trace_socket(character, L"lowerarm_LE");
	//		trace_socket(character, L"hand_LE");

	//		trace_socket(character, L"lowerarm_RI");
	//		trace_socket(character, L"hand_RI");
	//	}

	//	if (gvalue::aim_foot)
	//	{
	//		trace_socket(character, L"calf_LE");
	//		trace_socket(character, L"foot_LE");

	//		trace_socket(character, L"calf_ri");
	//		trace_socket(character, L"foot_RI");
	//	}
	//}

	for (const actor_info& info : manager::get()->actor_list)
	{
		bool allow = false;
		switch (info.type)
		{
		case actor_type::team:
			allow = gvalue::aim_player;
			break;
		case actor_type::civilian:
			allow = gvalue::aim_civilian;
			break;
		case actor_type::suspect:
			allow = gvalue::aim_suspect;
			break;
		default:
			allow = false;
			break;
		}

		if (!allow)
		{
			continue;
		}

		SDK::AReadyOrNotCharacter* character = static_cast<SDK::AReadyOrNotCharacter*>(info.actor);
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

		if (gvalue::aim_head)
		{
			trace_socket(character, "head");
		}

		if (gvalue::aim_spine)
		{
			trace_socket(character, "spine_01");
			trace_socket(character, "spine_02");
			trace_socket(character, "neck_01");
		}

		if (gvalue::aim_pelvis)
		{
			trace_socket(character, "pelvis");
		}

		if (gvalue::aim_hand)
		{
			trace_socket(character, "lowerarm_l");
			trace_socket(character, "hand_l");

			trace_socket(character, "lowerarm_r");
			trace_socket(character, "hand_r");
		}

		if (gvalue::aim_foot)
		{
			trace_socket(character, "calf_l");
			trace_socket(character, "foot_l");

			trace_socket(character, "calf_r");
			trace_socket(character, "foot_r");
		}
	}

	if (gvalue::aim_use_radius && min_dist > gvalue::aim_radius)
	{
		return false;
	}

	return find_target;
}

bool aim::is_visible(const SDK::FVector& pos_3d)
{
	if (!gvalue::aim_wall_cond)
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
