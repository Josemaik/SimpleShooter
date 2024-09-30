// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PawnKilled(APawn* PawnKilled) override;

	UPROPERTY(BlueprintReadOnly)
	bool AllEnemiesDead = false;

	int getCurrentMission() const { return currentmission; }
	UPROPERTY(BlueprintReadWrite)
	int currentmission;

private:

	UFUNCTION(BlueprintCallable)
	void EndGame(bool bisPlayerWinner);
};
