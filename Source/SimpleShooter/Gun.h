// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	void ReloadAmmo();
	void DecreaseAmmo() { if(CurrentAmmo > 0) --CurrentAmmo; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintPure)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintPure)
	int GetMaxAmmo() const;

	void DestroySphereCollision(); 

	bool CanbeCharged() const { return GetCurrentAmmo() < GetMaxAmmo(); }

	void SetMaxAmmo(float maxamo) { MaxAmmo = CurrentAmmo = maxamo; }
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;


	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	USoundBase* EmptyGunSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int MaxAmmo = 15;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int CurrentAmmo = 15;


	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;	
};
