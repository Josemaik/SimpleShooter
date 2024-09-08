// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetVelocitytoOriginal.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetVelocitytoOriginal::UBTTask_SetVelocitytoOriginal()
{
	NodeName = TEXT("Set Velocity to Original");
}

EBTNodeResult::Type UBTTask_SetVelocitytoOriginal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//GetSelectedBlackboardKey()
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(),false);

	return EBTNodeResult::Succeeded;
}