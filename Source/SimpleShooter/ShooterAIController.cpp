// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	//initialice player
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//run behavuourTree
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//if (LineOfSightTo(PlayerPawn))
	//{
	//	//AI aim to us
	//	SetFocus(PlayerPawn);
	//	//AI move to us
	//	MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//else {
	//	//AI stop focus us and stop move
	//	ClearFocus(EAIFocusPriority::Gameplay);
	//	StopMovement();
	//}
}