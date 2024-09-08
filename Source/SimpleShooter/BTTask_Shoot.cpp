// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ShooterCharacter == nullptr) 
	{
		return EBTNodeResult::Failed;
	}
	//ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;

	//Dont shoot when player is far away
	APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	if (Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) > 700.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Está muy alejado"));
		return EBTNodeResult::Failed;
	}
		
	ShooterCharacter->Shoot();

	return EBTNodeResult::Succeeded;
}