// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Mover.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass);
	/*if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}*/
}
void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	UE_LOG(LogTemp, Warning, TEXT("We have finished"));

	HUD->RemoveFromParent();
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
		//lanzar la cinemática.
	}
	else
	{
		LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
		DisableInput(this);
		GetWorldTimerManager().SetTimer(RestartTimer, this, &AShooterPlayerController::Respawn,RestartDelay);
	}
}

void AShooterPlayerController::Respawn()
{
	UE_LOG(LogTemp, Display, TEXT("Respawneando"));
	LoseScreen->RemoveFromParent();
	AShooterCharacter* ShooterPlayer = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (ShooterPlayer)
	{
		UE_LOG(LogTemp, Display, TEXT("Sigo vivo"));
		ShooterPlayer->SetHealth(100);
		ShooterPlayer->RespawnCheckpoint();
		EnableInput(this);
		HUD->AddToViewport();

		TArray<AActor*>FindActor;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Elevator"), FindActor);
		UE_LOG(LogTemp, Display, TEXT("FindActor: %i"),FindActor.Num());
		if (FindActor.Num() > 0 && FindActor[0]) {
			FindActor[0]->SetActorLocation(FVector{2818.827081,-10930.001953,-0.000009});
		
			UMover* MoverComponent = FindActor[0]->FindComponentByClass<UMover>();
			if (MoverComponent)
			{
				MoverComponent->SetShouldMove(false);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("No sigo vivo"));
	}
	/*ShooterPlayer->RespawnCheckpoint();*/
}