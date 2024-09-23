// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetVelocitytoOriginal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterAIController.h"

UBTTask_SetVelocitytoOriginal::UBTTask_SetVelocitytoOriginal()
{
	NodeName = TEXT("Set Velocity to Original");
}

EBTNodeResult::Type UBTTask_SetVelocitytoOriginal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//GetSelectedBlackboardKey()
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(),false);

	/*OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("ShootMove"), true);*/
	AShooterAIController* aic = Cast<AShooterAIController>(OwnerComp.GetAIOwner());
	aic->SetShootingMove(true);

	return EBTNodeResult::Succeeded;
}