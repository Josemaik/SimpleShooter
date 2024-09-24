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
#include "Components/SphereComponent.h"
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

	//tecla siendo mantenida
	if (bisAiming)
	{
		if (PrimaryGun->ActorHasTag("Active"))
		{
			if (PrimaryGun->IsAimingEnemy())
			{
				UE_LOG(LogTemp, Warning, TEXT("Estoy apuntando enemigo"));
				PrimaryGun->SetAiming(true);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Noooo Estoy apuntando enemigo"));
				PrimaryGun->SetAiming(false);
			}

		}
		else {
			if (SecondaryGun->IsAimingEnemy())
			{
				SecondaryGun->SetAiming(true);
			}
			else {
				SecondaryGun->SetAiming(false);
			}
		}
	}
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
	PlayerInputComponent->BindAction(TEXT("Heal"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Heal);
	PlayerInputComponent->BindAction(TEXT("Crounch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Crounch);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Aiming);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &AShooterCharacter::StopAiming);
	PlayerInputComponent->BindAction(TEXT("Melee"), EInputEvent::IE_Pressed, this, &AShooterCharacter::AttackMelee);
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
void AShooterCharacter::Crounch()
{
	UE_LOG(LogTemp, Display, TEXT("Me agacho"));
	crounch = !crounch;
}

AGun* AShooterCharacter::GetCurrentGun()
{
	if (PrimaryGun != nullptr && PrimaryGun->ActorHasTag("Active"))
	{
		return PrimaryGun;
	}
	return SecondaryGun;
}
void AShooterCharacter::StopAiming()
{
	bisAiming = false;
	if (PrimaryGun->ActorHasTag("Active"))
	{
		if (PrimaryGun->IsAimingEnemy())
		{
			UE_LOG(LogTemp, Warning, TEXT("Estoy apuntando enemigo"));
			PrimaryGun->SetAiming(true);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Noooo Estoy apuntando enemigo"));
			PrimaryGun->SetAiming(false);
		}

	}
	else {
		if (SecondaryGun->IsAimingEnemy())
		{
			SecondaryGun->SetAiming(true);
		}
		else {
			SecondaryGun->SetAiming(false);
		}
	}
}
void AShooterCharacter::Aiming()
{
	//Sound Aim
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AimingSound, GetActorLocation());
	bisAiming = true;
	if (PrimaryGun->ActorHasTag("Active"))
	{
		if (PrimaryGun->IsAimingEnemy())
		{
			UE_LOG(LogTemp, Warning, TEXT("Estoy apuntando enemigo"));
			PrimaryGun->SetAiming(true);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Noooo Estoy apuntando enemigo"));
			PrimaryGun->SetAiming(false);
		}

	}
	else {
		if (SecondaryGun->IsAimingEnemy())
		{
			SecondaryGun->SetAiming(true);
		}
		else {
			SecondaryGun->SetAiming(false);
		}
	}
}
void AShooterCharacter::Heal()
{
	//Heal Sound
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HealingSound, GetActorLocation());
	// Vida que se va a curar en total
	if (currentpotion < MaxCurePotions && Health < MaxHealth)
	{
		healing = true;

		TotalLifeToCure = 30;

		DeleteCurePotion(currentpotion);
		currentpotion++;

		// Configura un temporizador para curar cada 0.5 segundos
		FTimerDelegate PlayerHealDelegate = FTimerDelegate::CreateUObject(
			this,
			&AShooterCharacter::HealStep
		);

		GetWorldTimerManager().SetTimer(PlayerHealTimerHandle, PlayerHealDelegate, 0.5f, true);
	}
}

void AShooterCharacter::HealStep()
{
	// Define cuánta vida vas a curar por tick
	int32 LifePerTick = 1;

	// Si el jugador está completamente curado o ya curamos todo el LifeToCure
	if (Health >= 100 || TotalLifeToCure <= 0)
	{
		// Limpiar el temporizador cuando termine la curación
		GetWorldTimerManager().ClearTimer(PlayerHealTimerHandle);
		healing = false;
		return;
	}

	// Curar al jugador
	AddLife();

	// Restar la cantidad curada de la cantidad total a curar
	TotalLifeToCure -= LifePerTick;
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

void AShooterCharacter::AttackMelee()
{
	melee = true;
	UE_LOG(LogTemp, Display, TEXT("Melee"));

	if (currentgun != nullptr)
	{
		currentgun->ActiveMeleeCollision(true);
		FTimerHandle MeleeHandle;
		GetWorldTimerManager().SetTimer(MeleeHandle, this, &AShooterCharacter::EndAttackMelee, 1.2);
	}
}
void AShooterCharacter::EndAttackMelee()
{
	UE_LOG(LogTemp, Display, TEXT("End Melee"));
	if (currentgun != nullptr)
	{
		currentgun->ActiveMeleeCollision(false);
	}
	melee = false;

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
	//If has shell no get damage
	float DamagetoApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (!ActorHasTag(TEXT("Shell")))
	{
		DamagetoApply = FMath::Min(Health, DamagetoApply);
		Health -= DamagetoApply;
		UE_LOG(LogTemp, Display, TEXT("Health left: %f"), Health);
		Takedamage();
	}

	if (IsDead() && !ActorHasTag("Dead"))
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			//Dead Sound
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeadSound, GetActorLocation());
			GameMode->PawnKilled(this);
		}
		if (!this->ActorHasTag("Player"))
		{
			//AI
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			DetachFromControllerPendingDestroy();
		}

		//if (PrimaryGun)
		//{
			/*PrimaryGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PrimaryGun->*/
		//	USphereComponent* PhysicsSphere = NewObject<USphereComponent>(PrimaryGun);
		//	if (PhysicsSphere)
		//	{
		//		// Adjuntar el SphereComponent al PrimaryGun como RootComponent o al mesh si es necesario
		//		PhysicsSphere->AttachToComponent(PrimaryGun->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

		//		// Configurar el tamaño del SphereComponent (radio)
		//		PhysicsSphere->SetSphereRadius(30.0f);  // Ajusta el tamaño del radio según lo que necesites

		//		// Habilitar la colisión para física
		//		PhysicsSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//		// Configurar el tipo de colisión (por ejemplo, bloquear todo)
		//		PhysicsSphere->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		//		PhysicsSphere->SetCollisionResponseToAllChannels(ECR_Block);  // Bloquea colisiones con todo

		//		// Agregar el componente al sistema de actores
		//		PhysicsSphere->RegisterComponent();

		//		// Habilitar la simulación de físicas
		//		PhysicsSphere->SetSimulatePhysics(true);
		//	}
		//}
		//if(SecondaryGun)
		//{
		//	SecondaryGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//}

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

		currentgun = SecondaryGun;
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
		currentgun = PrimaryGun;
	}
}

void AShooterCharacter::SetHiddenGun() {
	PrimaryGun->SetHidden(true);
}
void AShooterCharacter::SetVisibleGun() {
	PrimaryGun->SetHidden(false);
}
void AShooterCharacter::Interact()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	if (Actors.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("at least one actor"));
		FString Name = Actors[0]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("name: %s"),*Name);
		AActor* InteractableActor = Actors[0];
		//ShotGun
		if (InteractableActor->ActorHasTag("ShotGun"))
		{
			UE_LOG(LogTemp, Display, TEXT("Pick gun shot"));
			AGun* ShotGun = Cast<AGun>(InteractableActor);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
			PickUpGun(ShotGun);
		}
		if (InteractableActor->ActorHasTag("PassDoor"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interactuo con Puerta"));
			interactdoor = true;
			PassWordPanel = CreateWidget(GetWorld(), PassWordPanelClass);
			if (PassWordPanel != nullptr)
			{
				//Añado el panel widget al viewport
				PassWordPanel->AddToViewport();
				// Deshabilito Input
				APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				PlayerPawn->DisableInput(PlayerController);
				//Cambio cámara a primera persona
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

		currentgun = SecondaryGun;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Escopeta a Rifle"));
		PrimaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		SecondaryGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("secondaryGunSocket"));
		//Gun -> add tag
		PrimaryGun->Tags.Add("Active");

		currentgun = PrimaryGun;
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

void AShooterCharacter::RespawnCheckpoint()
{
	//FRotator DesiredRotation = FRotator(0.0f, -90.0f, 0.0f);  // Esto hará que el Pawn mire hacia el eje Y positivo.

	// Aplica la rotación deseada al transform del checkpoint
	//CurrentCheckpoint.SetRotation(DesiredRotation.Quaternion());

	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Transform: %s"), *CurrentCheckpoint.ToString());
	SetActorTransform(CurrentCheckpoint);
	
	AController* PlayerController = GetController();
	if (PlayerController)
	{
		PlayerController->SetControlRotation(CurrentCheckpoint.GetRotation().Rotator());  // Cambia la rotación del controlador
	}

	spawning = true;

	FTimerHandle SpawnAnim;
	GetWorldTimerManager().SetTimer(SpawnAnim, this, &AShooterCharacter::EndSpawnAnimation, 5);
}