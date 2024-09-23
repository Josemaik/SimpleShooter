// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition_AvailableBullets.h"
#include "BehaviorTree/BlackboardComponent.h"

UCondition_AvailableBullets::UCondition_AvailableBullets()
{
	NodeName = TEXT("HaveBullets?");
}

EBTNodeResult::Type UCondition_AvailableBullets::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int bullets = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("Bullets"));
	if (bullets > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Much bullets"));
		return EBTNodeResult::Succeeded;
	}
	UE_LOG(LogTemp, Display, TEXT("No bullets"));
	return EBTNodeResult::Failed;
}