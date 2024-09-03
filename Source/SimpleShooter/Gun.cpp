// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
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
	CurrentAmmo = MaxAmmo;
}