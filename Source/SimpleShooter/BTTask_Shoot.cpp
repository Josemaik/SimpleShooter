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
	UE_LOG(LogTemp, Warning, TEXT("ejecuto"));
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Falloo"));
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ShooterCharacter == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Falloo"));
		return EBTNodeResult::Failed;
	}
	//ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;

	//Dont shoot when player is far away
	/*APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	float MaxRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("MaxRadius"));
	UE_LOG(LogTemp, Warning, TEXT("MaxRadius:%f"), MaxRadius);
	UE_LOG(LogTemp, Warning, TEXT("Dist:%f"), Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()));
	if (Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) > 40000.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Falloo"));
		return EBTNodeResult::Failed;
	}
	UE_LOG(LogTemp, Warning, TEXT("Disparo"));*/
	ShooterCharacter->Shoot();

	return EBTNodeResult::Succeeded;
}