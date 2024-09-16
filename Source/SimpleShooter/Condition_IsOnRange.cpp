// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition_IsOnRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UCondition_IsOnRange::UCondition_IsOnRange()
{
	NodeName = TEXT("IsInRange");
}

EBTNodeResult::Type UCondition_IsOnRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	float MaxRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("MaxRadius"));
	UE_LOG(LogTemp, Warning, TEXT("MaxRadius:%f"), MaxRadius);
	UE_LOG(LogTemp, Warning, TEXT("Dist:%f"), Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()));
	if (Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) < MaxRadius)
	{
		//esta a rango, disparo
		UE_LOG(LogTemp, Display, TEXT("PASO A DISPARAR"));
		return EBTNodeResult::Succeeded;
	}
	//no esta en rango, paso a moverme
	UE_LOG(LogTemp, Display, TEXT("PASO A MOVERME"));
	return EBTNodeResult::Failed;
}