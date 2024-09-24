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
	bool IsDead() const;
	void SetShootingMove(bool value);
protected:
	virtual void BeginPlay() override;
private:
	//player
	//APawn* PlayePawn;
	class AShooterCharacter* PlayerPawn;
	//detection radius
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 1000;
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehavior;
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehavior_boss;
};
