// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ActiveShell.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTaskNode_ActiveShell::UBTTaskNode_ActiveShell()
{
	NodeName = TEXT("ActiveShell");
}

EBTNodeResult::Type UBTTaskNode_ActiveShell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    //get pawn controled by boss
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    if (ControlledPawn != nullptr)
    {
        ControlledPawn->Tags.Add(TEXT("Shell"));
    }

    AShooterCharacter* character = Cast<AShooterCharacter>(ControlledPawn);
    if (character != nullptr)
    {
        character->ActiveShell();
    }

    return EBTNodeResult::Failed;
}