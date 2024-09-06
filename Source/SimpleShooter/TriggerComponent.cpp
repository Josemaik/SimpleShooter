// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mover == nullptr)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Moverse"));

	switch (TriggerType)
	{
	case EnumTriggerType::Option1:
	{
		AActor* Actor = GetAcceptableActor();
		if (Actor != nullptr)
		{
			Mover->SetShouldMove(true);
		}
	}
		break;
	case EnumTriggerType::Option2: {
		AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
		if (Player->IsPicking())
		{
			Mover->SetShouldMove(true);
		}
	}
		break;
	case EnumTriggerType::Option3:
	{
		AShooterCharacter* Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (Player->IsPasswordCorrect())
		{
			Mover->SetShouldMove(true);
		}
	}
		break;
	default:
		break;
	}
	/*else {
		UE_LOG(LogTemp, Warning, TEXT("No me muevo"));
		Mover->SetShouldMove(false);
	}*/
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	AActor* ReturnActor = nullptr;
	TArray<AActor*> Actors;

	GetOverlappingActors(Actors);
	//UE_LOG(LogTemp, Display, TEXT("Tag: %s"),*AcceptableActorTag.ToString());
	for (AActor* Actor : Actors)
	{
		bool hasAceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		if (hasAceptableTag)
		{
			return Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}