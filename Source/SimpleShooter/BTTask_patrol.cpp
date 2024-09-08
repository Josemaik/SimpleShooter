// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_patrol.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_patrol::UBTTask_patrol()
{
	NodeName = TEXT("Patroling");
}

EBTNodeResult::Type UBTTask_patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ShooterCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsPatroling"), true);
	//ajustar speeed
	/*ShooterCharacter->GetCharacterMovement()->MaxWalkSpeed = 100;*/
	//UE_LOG(LogTemp, Display, TEXT("encontrado: %f"), speed);
	//Empieza a recorrer el path
	TArray<FVector> path = ShooterCharacter->GetPath();
	if (path.Num() > 0)
	{
		/*UE_LOG(LogTemp, Display, TEXT("hay pathhhhhhhhhh"));*/
		FVector NextPos;
		/*UE_LOG(LogTemp, Display, TEXT("path lenth: %i"), path.Num());
		UE_LOG(LogTemp, Display, TEXT("indice: %i"), nextposindex);
		bool finded = path.Find(NextPos, nextposindex);
		FString xd = finded ? "true" : "false";
		UE_LOG(LogTemp, Display, TEXT("encontrado: %s"), *xd);*/
		if (path.IsValidIndex(nextposindex))
		{
			NextPos = path[nextposindex];
			//UE_LOG(LogTemp, Display, TEXT("seteo next possssssss"));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("NexPathPosition"), NextPos);
			nextposindex++;
		}
		else {
			//si sales del array, vuelves al principio
			nextposindex = 0;
		}

		return EBTNodeResult::Failed;
	}
	/*ShooterCharacter->move*/
	//Dont shoot when player is far away
	//APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	/*if (Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) > 700.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Está muy alejado"));
		return EBTNodeResult::Failed;
	}
	
	ShooterCharacter->Shoot();*/

	return EBTNodeResult::Failed;
}