// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_patrol.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_patrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_patrol();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	int nextposindex = 0;
};
