// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EnumTriggerType : uint8 {
	Option1 UMETA(DisplayName = "Overlap"),
	Option2 UMETA(DisplayName = "PickShotGun"),
	Option3 UMETA(DisplayName = "PassWord")
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIMPLESHOOTER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UTriggerComponent();

protected:
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* Mover);

private:

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UMover* Mover;

	AActor* GetAcceptableActor() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EnumTriggerType TriggerType;

	UPROPERTY(EditAnywhere)
	USoundBase* OpenSound;
};
