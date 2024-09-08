// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

//fordward declaring
class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure) // no tiene pin de ejecución, solo tiene impacto cuando se requiere su resultado
	bool IsDead() const;

	UFUNCTION(BlueprintPure) 
	bool IsReloading() const;

	UFUNCTION(BlueprintPure)
	bool IsSwitching() const;

	UFUNCTION(BlueprintPure)
	bool IsPicking() const { return IsPickinggun; }

	UFUNCTION(BlueprintPure)
	bool IsInteractingWithDoor() const { return interactdoor; }

	UFUNCTION(BlueprintCallable)
	void SetInteractDoor(bool value) { interactdoor = value; }

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetHealth() const { return Health; }


	UFUNCTION(BlueprintPure)
	AGun* GetCurrentGun();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchCam();

	UFUNCTION(BlueprintCallable)
	void HabilitarInput();

	bool IsPasswordCorrect() const { return passwordintroduced; }

	UFUNCTION(BlueprintCallable)
	void SetPassword(bool value); 

	TArray<FVector> GetPath() const { return Path; }

	/*UFUNCTION(BlueprintCallable)
	AActor* GetPath() const { if (Path) return Path; return nullptr; }*/

	/*UFUNCTION(BlueprintImplementableEvent)
	void StartMoving();*/
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//take damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void Shoot();
	void Reload();
	void AddAmmo(class AGun* gun);
private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	void Interact();
	void PickUpGun(class AGun* gun);
	void ManagePickGun(AGun* gun);
	void SwithGun();
	void ManageSwithGun();

	UPROPERTY(EditAnywhere)
	float Rotationrate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* PrimaryGun;

	UPROPERTY()
	AGun* SecondaryGun;

	bool isreloading;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsSwitchinhgun;

	bool IsPickinggun;

	bool interactdoor;

	bool passwordintroduced;

	//Panel de password
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PassWordPanelClass;

	UUserWidget* PassWordPanel;

	/*UPROPERTY(EditAnywhere)
	FName Path_name;

	UPROPERTY(EditAnywhere)
	AActor* Path;*/

	UPROPERTY(EditAnywhere)
	TArray<FVector> Path;
};
