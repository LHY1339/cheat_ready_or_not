#include "esp.h"
#include "sdk/SDK/Engine_classes.hpp"
#include "sdk/SDK/BasePlayer_classes.hpp"
#include "gvalue.h"
#include "render.h"
#include "kismet.h"
#include "manager.h"

esp* esp::get()
{
	static esp inst;
	return &inst;
}

void esp::init()
{

}

void esp::main()
{
    //SDK::TArray<SDK::AActor*> actor_arr;
    //SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::AActor::StaticClass(), &actor_arr);
    //for (SDK::AActor* actor : actor_arr)
    //{
    //    handle(actor);
    //}

    for (const actor_info& info : manager::get()->actor_list)
    {
        handle(info);
    }
}

//void esp::handle(SDK::AActor* actor)
//{
//    if (!actor || !actor->RootComponent)
//    {
//        return;
//    }
//
//    if (actor->IsA(SDK::ABasePlayer_C::StaticClass()) || actor->IsA(SDK::ASWATCharacter::StaticClass()))
//    {
//        handle_team(actor);
//        return;
//    }
//    if (actor->IsA(SDK::ASuspectCharacter::StaticClass()))
//    {
//        handle_suspect(actor);
//        return;
//    }
//    if (actor->IsA(SDK::ACivilianCharacter::StaticClass()))
//    {
//        handle_civilian(actor);
//        return;
//    }
//
//    if (actor->IsA(SDK::ATrapActor::StaticClass()))
//    {
//        handle_trap(actor);
//        return;
//    }
//    if (actor->IsA(SDK::AReportableActor::StaticClass()))
//    {
//        handle_report(actor);
//        return;
//    }
//    if (actor->IsA(SDK::ABombActor::StaticClass()))
//    {
//        handle_bomb(actor);
//        return;
//    }
//    if (actor->IsA(SDK::ABaseWeapon::StaticClass()))
//    {
//        handle_weapon(actor);
//        return;
//    }
//
//    if (kismet::is_key_down(VK_F2))
//    {
//        handle_test(actor);
//    }
//}

void esp::handle(const actor_info& info)
{
    if (!info.actor || !info.actor->RootComponent)
    {
        return;
    }

    switch (info.type)
    {
    case actor_type::team:
        handle_team(info.actor);
        break;
    case actor_type::civilian:
        handle_civilian(info.actor);
        break;
    case actor_type::suspect:
        handle_suspect(info.actor);
        break;
    case actor_type::trap:
        handle_trap(info.actor);
        break;
    case actor_type::report:
        handle_report(info.actor);
        break;
    case actor_type::bomb:
        handle_bomb(info.actor);
        break;
    case actor_type::weapon:
        handle_weapon(info.actor);
        break;
    case actor_type::other:
        handle_other(info.actor);
        break;
    }
}

bool esp::get_box(SDK::USceneComponent* comp, SDK::FVector2D& min, SDK::FVector2D& max)
{
    if (!comp)
    {
        return false;
    }

    SDK::FVector origin, extent;
    float radius;
    SDK::UKismetSystemLibrary::GetComponentBounds(comp, &origin, &extent, &radius);
    const SDK::FVector point[] =
    {
        origin + SDK::FVector(extent.X, extent.Y, extent.Z),
        origin + SDK::FVector(-extent.X, -extent.Y, -extent.Z),

        origin + SDK::FVector(extent.X, -extent.Y, extent.Z),
        origin + SDK::FVector(extent.X, extent.Y, -extent.Z),
        origin + SDK::FVector(extent.X, -extent.Y, -extent.Z),

        origin + SDK::FVector(-extent.X, extent.Y, extent.Z),
        origin + SDK::FVector(-extent.X, extent.Y, -extent.Z),

        origin + SDK::FVector(-extent.X, -extent.Y, extent.Z)
    };

    for (int i = 0; i < 8; i++)
    {
        SDK::FVector2D ori_2d;
        const bool in_view = gvalue::controller->ProjectWorldLocationToScreen(point[i], &ori_2d, true);
        if (!in_view)
        {
            return false;
        }

        if (i == 0)
        {
            min.X = ori_2d.X;
            min.Y = ori_2d.Y;
            max.X = ori_2d.X;
            max.Y = ori_2d.Y;
            continue;
        }
        min.X = ori_2d.X < min.X ? ori_2d.X : min.X;
        min.Y = ori_2d.Y < min.Y ? ori_2d.Y : min.Y;
        max.X = ori_2d.X > max.X ? ori_2d.X : max.X;
        max.Y = ori_2d.Y > max.Y ? ori_2d.Y : max.Y;
    }
    return true;
}

bool esp::get_origin(SDK::AActor* actor, SDK::FVector2D& pos)
{
    if (!actor)
    {
        return false;
    }

    SDK::FVector origin, extent;
    actor->GetActorBounds(true, &origin, &extent, true);
    const bool in_view = gvalue::controller->ProjectWorldLocationToScreen(origin, &pos, true);
    return in_view;
}

void esp::draw_box(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color)
{
    render::draw_box(
        min,
        SDK::FVector2D(max.X - min.X, max.Y - min.Y),
        1,
        color
    );
}

void esp::draw_line(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color)
{
    render::draw_line(
        SDK::FVector2D(gvalue::canvas->SizeX / 2, 0.0f),
        SDK::FVector2D(max.X - (max.X - min.X) / 2, min.Y),
        1,
        color
    );
}

void esp::draw_skeleton(SDK::USkeletalMeshComponent* comp, const SDK::FLinearColor& color)
{
    if (!comp)
    {
        return;
    }

    std::unordered_map<std::string, SDK::FVector2D> bone_pos_2d;
    for (const auto& cur_bone : manager::get()->bone_list)
    {
        SDK::FVector2D pos_2d;
        const SDK::FVector location = comp->GetSocketLocation(cur_bone.second);
        const bool in_view = gvalue::controller->ProjectWorldLocationToScreen(location, &pos_2d, true);
        if (!in_view)
        {
            return;
        }
        bone_pos_2d[cur_bone.first] = pos_2d;
    }

    const SDK::FVector4 lines[] =
    {
        SDK::FVector4(bone_pos_2d["neck_01"].X,bone_pos_2d["neck_01"].Y,bone_pos_2d["upperarm_l"].X,bone_pos_2d["upperarm_l"].Y),
        SDK::FVector4(bone_pos_2d["upperarm_l"].X,bone_pos_2d["upperarm_l"].Y,bone_pos_2d["lowerarm_l"].X,bone_pos_2d["lowerarm_l"].Y),
        SDK::FVector4(bone_pos_2d["lowerarm_l"].X,bone_pos_2d["lowerarm_l"].Y,bone_pos_2d["hand_l"].X,bone_pos_2d["hand_l"].Y),

        SDK::FVector4(bone_pos_2d["neck_01"].X,bone_pos_2d["neck_01"].Y,bone_pos_2d["upperarm_r"].X,bone_pos_2d["upperarm_r"].Y),
        SDK::FVector4(bone_pos_2d["upperarm_r"].X,bone_pos_2d["upperarm_r"].Y,bone_pos_2d["lowerarm_r"].X,bone_pos_2d["lowerarm_r"].Y),
        SDK::FVector4(bone_pos_2d["lowerarm_r"].X,bone_pos_2d["lowerarm_r"].Y,bone_pos_2d["hand_r"].X,bone_pos_2d["hand_r"].Y),

        SDK::FVector4(bone_pos_2d["neck_01"].X,bone_pos_2d["neck_01"].Y,bone_pos_2d["spine_02"].X,bone_pos_2d["spine_02"].Y),
        SDK::FVector4(bone_pos_2d["spine_02"].X,bone_pos_2d["spine_02"].Y,bone_pos_2d["spine_01"].X,bone_pos_2d["spine_01"].Y),
        SDK::FVector4(bone_pos_2d["spine_01"].X,bone_pos_2d["spine_01"].Y,bone_pos_2d["pelvis"].X,bone_pos_2d["pelvis"].Y),

        SDK::FVector4(bone_pos_2d["pelvis"].X,bone_pos_2d["pelvis"].Y,bone_pos_2d["thigh_l"].X,bone_pos_2d["thigh_l"].Y),
        SDK::FVector4(bone_pos_2d["thigh_l"].X,bone_pos_2d["thigh_l"].Y,bone_pos_2d["calf_l"].X,bone_pos_2d["calf_l"].Y),
        SDK::FVector4(bone_pos_2d["calf_l"].X,bone_pos_2d["calf_l"].Y,bone_pos_2d["foot_l"].X,bone_pos_2d["foot_l"].Y),

        SDK::FVector4(bone_pos_2d["pelvis"].X,bone_pos_2d["pelvis"].Y,bone_pos_2d["thigh_r"].X,bone_pos_2d["thigh_r"].Y),
        SDK::FVector4(bone_pos_2d["thigh_r"].X,bone_pos_2d["thigh_r"].Y,bone_pos_2d["calf_r"].X,bone_pos_2d["calf_r"].Y),
        SDK::FVector4(bone_pos_2d["calf_r"].X,bone_pos_2d["calf_r"].Y,bone_pos_2d["foot_r"].X,bone_pos_2d["foot_r"].Y)
    };

    for (int i = 0; i < 15; i++)
    {
        const SDK::FVector4 line = lines[i];
        render::draw_line(
            SDK::FVector2D(line.X, line.Y),
            SDK::FVector2D(line.Z, line.W),
            1,
            color
        );
    }
}

void esp::draw_character(SDK::AActor* actor, SDK::FLinearColor color, const std::wstring& name, const s_esp_character& esp)
{
    SDK::AReadyOrNotCharacter* character = static_cast<SDK::AReadyOrNotCharacter*>(actor);

    if (!esp.enable)
    {
        return;
    }
    const int dist = static_cast<int>(SDK::UKismetMathLibrary::Vector_Distance(gvalue::controller->PlayerCameraManager->GetCameraLocation(), character->Mesh->K2_GetComponentLocation()) / 100.0f);
    if (dist > gvalue::esp_distance)
    {
        return;
    }

#define COLOR(_type_) color._type_ = color._type_ > 0 ? 0.2 : 0;

    switch (character->CharacterHealth->HealthStatus)
    {
    case SDK::EPlayerHealthStatus::HS_Downed:
    case SDK::EPlayerHealthStatus::HS_Dead:
    case SDK::EPlayerHealthStatus::HS_Incapacitated:
    case SDK::EPlayerHealthStatus::HS_Arrested:
        COLOR(R);
        COLOR(G);
        COLOR(B);
        break;
    }

#undef COLOR

    if (esp.skeleton)
    {
        draw_skeleton(character->Mesh, color);
    }

    SDK::FVector2D min, max;
    bool in_view = get_box(character->Mesh, min, max);
    if (in_view)
    {
        if (esp.box)
        {
            draw_box(min, max, color);
        }
        if (esp.line)
        {
            draw_line(min, max, color);
        }
        std::wstring draw_str;
        if (esp.name)
        {
            draw_str += name + L"\n";
        }
        if (esp.status)
        {
            std::wstring status;
            switch (character->CharacterHealth->HealthStatus)
            {
            case SDK::EPlayerHealthStatus::HS_Healthy:
                status = L"健康";
                break;
            case SDK::EPlayerHealthStatus::HS_Injured:
                status = L"受伤";
                break;
            case SDK::EPlayerHealthStatus::HS_Downed:
                status = L"倒地";
                break;
            case SDK::EPlayerHealthStatus::HS_Dead:
                status = L"死亡";
                break;
            case SDK::EPlayerHealthStatus::HS_Incapacitated:
                status = L"无法行动";
                break;
            case SDK::EPlayerHealthStatus::HS_Arrested:
                status = L"已逮捕";
                break;
            default:
                status = L"未知";
                break;
            }

            draw_str += status + L"\n";
        }
        if (esp.distance)
        {
            draw_str += std::to_wstring(dist) + L"米\n";
        }
        render::draw_text(
            gvalue::engine->MediumFont,
            draw_str.c_str(),
            SDK::FVector2D(max.X + 5.0f, min.Y),
            SDK::FVector2D(1.0f, 1.0f),
            color,
            1.0f,
            SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
            SDK::FVector2D(0.0f, 0.0f),
            false,
            false,
            false,
            SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
        );
    }
}

void esp::draw_item(SDK::AActor* actor, const SDK::FLinearColor& color, const std::wstring& name, const std::wstring& status, const s_esp_item& esp)
{
    if (!esp.enable)
    {
        return;
    }
    const int dist = static_cast<int>(SDK::UKismetMathLibrary::Vector_Distance(gvalue::controller->PlayerCameraManager->GetCameraLocation(), actor->K2_GetActorLocation()) / 100.0f);
    if (dist > gvalue::esp_distance)
    {
        return;
    }

    SDK::FVector2D pos;
    bool in_view = get_origin(actor, pos);
    if (in_view)
    {
        if (esp.circle)
        {
            render::draw_circle(pos, 5, 1, 6, color);
        }
        if (esp.line)
        {
            draw_line(pos - SDK::FVector2D(5, 5), pos + SDK::FVector2D(5, 5), color);
        }
        std::wstring draw_str;
        if (esp.name)
        {
            draw_str += name + L"\n";
        }
        if (esp.status)
        {
            draw_str += status + L"\n";
        }
        if (esp.distance)
        {
            draw_str += std::to_wstring(dist) + L"米\n";
        }
        render::draw_text(
            gvalue::engine->MediumFont,
            draw_str.c_str(),
            SDK::FVector2D(pos.X + 10, pos.Y),
            SDK::FVector2D(1.0f, 1.0f),
            color,
            1.0f,
            SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
            SDK::FVector2D(0.0f, 0.0f),
            false,
            false,
            false,
            SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
        );
    }
}

void esp::handle_team(SDK::AActor* actor)
{
    std::wstring name = L"";
    if (actor->IsA(SDK::ABasePlayer_C::StaticClass()))
    {
        SDK::ABasePlayer_C* player = static_cast<SDK::ABasePlayer_C*>(actor);
        name = L"玩家：";
        if (player && player->PlayerState)
        {
            name += player->PlayerState->GetPlayerName().ToWString();
        }
    }
    else
    {
        name = L"AI队友";
    }
    draw_character(
        actor,
        SDK::FLinearColor(0, 1, 0, 1),
        name,
        gvalue::esp_team
    );
}

void esp::handle_civilian(SDK::AActor* actor)
{
    draw_character(
        actor,
        SDK::FLinearColor(1, 1, 1, 1),
        L"平民",
        gvalue::esp_civilian
    );
}

void esp::handle_suspect(SDK::AActor* actor)
{
    draw_character(
        actor,
        SDK::FLinearColor(1, 0, 0, 1),
        L"嫌疑人",
        gvalue::esp_suspect
    );
}

void esp::handle_trap(SDK::AActor* actor)
{
    SDK::ATrapActor* target = static_cast<SDK::ATrapActor*>(actor);
    std::wstring status;
    SDK::FLinearColor color(1, 1, 0, 1);

    switch (target->TrapStatus)
    {
    case SDK::ETrapState::TS_Live:
        status = L"激活";
        break;
    case SDK::ETrapState::TS_Activated:
        status = L"已触发";
        color = SDK::FLinearColor(0.2, 0.2, 0, 1);
        break;
    case SDK::ETrapState::TS_Disabled:
        status = L"已拆除";
        color = SDK::FLinearColor(0.2, 0.2, 0, 1);
        break;
    }
    draw_item(
        target,
        color,
        L"陷阱",
        status,
        gvalue::esp_trap
    );
}

void esp::handle_report(SDK::AActor* actor)
{
    SDK::AReportableActor* target = static_cast<SDK::AReportableActor*>(actor);
    std::wstring status;
    SDK::FLinearColor color(0, 1, 1, 1);
    if (target->bReportableEnabled)
    {
        status = L"未报告";
    }
    else
    {
        status = L"已报告";
        color = SDK::FLinearColor(0, 0.2, 0.2, 1);
    }
    draw_item(
        target,
        color,
        L"报告",
        status,
        gvalue::esp_report
    );
}

void esp::handle_bomb(SDK::AActor* actor)
{
    SDK::ABombActor* target = static_cast<SDK::ABombActor*>(actor);
    std::wstring status;
    SDK::FLinearColor color(1, 1, 0, 1);
    switch (target->BombState)
    {
    case SDK::EBombState::BS_Active:
        status = L"激活";
        break;
    case SDK::EBombState::BS_Disabled:
        status = L"已拆除";
        color = SDK::FLinearColor(0.2, 0.2, 0, 1);
        break;
    case SDK::EBombState::BS_Exploded:
        status = L"已爆炸";
        color = SDK::FLinearColor(0.2, 0.2, 0, 1);
        break;
    default:
        status = L"未知";
        break;
    }
    draw_item(
        target,
        color,
        L"炸弹",
        status,
        gvalue::esp_bomb
    );
}

void esp::handle_weapon(SDK::AActor* actor)
{
    std::wstring status;
    SDK::FLinearColor color(0, 1, 1, 1);
    if (actor->GetOwner())
    {
        if (actor->GetOwner() == gvalue::controller->Pawn)
        {
            return;
        }
        status = L"握持";
        color = SDK::FLinearColor(0, 0.1, 0.1, 1);
    }
    else
    {
        status = L"已掉落";
    }
    draw_item(
        actor,
        color,
        L"武器",
        status,
        gvalue::esp_weapon
    );
}

void esp::handle_other(SDK::AActor* actor)
{
}

void esp::handle_test(SDK::AActor* actor)
{
    const int dist = static_cast<int>(SDK::UKismetMathLibrary::Vector_Distance(gvalue::controller->PlayerCameraManager->GetCameraLocation(), actor->K2_GetActorLocation()) / 100.0f);
    if (dist > 20)
    {
        return;
    }

    SDK::FVector2D pos_2d;
    if (get_origin(actor, pos_2d))
    {
        render::draw_text(
            gvalue::engine->MediumFont,
            SDK::UKismetStringLibrary::Conv_NameToString(actor->Class->Name),
            pos_2d,
            SDK::FVector2D(1.0f, 1.0f),
            SDK::FLinearColor(1, 1, 1, 1), 
            1.0f,
            SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
            SDK::FVector2D(0.0f, 0.0f),
            false,
            false,
            false,
            SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
        );
    }
}
