// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
//#include "RotatingMovementComponent.generated.h"
#include "SimpleShooterGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "Path.h"
// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);							
	PickUpGun(GetWorld()->SpawnActor<AGun>(GunClass));

	//path
	/*if (!Path_name.ToString().IsEmpty())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			if (Actor->ActorHasTag(Path_name))
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor encontrado"));
				Path = Actor;
				break;
			}
		}
	}*/
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

bool AShooterCharacter::IsReloading() const
{
	return isreloading;
}

bool AShooterCharacter::IsSwitching() const
{
	return IsSwitchinhgun;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis(TEXT("MoveFordward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	//Actions
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shooting"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reloading"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("SwitchGun"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwithGun);
	PlayerInputComponent->BindAction(TEXT("SwitchCamera"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchCam);
}

//pc input mappings
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

//Controller input mappings
void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * Rotationrate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * Rotationrate * GetWorld()->GetDeltaSeconds());
}
AGun* AShooterCharacter::GetCurrentGun()
{
	if (PrimaryGun != nullptr && PrimaryGun->ActorHasTag("Active"))
	{
		return PrimaryGun;
	}
	return SecondaryGun;
}

void AShooterCharacter::Shoot()
{
	if (PrimaryGun->ActorHasTag("Active"))
	{
		UE_LOG(LogTemp, Warning, TEXT("rifle shooting"));
		PrimaryGun->PullTrigger();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("EScopeta shooting"));
		SecondaryGun->PullTrigger();
	}
}

void AShooterCharacter::Reload()
{
	
	FTimerHandle PlayerEnabledReload;
	FTimerDelegate PlayerEnabledReloadDelegate;
	bool canbecharged = false;
	if (PrimaryGun->ActorHasTag("Active"))
	{
		if (PrimaryGun->CanbeCharged())
		{
			canbecharged = true;
			PlayerEnabledReloadDelegate = FTimerDelegate::CreateUObject(
				this,
				&AShooterCharacter::AddAmmo,
				PrimaryGun
			);
		}
	}
	else {
		if (SecondaryGun->CanbeCharged())
		{
			canbecharged = true;
			PlayerEnabledReloadDelegate = FTimerDelegate::CreateUObject(
				this,
				&AShooterCharacter::AddAmmo,
				SecondaryGun
			);
		}
	}

	if (canbecharged)
	{
		GetWorldTimerManager().SetTimer(PlayerEnabledReload, PlayerEnabledReloadDelegate, 3, false);	
		isreloading = true;
	}
}
void AShooterCharacter::AddAmmo(AGun* gun)
{
	isreloading = false;
	gun->ReloadAmmo();
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamagetoApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamagetoApply = FMath::Min(Health, DamagetoApply);
	Health -= DamagetoApply;
	UE_LOG(LogTemp, Display, TEXT("Health left: %f"), Health);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
	}
	return DamagetoApply;
}
void AShooterCharacter::ManagePickGun(AGun* gun)
{
	IsPickinggun = false;

}
void AShooterCharacter::PickUpGun(AGun* gun)
{
	if (PrimaryGun != nullptr)
	{
		IsPickinggun = true;

		//
		//La primaria pasa a la Secundaria
		PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("secondaryGunSocket"));
		PrimaryGun->Tags.Remove("Active");

		//La secundaria pasa a ser la primaria
		SecondaryGun = gun;
		SecondaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		SecondaryGun->Tags.Add("Secondary");
		SecondaryGun->SetOwner(this);
		SecondaryGun->DestroySphereCollision();
		//add maxAmmo
		SecondaryGun->SetMaxAmmo(6);
		SecondaryGun->SetReservedAmmo(45);
		//
		FTimerHandle PlayerEnabledSwitch;
		FTimerDelegate PlayerEnabledSwitchDelegate;
		PlayerEnabledSwitchDelegate = FTimerDelegate::CreateUObject(
			this,
			&AShooterCharacter::ManagePickGun,
			gun
		);
		GetWorldTimerManager().SetTimer(PlayerEnabledSwitch, PlayerEnabledSwitchDelegate, 3.3, false);
	}
	else {
		//first time you get weapon
		PrimaryGun = gun;
		PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		PrimaryGun->DestroySphereCollision();
		PrimaryGun->SetOwner(this);
		PrimaryGun->Tags.Add("Active");
		PrimaryGun->Tags.Add("Primary");
	}
}
void AShooterCharacter::Interact()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	if (Actors.Num() > 0)
	{
		FString Name = Actors[0]->GetActorNameOrLabel();
		AActor* InteractableActor = Actors[0];
		//ShotGun
		if (InteractableActor->ActorHasTag("ShotGun"))
		{
			AGun* ShotGun = Cast<AGun>(InteractableActor);
			
			PickUpGun(ShotGun);
		}
		if (InteractableActor->ActorHasTag("PassDoor"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interactuo con Puerta"));
			interactdoor = true;
			PassWordPanel = CreateWidget(GetWorld(), PassWordPanelClass);
			if (PassWordPanel != nullptr)
			{
				//A�ado el panel widget al viewport
				PassWordPanel->AddToViewport();
				// Deshabilito Input
				APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				PlayerPawn->DisableInput(PlayerController);
				//Cambio c�mara a primera persona
				//SwitchCam();
			}
		}
		
		//Another
	}

	/*for (AActor* actor : Actors)
	{

	}*/
}
void AShooterCharacter::HabilitarInput()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
}

void AShooterCharacter::SetPassword(bool value)
{
	passwordintroduced = value;
}
void AShooterCharacter::ManageSwithGun()
{
	IsSwitchinhgun = false;
	if (PrimaryGun->ActorHasTag("Active"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle a Escopeta"));
		PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("secondaryGunSocket"));
		SecondaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		//Gun -> remove tag
		PrimaryGun->Tags.Remove("Active");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Escopeta a Rifle"));
		PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		SecondaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("secondaryGunSocket"));
		//Gun -> add tag
		PrimaryGun->Tags.Add("Active");
	}
}
void AShooterCharacter::SwithGun()
{
	if (PrimaryGun && SecondaryGun)
	{
		FTimerHandle PlayerEnabledSwitch;
		FTimerDelegate PlayerEnabledSwitchDelegate;
		PlayerEnabledSwitchDelegate = FTimerDelegate::CreateUObject(
			this,
			&AShooterCharacter::ManageSwithGun
		);
		GetWorldTimerManager().SetTimer(PlayerEnabledSwitch, PlayerEnabledSwitchDelegate, 3.3, false);
		IsSwitchinhgun = true;
	}
}