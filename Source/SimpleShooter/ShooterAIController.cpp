// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gun.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	//initialice player
	//PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerPawn = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//run behavuourTree
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		//set start location
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		//Set velocity
		GetBlackboardComponent()->SetValueAsFloat(TEXT("OriginalVelocity"), 600);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("PatrolVelocity"), 100);
		//sel initial
		GetBlackboardComponent()->SetValueAsVector(TEXT("NexPathPosition"), GetPawn()->GetActorLocation());
		//set max radius
		GetBlackboardComponent()->SetValueAsFloat(TEXT("MaxRadius"), AcceptanceRadius);

		GetBlackboardComponent()->SetValueAsBool(TEXT("true"), true);		//true  //fal  //false //true
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//if (LineOfSightTo(PlayerPawn))
	//{
		//Setting playerlocation and lastknownplayerPosition
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		//GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());

		//----------ai without bt-------------------------
		//AI aim to us
		//SetFocus(PlayerPawn);
		//AI move to us
		//MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//else {
		//clear player location
		//GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));

		//----------ai without bt-------------------------
		//AI stop focus us and stop move
		//ClearFocus(EAIFocusPriority::Gameplay);
		//StopMovement();
	//}
	// 

	// Comprobar si el AI está en modo patrullaje
	bool bIsPatrolling = GetBlackboardComponent()->GetValueAsBool(TEXT("IsPatroling"));
	FString xd = bIsPatrolling ? "true" : "false";
	//UE_LOG(LogTemp, Display, TEXT("Patroling:%s"),*xd);
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetPawn());

	if (ShooterCharacter)
	{
		if (PlayerPawn->IsDead())
		{
			GetBlackboardComponent()->ClearValue(TEXT("IsPlayerDead"));
		}
		else {
			GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerDead"), true);
		}

		//set bullets
		int currentbullets = ShooterCharacter->GetCurrentGun()->GetCurrentAmmo();
		GetBlackboardComponent()->SetValueAsInt(TEXT("Bullets"), currentbullets);

		if (bIsPatrolling)
		{
			//UE_LOG(LogTemp, Display, TEXT("Patroling"));
			// Cambiar la velocidad a la velocidad de patrullaje
			float PatrolSpeed = GetBlackboardComponent()->GetValueAsFloat(TEXT("PatrolVelocity"));
			ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("Moving to shoot"));
			// Cambiar la velocidad a la velocidad original
			float OriginalSpeed = GetBlackboardComponent()->GetValueAsFloat(TEXT("OriginalVelocity"));
			ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
		}
	}
}

void AShooterAIController::SetShootingMove(bool value)
{
	UE_LOG(LogTemp, Display, TEXT("SETEEEEEEEEEEEEEOOOOOOOOOOOOOO"));
	GetBlackboardComponent()->SetValueAsBool(TEXT("ShootMove"), value);
}
bool AShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	return true;
}