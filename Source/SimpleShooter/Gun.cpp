// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(Root);

	MeleeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Collision"));
	MeleeCollision->SetupAttachment(Root);

	MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActiveMelee && !MeleeHit)
	{
		//check collisions with enemy
		UE_LOG(LogTemp, Display, TEXT("MeleeActivooo"));
		TArray<AActor*> OverlappingActors;
		MeleeCollision->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			if (Actor && (Actor->ActorHasTag("Enemy") || Actor->ActorHasTag("Boss")))  // Suponiendo que "AEnemyClass" es la clase de tus enemigos
			{
				//impact sound
				UGameplayStatics::SpawnSoundAttached(MeleeImpactSound, Mesh, TEXT("MuzzleFlashSocket"));

				MeleeHit = true;

				UE_LOG(LogTemp, Display, TEXT("Overlapping with: %s"), *Actor->GetName());
				// Lógica para aplicar daño o interacción al enemigo
				 // Crear un evento de daño simple
				FDamageEvent DamageEvent;
				AController* OwnerController = GetOwnerController();  // Asegúrate de implementar esta función correctamente

				// Aplicar 100 de daño al actor que está overlapeando
				Actor->TakeDamage(100.0f, DamageEvent, OwnerController, this);

				UE_LOG(LogTemp, Display, TEXT("MeleeePegado"));
				DrawDamageTaken(100.0f);

				break;
			}
		}
	}

}
void AGun::DestroySphereCollision()
{
	if (SphereCollision != nullptr)
	{
		SphereCollision->DestroyComponent();
	}
}
void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo: %i"), CurrentAmmo);
	
	if (CurrentAmmo != 0)
	{

		//spawn shot effect
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

		FHitResult Hit;
		FVector ShotDirection;
		bool bSuccess = GunTrace(Hit, ShotDirection);
		if (bSuccess)
		{
			UE_LOG(LogTemp, Display, TEXT("Sucess"));
			//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactSound,Hit.Location);
			//dealing damage
			AActor* HitActor = Hit.GetActor();
			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				if ((HitActor->ActorHasTag("Enemy") || HitActor->ActorHasTag("Boss")) 
					&& !Hit.GetActor()->ActorHasTag("Dead"))
				{
					DrawDamageTaken(Damage);
				}
			}
		}

		//Decrease Ammo
		DecreaseAmmo();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Sonido vacio"));
		UGameplayStatics::SpawnSoundAttached(EmptyGunSound, Mesh, TEXT("MuzzleFlashSocket"));
	}
}
void AGun::ActiveMeleeCollision(bool mode)
{ 
	bActiveMelee = mode; 
	if (bActiveMelee)
	{
		MeleeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		MeleeHit = false;
		MeleeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AGun::IsAimingEnemy()
{
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		if ((Hit.GetActor()->ActorHasTag("Enemy") || Hit.GetActor()->ActorHasTag("Boss")) 
			&& !Hit.GetActor()->ActorHasTag("Dead")) {
			return true;
		}
		else return false;
	}
	return false;
}
bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	//draw camera on player direction
	//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);
	//DrawDebugPoint(GetWorld(), Location, 20, FColor::Red, true);

	//return line trace result
	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	//evoid hit gun and himself
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

int AGun::GetCurrentAmmo() const
{
	if (CurrentAmmo != 0)
	{
		return CurrentAmmo;
	}
	return 0;
}

int AGun::GetMaxAmmo() const
{
	if (MaxAmmo != 0)
	{
		return MaxAmmo;
	}
	return 0;
}

void AGun::ReloadAmmo()
{
	int AmmotobeReloaded = MaxAmmo - CurrentAmmo;
	if(ReservedAmmo - AmmotobeReloaded < 0)
	{
		AmmotobeReloaded = ReservedAmmo;
		ReservedAmmo = 0;
	}
	else {
		ReservedAmmo -= AmmotobeReloaded;
	}
	
	/*if (ReservedAmmo >= MaxAmmo)
	{*/
	CurrentAmmo += AmmotobeReloaded;
	/*}
	else {
		CurrentAmmo += ReservedAmmo;
	}*/
}