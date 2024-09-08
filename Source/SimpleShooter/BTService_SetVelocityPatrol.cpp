// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetVelocityPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_SetVelocityPatrol::UBTService_SetVelocityPatrol()
{
	NodeName = "Set Patrol Velocity";
}
void UBTService_SetVelocityPatrol::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ShooterCharacter == nullptr)
	{
		return;
	}

	float speed = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());
	ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = speed;

}