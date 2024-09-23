// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Reload.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "Gun.h"

UBTTaskNode_Reload::UBTTaskNode_Reload()
{
	NodeName = TEXT("Reload");
}

EBTNodeResult::Type UBTTaskNode_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ShooterCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ShooterCharacter->GetCurrentGun()->ReloadAmmo();

	return EBTNodeResult::Succeeded;
}