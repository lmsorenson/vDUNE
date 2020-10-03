// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarTool.h"
#include "Engine/StaticMeshActor.h"
#include "../DuneAvatar.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"


UAvatarTool::UAvatarTool()
: UAvatarMode()
{
}

void UAvatarTool::setup(APawn * pawn, FTimerManager *manager)
{
    UAvatarMode::setup(pawn, manager);
}


void UAvatarTool::update()
{
    UAvatarMode::update();

    ADuneAvatar * avatar = Cast<ADuneAvatar>(pawn_);

    if (!avatar)
        return;

    auto up = FVector(0, 0, 1);
    auto forward = avatar->GetFollowCamera()->GetForwardVector();
    FVector start = avatar->GetActorLocation();
    FVector end = ((forward * 1000.0f)) + start;

    FHitResult result;
    FCollisionQueryParams collision_params;
    if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Visibility, collision_params))
    {
        if(result.bBlockingHit)
        {
            DrawDebugLine(GetWorld(), start + FVector(0,0,100), result.ImpactPoint, FColor::Cyan, false, 0.5f, 0, 1);
            DrawDebugSphere(GetWorld(), result.ImpactPoint, 50, 12, FColor(181,0,0), false, 1.0f, 0, 2);
        }
    }
}

FVector UAvatarTool::calculate_camera_displacement(FVector forward, FVector start)
{
    auto up_vector = FVector(0,0,1);

    return (FVector::CrossProduct(up_vector, forward) * 100) + start + FVector(0,0,25);
}

float UAvatarTool::calculate_camera_arm_length()
{
    return 150.0f;
}