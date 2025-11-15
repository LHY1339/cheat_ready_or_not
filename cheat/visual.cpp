#include "visual.h"

#include "gvalue.h"
#include "_sdk.h"
#include "render.h"

#include <unordered_map>

std::unordered_map<std::wstring, std::wstring> name_map;
std::unordered_map<std::string, SDK::FName> bone_map;

void visual::init()
{
    name_map[L"Cybernetics_Civilian_V2_C"] = L"人质";
    name_map[L"CyberneticsSuspect_V2_C"] = L"嫌疑人";

    bone_map["neck_01"] = to_name(L"neck_1");
    bone_map["upperarm_l"] = to_name(L"upperarm_LE");
    bone_map["lowerarm_l"] = to_name(L"lowerarm_LE");
    bone_map["hand_l"] = to_name(L"hand_LE");
    bone_map["upperarm_r"] = to_name(L"upperarm_RI");
    bone_map["lowerarm_r"] = to_name(L"lowerarm_RI");
    bone_map["hand_r"] = to_name(L"hand_RI");
    bone_map["spine_01"] = to_name(L"spine_1");
    bone_map["spine_02"] = to_name(L"spine_2");
    bone_map["pelvis"] = to_name(L"pelvis");
    bone_map["thigh_l"] = to_name(L"thigh_LE");
    bone_map["calf_l"] = to_name(L"calf_LE");
    bone_map["foot_l"] = to_name(L"foot_LE");
    bone_map["thigh_r"] = to_name(L"thigh_RI");
    bone_map["calf_r"] = to_name(L"calf_RI");
    bone_map["foot_r"] = to_name(L"foot_RI");
}

void visual::main()
{
	if (!gvalue::world || !gvalue::controller)
	{
		return;
	}
	get_all();
	camera();
}

void visual::get_all()
{
    SDK::TArray<SDK::AActor*> actor_list;
    SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::AActor::StaticClass(), &actor_list);
    for (SDK::AActor* actor : actor_list)
    {
        if (!actor || !actor->RootComponent)
        {
            continue;
        }

        if (actor->IsA(SDK::ABasePlayer_C::StaticClass()))
        {
            draw_character(
                actor->RootComponent,
                SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f),
                L"玩家",
                gvalue::visual_player
            );
            continue;
        }
        if (actor->IsA(SDK::ASuspectCharacter::StaticClass()))
        {
            draw_character(
                actor->RootComponent,
                SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
                SDK::UKismetStringLibrary::Conv_NameToString(actor->Class->Name),
                gvalue::visual_suspect
            );
            continue;
        }
        if (actor->IsA(SDK::ACivilianCharacter::StaticClass()))
        {
            SDK::ACharacter* pawn = static_cast<SDK::ACharacter*>(actor);
            draw_character(
                actor->RootComponent,
                SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
                SDK::UKismetStringLibrary::Conv_NameToString(actor->Class->Name),
                gvalue::visual_civilian
            );
            draw_skeleton(pawn->Mesh, SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
            continue;
        }
    }
}

void visual::camera()
{
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
        bool in_view = gvalue::controller->ProjectWorldLocationToScreen(point[i], &ori_2d, true);
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
            return;
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
        render::draw_line(
            line[i].p1,
            line[i].p2,
            1.0f,
            color
        );
    }
}

void visual::draw_skeleton(SDK::USkeletalMeshComponent* mesh, const SDK::FLinearColor& color)
{
    if (!mesh)
    {
        return;
    }

    std::unordered_map<std::string, SDK::FVector2D> bone_pos_2d;
    for (const auto& cur_bone : bone_map)
    {
        SDK::FVector2D pos_2d;
        const SDK::FVector location = mesh->GetSocketLocation(cur_bone.second);
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
        const SDK::FVector4 cur_line = lines[i];

        render::draw_line(
            SDK::FVector2D(cur_line.X, cur_line.Y),
            SDK::FVector2D(cur_line.Z, cur_line.W),
            1.0f,
            color
        );
    }
}

void visual::draw_character(SDK::USceneComponent* comp, const SDK::FLinearColor& color, const UC::FString& name, const s_character_visual& visual)
{
    if (!visual.enable)
    {
        return;
    }

    const int dist = static_cast<int>(SDK::UKismetMathLibrary::Vector_Distance(gvalue::controller->PlayerCameraManager->GetCameraLocation(), comp->K2_GetComponentLocation()) / 100.0f);
    if (dist > gvalue::visual_distance)
    {
        return;
    }

    if (visual.extent)
    {
        draw_extent(comp, color);
    }

    SDK::FVector2D min, max;
    bool in_view = get_box(comp, min, max);
    if (in_view)
    {
        if (visual.box)
        {
            render::draw_box(
                min,
                SDK::FVector2D(max.X - min.X, max.Y - min.Y),
                1.0f,
                color
            );
        }
        if (visual.line)
        {
            render::draw_line(
                SDK::FVector2D(gvalue::canvas->SizeX / 2, 0.0f),
                SDK::FVector2D(max.X - (max.X - min.X) / 2, min.Y),
                1.0f,
                color
            );
        }
        std::wstring draw_str;
        if (visual.name)
        {
            draw_str += find_name(name.CStr()) + L"\n";
        }
        if (visual.distance)
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

std::wstring visual::find_name(const std::wstring& in_name)
{
    auto it = name_map.find(in_name);
    if (it != name_map.end())
    {
        return it->second;
    }
    wprintf(L"%ls\n", in_name.c_str());
    return in_name;
}

SDK::FName visual::to_name(std::wstring str)
{
    return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString::FString(str.c_str()));
}
