// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Teleport.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

UBTTask_Teleport::UBTTask_Teleport()
{
	NodeName = TEXT("Teleport");
}

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    //get pawn controled by boss
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    if (ControlledPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    //get the player
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerActor == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Display, TEXT("Ejecutamos teletransporte xd"));
    // get player current location
    FVector PlayerLocation = PlayerActor->GetActorLocation();

    // Define radius for teleport
    float MinTeleportDistance = 100.0f; // Distancia mínima del teletransporte
    float MaxTeleportDistance = 500.0f; // Distancia máxima del teletransporte

    // Generate random angle on radians  (0 a 360)
    float RandomAngle = FMath::RandRange(0.0f, 2 * PI);

    // Generate random distance between min and max range
    float RandomDistance = FMath::RandRange(MinTeleportDistance, MaxTeleportDistance);

    // Calcualte final position in radius around the player
    FVector TeleportLocation;
    TeleportLocation.X = PlayerLocation.X + RandomDistance * FMath::Cos(RandomAngle);
    TeleportLocation.Y = PlayerLocation.Y + RandomDistance * FMath::Sin(RandomAngle);
    TeleportLocation.Z = ControlledPawn->GetActorLocation().Z + 50.f;

    // Validate location ( collisions )
    FHitResult HitResult;
    bool bTeleportSuccess = GetWorld()->SweepSingleByChannel(
        HitResult,
        TeleportLocation,
        TeleportLocation,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(100.0f) 
    );
    if (bTeleportSuccess)
    {
        UE_LOG(LogTemp, Display, TEXT("Siuuuuuuuu"));
    }
    else {
        UE_LOG(LogTemp, Display, TEXT("Mehhhhhhhh"));
    }

    /*if (!bTeleportSuccess)
    {
        return EBTNodeResult::Failed;
    }*/
    UE_LOG(LogTemp, Display, TEXT("ME teletransporto"));
    // Teleport boss to new position
    ControlledPawn->SetActorLocation(TeleportLocation);

    return EBTNodeResult::Failed;
}