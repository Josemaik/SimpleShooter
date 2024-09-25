// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	UE_LOG(LogTemp, Warning, TEXT("A pawn was killed"));

	//Add dead tag if is enemy
	if(PawnKilled->ActorHasTag("Enemy") || PawnKilled->ActorHasTag("Boss"))
		PawnKilled->Tags.Add(TEXT("Dead"));

	//if Player is died then lose the game
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		/*PlayerController->GameHasEnded(nullptr, false);*/
		EndGame(false);
	}
	//check if all AI shooters are dead, if not early return(the game continue)
	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead())
		{
			return;
		}
	}
	//if all AIShooters are dead then Player win
	AllEnemiesDead = true;
	/*AShooterCharacter* ShooterPlayer = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (ShooterPlayer)
	{
		UE_LOG(LogTemp, Display, TEXT("Nueva mision"));
		ShooterPlayer->NewMision(TEXT("3"));
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("No Nueva mision"));
	}*/
	/*if (YouWin)
	{
		UE_LOG(LogTemp, Display, TEXT("You Win"));
		EndGame(true);
	}	*/
}


void AKillEmAllGameMode::EndGame(bool bisPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bisWinner = Controller->IsPlayerController() == bisPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bisWinner);
		//if (bisPlayerController)
		//{
		//	//Player won
		//	Controller->GameHasEnded(nullptr, bisPlayerController);
		//}
		//else {
		//	//AI won
		//	Controller->GameHasEnded(nullptr, !bisPlayerController);
		//}
	}
}