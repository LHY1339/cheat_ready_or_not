#include "visual.h"
#include "sdk/SDK/Engine_classes.hpp"
#include "sdk/SDK/BasePlayer_classes.hpp"
#include "gvalue.h"

visual* visual::get()
{
	static visual inst;
	return &inst;
}

void visual::init()
{

}

void visual::main()
{
    if (!gvalue::controller || !gvalue::controller->Pawn || !gvalue::controller->Pawn->IsA(SDK::ABasePlayer_C::StaticClass()))
    {
        return;
    }

	third_person();
    fov_change();
    post();
}

void visual::third_person()
{
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

    if (gvalue::tpp)
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
            trace_start += character->GetActorUpVector() * (gvalue::tpp_camera_y);
            trace_start += character->GetActorRightVector() * (gvalue::tpp_camera_x);
            SDK::FVector trace_end = trace_start + SDK::UKismetMathLibrary::GetForwardVector(character->GetControlRotation()) * -1 * gvalue::tpp_camera_arm;
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

            tpp_camera->K2_SetActorLocation((result.bBlockingHit && gvalue::tpp_camera_collision) ? result.Location : result.TraceEnd, false, nullptr, true);
            tpp_camera->K2_SetActorRotation(character->GetControlRotation(), false);

            tpp_camera->CameraComponent->FieldOfView = static_cast<float>(gvalue::third_fov);
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

void visual::fov_change()
{
    uintptr_t fov_addr = (uintptr_t)gvalue::controller->Pawn;
    fov_addr += 0x23D8;
    float* ptr = (float*)fov_addr;
    *ptr = gvalue::first_fov;
}

void visual::post()
{
    SDK::ABasePlayer_C* character = static_cast<SDK::ABasePlayer_C*>(gvalue::controller->Pawn);
    character->FirstPersonCameraComponent->PostProcessBlendWeight = gvalue::disable_post ? 0.0f : 1.0f;
}
