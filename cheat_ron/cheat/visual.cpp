#include "visual.h"

#include "gvalue.h"
#include "_sdk.h"
#include "render.h"

class param
{
public:
   inline static std::unordered_map<std::string, SDK::FName> bone_list;
};

class function
{
public:
    static SDK::FName to_name(const std::wstring& str)
    {
        return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString::FString(str.c_str()));
    }
};

visual* visual::get()
{
    static visual inst;
    return &inst;
}

void visual::init()
{
    param::bone_list["neck_01"] = function::to_name(L"neck_1");
    param::bone_list["upperarm_l"] = function::to_name(L"upperarm_LE");
    param::bone_list["lowerarm_l"] = function::to_name(L"lowerarm_LE");
    param::bone_list["hand_l"] = function::to_name(L"hand_LE");
    param::bone_list["upperarm_r"] = function::to_name(L"upperarm_RI");
    param::bone_list["lowerarm_r"] = function::to_name(L"lowerarm_RI");
    param::bone_list["hand_r"] = function::to_name(L"hand_RI");
    param::bone_list["spine_01"] = function::to_name(L"spine_1");
    param::bone_list["spine_02"] = function::to_name(L"spine_2");
    param::bone_list["pelvis"] = function::to_name(L"pelvis");
    param::bone_list["thigh_l"] = function::to_name(L"thigh_LE");
    param::bone_list["calf_l"] = function::to_name(L"calf_LE");
    param::bone_list["foot_l"] = function::to_name(L"foot_LE");
    param::bone_list["thigh_r"] = function::to_name(L"thigh_RI");
    param::bone_list["calf_r"] = function::to_name(L"calf_RI");
    param::bone_list["foot_r"] = function::to_name(L"foot_RI");
}

void visual::main()
{
	if (!gvalue::world || !gvalue::controller)
	{
		return;
	}
    domain();
	camera();
}

std::wstring visual::find_name(const std::wstring& in_name)
{
	return std::wstring();
}

void visual::domain()
{
    auto dispatch_type = [&](SDK::AActor* actor)
        {
            if (!actor || !actor->RootComponent)
            {
                return;
            }

            if (actor->IsA(SDK::ABasePlayer_C::StaticClass()))
            {
                SDK::ABasePlayer_C* player = static_cast<SDK::ABasePlayer_C*>(actor);
                std::wstring name = L"玩家：" + player->PlayerState->GetPlayerName().ToWString();
                draw_character(
                    actor,
                    SDK::FLinearColor(0, 1, 0, 1),
                    name,
                    gvalue::esp_player
                );
                return;
            }
            if (actor->IsA(SDK::ASuspectCharacter::StaticClass()))
            {
                draw_character(
                    actor,
                    SDK::FLinearColor(1, 0, 0, 1),
                    L"嫌疑人",
                    gvalue::esp_suspect
                );
                return;
            }
            if (actor->IsA(SDK::ACivilianCharacter::StaticClass()))
            {
                draw_character(
                    actor,
                    SDK::FLinearColor(1, 1, 1, 1),
                    L"平民",
                    gvalue::esp_civilian
                );
                return;
            }

            if (actor->IsA(SDK::ATrapActor::StaticClass()))
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
                    actor,
                    color,
                    L"陷阱",
                    status,
                    gvalue::esp_trap
                );
                return;
            }
            if (actor->IsA(SDK::AReportableActor::StaticClass()))
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
                    actor,
                    color,
                    L"报告",
                    status,
                    gvalue::esp_report
                );
                return;
            }
            if (actor->IsA(SDK::ABombActor::StaticClass()))
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
                    actor,
                    color,
                    L"炸弹",
                    status,
                    gvalue::esp_bomb
                );
                return;
            }
            if (actor->IsA(SDK::ABaseWeapon::StaticClass()))
            {
                SDK::ABaseWeapon* target = static_cast<SDK::ABaseWeapon*>(actor);
                std::wstring status;
                SDK::FLinearColor color(0, 1, 1, 1);
                if (target->GetOwner())
                {
                    if (target->GetOwner() == gvalue::controller->Pawn)
                    {
                        return;
                    }
                    status = L"握持";
                    color = SDK::FLinearColor(0, 0.2, 0.2, 1);
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
                return;
            }
        };

    for (SDK::ULevel* level : gvalue::world->Levels)
    {
        for (SDK::AActor* actor : level->Actors)
        {
            dispatch_type(actor);
        }
    }
}

void visual::camera()
{
    if (!gvalue::controller->Pawn || !gvalue::controller->Pawn->IsA(SDK::ABasePlayer_C::StaticClass()))
    {
        return;
    }

    {
        uintptr_t fov_addr = (uintptr_t)gvalue::controller->Pawn;
        fov_addr += 0x22F0;
        float* ptr = reinterpret_cast<float*>(fov_addr);
        *ptr = gvalue::fov * 180;
    }

    SDK::ABasePlayer_C* character = static_cast<SDK::ABasePlayer_C*>(gvalue::controller->Pawn);

    static SDK::ACameraActor* tpp_camera = nullptr;
    static SDK::APawn* cur_pawn = nullptr;

    if (cur_pawn != gvalue::controller->Pawn)
    {
        tpp_camera = nullptr;
        cur_pawn = gvalue::controller->Pawn;
    }

    static bool do_once_tpp = false;
    static bool do_once_fpp = false;
    if (gvalue::third_person)
    {
        if (!tpp_camera)
        {
            SDK::FTransform trans;
            SDK::AActor* new_camera = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(
                gvalue::world,
                SDK::ACameraActor::StaticClass(),
                trans,
                SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
                character,
                SDK::ESpawnActorScaleMethod::MultiplyWithRoot
            );
            SDK::UGameplayStatics::FinishSpawningActor(new_camera, trans, SDK::ESpawnActorScaleMethod::MultiplyWithRoot);
            tpp_camera = static_cast<SDK::ACameraActor*>(new_camera);
        }
        else
        {
            SDK::FVector trace_start = character->K2_GetActorLocation();
            trace_start += character->GetActorUpVector() * (-250 + gvalue::cam_y * 500);
            trace_start += character->GetActorRightVector() * (-250 + gvalue::cam_x * 500);
            SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(character->GetControlRotation()) * -1 * gvalue::cam_distance * 1000;
            SDK::TArray<SDK::AActor*> ignore_actors;
            ignore_actors.Add(character);
            SDK::FHitResult result;
            SDK::UKismetSystemLibrary::LineTraceSingle(
                gvalue::world,
                trace_start,
                trace_end,
                SDK::ETraceTypeQuery::TraceTypeQuery4,
                false,
                ignore_actors,
                SDK::EDrawDebugTrace::None,
                &result,
                true,
                SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
                SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f),
                0.0f
            );

            tpp_camera->K2_SetActorLocation(result.bBlockingHit ? result.Location : result.TraceEnd, false, nullptr, true);
            tpp_camera->K2_SetActorRotation(character->GetControlRotation(), false);

            tpp_camera->CameraComponent->FieldOfView = static_cast<float>(gvalue::fov * 180);
        }

        if (!do_once_tpp)
        {
            printf("tpp\n");
            gvalue::controller->SetViewTargetWithBlend(
                tpp_camera,
                0.2f,
                SDK::EViewTargetBlendFunction::VTBlend_EaseInOut,
                2.0f,
                false
            );
            do_once_tpp = true;
        }
        do_once_fpp = false;
    }
    else
    {
        if (!do_once_fpp)
        {
            printf("fpp\n");
            gvalue::controller->SetViewTargetWithBlend(
                character,
                0.2f,
                SDK::EViewTargetBlendFunction::VTBlend_EaseInOut,
                2.0f,
                false
            );
            do_once_fpp = true;
        }
        do_once_tpp = false;
    }
}

bool visual::get_box(SDK::USceneComponent* comp, SDK::FVector2D& min, SDK::FVector2D& max)
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

bool visual::get_origin(SDK::AActor* actor, SDK::FVector2D& pos)
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

void visual::draw_box(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color)
{
    render::draw_box(
        min,
        SDK::FVector2D(max.X - min.X, max.Y - min.Y),
        1,
        color
    );
}

void visual::draw_line(const SDK::FVector2D& min, const SDK::FVector2D& max, const SDK::FLinearColor& color)
{
    render::draw_line(
        SDK::FVector2D(gvalue::canvas->SizeX / 2, 0.0f),
        SDK::FVector2D(max.X - (max.X - min.X) / 2, min.Y),
        1,
        color
    );
}

void visual::draw_extent(SDK::USceneComponent* comp, const SDK::FLinearColor& color)
{
    if (!comp)
    {
        return;
    }

    struct line_01
    {
        SDK::FVector2D p1;
        SDK::FVector2D p2;
    };

    SDK::FVector origin, extent;
    float radius;
    SDK::UKismetSystemLibrary::GetComponentBounds(comp, &origin, &extent, &radius);
    const SDK::FVector point[] =
    {
        origin + SDK::FVector(extent.X, extent.Y, extent.Z),        //0
        origin + SDK::FVector(-extent.X, -extent.Y, -extent.Z),     //1
        origin + SDK::FVector(extent.X, -extent.Y, extent.Z),       //2
        origin + SDK::FVector(extent.X, extent.Y, -extent.Z),       //3
        origin + SDK::FVector(extent.X, -extent.Y, -extent.Z),      //4
        origin + SDK::FVector(-extent.X, extent.Y, extent.Z),       //5
        origin + SDK::FVector(-extent.X, extent.Y, -extent.Z),      //6
        origin + SDK::FVector(-extent.X, -extent.Y, extent.Z)       //7
    };

    SDK::FVector2D point_2d[8];
    for (int i = 0; i < 8; i++)
    {
        SDK::FVector2D ori_2d;
        bool in_view = gvalue::controller->ProjectWorldLocationToScreen(point[i], &ori_2d, true);
        if (!in_view)
        {
            ori_2d = SDK::FVector2D(-114514, -114514);
        }
        point_2d[i] = ori_2d;
    }

    const line_01 line[12] =
    {
        {point_2d[0],point_2d[2]},
        {point_2d[0],point_2d[3]},
        {point_2d[0],point_2d[5]},

        {point_2d[1],point_2d[4]},
        {point_2d[1],point_2d[6]},
        {point_2d[1],point_2d[7]},

        {point_2d[2],point_2d[4]},
        {point_2d[2],point_2d[7]},

        {point_2d[3],point_2d[4]},
        {point_2d[3],point_2d[6]},

        {point_2d[5],point_2d[6]},
        {point_2d[5],point_2d[7]}
    };

    for (int i = 0; i < 12; i++)
    {
        if (line[i].p1 == SDK::FVector2D(-114514, -114514) ||
            line[i].p2 == SDK::FVector2D(-114514, -114514))
        {
            continue;
        }
        render::draw_line(
            line[i].p1,
            line[i].p2,
            1,
            color
        );
    }
}

void visual::draw_skeleton(SDK::USkeletalMeshComponent* comp, const SDK::FLinearColor& color)
{
    if (!comp)
    {
        return;
    }

    std::unordered_map<std::string, SDK::FVector2D> bone_pos_2d;
    for (const auto& cur_bone : param::bone_list)
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

void visual::draw_character(SDK::AActor* actor, SDK::FLinearColor color, const std::wstring& name, const s_esp_character& esp)
{
    SDK::AReadyOrNotCharacter* character = static_cast<SDK::AReadyOrNotCharacter*>(actor);

    if (!esp.enable)
    {
        return;
    }
    const int dist = static_cast<int>(SDK::UKismetMathLibrary::Vector_Distance(gvalue::controller->PlayerCameraManager->GetCameraLocation(), character->Mesh->K2_GetComponentLocation()) / 100.0f);
    if (dist > gvalue::esp_distance * 1000)
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

    if (esp.extent)
    {
        draw_extent(character->Mesh, color);
    }
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

void visual::draw_item(SDK::AActor* actor, const SDK::FLinearColor& color, const std::wstring& name, const std::wstring& status, const s_esp_item& esp)
{
    if (!esp.enable)
    {
        return;
    }
    const int dist = static_cast<int>(SDK::UKismetMathLibrary::Vector_Distance(gvalue::controller->PlayerCameraManager->GetCameraLocation(), actor->K2_GetActorLocation()) / 100.0f);
    if (dist > gvalue::esp_distance * 1000)
    {
        return;
    }

    if (esp.extent)
    {
        draw_extent(actor->RootComponent, color);
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
