// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_WaitwithAnimation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTaskNode_WaitwithAnimation::UBTTaskNode_WaitwithAnimation()
{
	NodeName = TEXT("Wait_and_Animate");
}

EBTNodeResult::Type UBTTaskNode_WaitwithAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (!timerinit)
	{
		FTimerHandle PlayerEnabledReload;
		FTimerDelegate PlayerEnabledReloadDelegate;
		PlayerEnabledReloadDelegate = FTimerDelegate::CreateUObject(
			this,
			&UBTTaskNode_WaitwithAnimation::Timer
		);
		GetWorld()->GetTimerManager().SetTimer(PlayerEnabledReload, PlayerEnabledReloadDelegate, 6.0f, true);

		timerinit = true;
	}


	if (!timerfinished)
	{
		return EBTNodeResult::InProgress;
	}
	else {
		timerinit = false;
		timerfinished = false;
		return EBTNodeResult::Failed;
	}
	//APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	//float MaxRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("MaxRadius"));
	//UE_LOG(LogTemp, Warning, TEXT("MaxRadius:%f"), MaxRadius);
	//UE_LOG(LogTemp, Warning, TEXT("Dist:%f"), Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()));
	//if (Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) < MaxRadius)
	//{
	//	//esta a rango, disparo
	//	UE_LOG(LogTemp, Display, TEXT("PASO A DISPARAR"));
	//	return EBTNodeResult::Succeeded;
	//}
	////no esta en rango, paso a moverme
	//UE_LOG(LogTemp, Display, TEXT("PASO A MOVERME"));
	return EBTNodeResult::Failed;
}

void UBTTaskNode_WaitwithAnimation::Timer()
{
	timerfinished = true;
}