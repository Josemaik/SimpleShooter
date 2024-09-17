// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_WaitwithAnimation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTaskNode_WaitwithAnimation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_WaitwithAnimation();
	void Timer();
	bool timerfinished = false;
	bool timerinit = false;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
