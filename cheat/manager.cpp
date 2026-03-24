#include "manager.h"
#include "gvalue.h"

#include "sdk/SDK/Engine_classes.hpp"
#include "sdk/SDK/BasePlayer_classes.hpp"

manager* manager::get()
{
	static manager inst;
	return &inst;
}

void manager::init()
{
    bone_list["head"] = to_name(L"head");
    bone_list["neck_01"] = to_name(L"neck_1");
    bone_list["upperarm_l"] = to_name(L"upperarm_LE");
    bone_list["lowerarm_l"] = to_name(L"lowerarm_LE");
    bone_list["hand_l"] = to_name(L"hand_LE");
    bone_list["upperarm_r"] = to_name(L"upperarm_RI");
    bone_list["lowerarm_r"] = to_name(L"lowerarm_RI");
    bone_list["hand_r"] = to_name(L"hand_RI");
    bone_list["spine_01"] = to_name(L"spine_1");
    bone_list["spine_02"] = to_name(L"spine_2");
    bone_list["pelvis"] = to_name(L"pelvis");
    bone_list["thigh_l"] = to_name(L"thigh_LE");
    bone_list["calf_l"] = to_name(L"calf_LE");
    bone_list["foot_l"] = to_name(L"foot_LE");
    bone_list["thigh_r"] = to_name(L"thigh_RI");
    bone_list["calf_r"] = to_name(L"calf_RI");
    bone_list["foot_r"] = to_name(L"foot_RI");
}

void manager::main()
{
	timer();
    //print_dbg();
}

void manager::timer()
{
	static float timer_time = 0.0f;
	timer_time += gvalue::delta_time;
	if (timer_time < 1.0f)
	{
		return;
	}
	timer_time = 0.0f;
    actor_list.clear();
	SDK::TArray<SDK::AActor*> actor_list;
	SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::AActor::StaticClass(), &actor_list);
	for (SDK::AActor* actor : actor_list)
	{
		handle(actor);
	}
}

void manager::handle(SDK::AActor* actor)
{
    if (!actor || !actor->RootComponent)
    {
        return;
    }

    auto add_info = [&](const actor_type& type)
        {
            actor_info info;
            info.type = type;
            info.actor = actor;
            actor_list.push_back(info);
        };

    if (actor->IsA(SDK::ABasePlayer_C::StaticClass()) || actor->IsA(SDK::ASWATCharacter::StaticClass()))
    {
        add_info(actor_type::team);
        return;
    }
    if (actor->IsA(SDK::ASuspectCharacter::StaticClass()))
    {
        add_info(actor_type::suspect);
        return;
    }
    if (actor->IsA(SDK::ACivilianCharacter::StaticClass()))
    {
        add_info(actor_type::civilian);
        return;
    }

    if (actor->IsA(SDK::ATrapActor::StaticClass()))
    {
        add_info(actor_type::trap);
        return;
    }
    if (actor->IsA(SDK::AReportableActor::StaticClass()))
    {
        add_info(actor_type::report);
        return;
    }
    if (actor->IsA(SDK::ABombActor::StaticClass()))
    {
        add_info(actor_type::bomb);
        return;
    }
    if (actor->IsA(SDK::ABaseWeapon::StaticClass()))
    {
        add_info(actor_type::weapon);
        return;
    }
}

void manager::print_dbg()
{
    for (const actor_info& info : actor_list)
    {
        printf("info : %s\n", info.actor->GetName().c_str());
    }
}

SDK::FName manager::to_name(const std::wstring& str)
{
    return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString::FString(str.c_str()));
}