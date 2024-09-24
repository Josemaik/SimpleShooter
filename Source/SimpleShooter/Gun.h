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
	bool IsAimingEnemy();

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

	UFUNCTION(BlueprintPure)
	int GetReservedAmmo() const { return ReservedAmmo; }

	UFUNCTION(BlueprintPure)
	bool GetIsAmingEnemy() const { return bIsAimingEnemy; }

	UFUNCTION(BlueprintImplementableEvent)
	void DrawDamageTaken(float damage);


	void DestroySphereCollision(); 

	bool CanbeCharged() const { return GetReservedAmmo() > 0 && GetCurrentAmmo() < GetMaxAmmo(); }

	void SetMaxAmmo(float maxamo) { MaxAmmo = CurrentAmmo = maxamo; }

	void SetReservedAmmo(float newreservedammo) { ReservedAmmo = newreservedammo; }

	void SetAiming(bool newIsAimingEnemy) { bIsAimingEnemy = newIsAimingEnemy; }

	void ActiveMeleeCollision(bool mode); 
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* MeleeCollision;

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
	USoundBase* MeleeImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int MaxAmmo = 15;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int CurrentAmmo = 15;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int ReservedAmmo = 120;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	UUserWidget* HUD;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;	

	bool bIsAimingEnemy;

	bool bActiveMelee = false;
	bool MeleeHit = false;

};
