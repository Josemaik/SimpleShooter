// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds);

protected:
	virtual void BeginPlay() override;
private:
	//player
	APawn* PlayerPawn;
	//detection radius
	//UPROPERTY(EditAnywhere)
	//float AcceptanceRadius = 200;
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehavior;
};
