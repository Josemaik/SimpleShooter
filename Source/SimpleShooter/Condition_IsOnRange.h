// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Condition_IsOnRange.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UCondition_IsOnRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UCondition_IsOnRange();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
