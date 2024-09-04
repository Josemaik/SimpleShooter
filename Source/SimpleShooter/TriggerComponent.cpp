// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

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

	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Moverse"));
		Mover->SetShouldMove(true);
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