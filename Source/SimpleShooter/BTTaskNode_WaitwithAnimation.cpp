// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_WaitwithAnimation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterAIController.h"

UBTTaskNode_WaitwithAnimation::UBTTaskNode_WaitwithAnimation()
{
	NodeName = TEXT("Wait_and_Animate");
	timerinit = false;
	timerfinished = false;
}

EBTNodeResult::Type UBTTaskNode_WaitwithAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (!timerinit)
	{
		/*EBTNodeResult::Type hello;
		FTimerHandle PlayerEnabledReload;
		FTimerDelegate PlayerEnabledReloadDelegate;
		PlayerEnabledReloadDelegate = FTimerDelegate::CreateUObject(
			this,
			&UBTTaskNode_WaitwithAnimation::Timer,
			hello
		);*/
		// Configurar el temporizador para dispararse una vez después de 2 segundos
		/*GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBTTaskNode_WaitwithAnimation::Timer(OwnerComp), 2.0f, false);*/
		/*GetWorld()->GetTimerManager().SetTimer(PlayerEnabledReload, PlayerEnabledReloadDelegate, 2.0f, true);*/
		// Pasar OwnerComp como parámetro
		 // Crear un FTimerDelegate para pasar parámetros al Timer
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(
			this,
			&UBTTaskNode_WaitwithAnimation::Timer,
			&OwnerComp // Pasar OwnerComp como puntero
		);

		// Configurar el temporizador para dispararse una vez después de 2 segundos
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.0f, false); // Usar la variable miembro 'TimerHandle'

		UE_LOG(LogTemp, Display, TEXT("Init Timer"));
		timerinit = true;
	}


	if (!timerfinished)
	{
		UE_LOG(LogTemp, Display, TEXT("In Progress"));
		return EBTNodeResult::InProgress;
	}
	else {
		timerinit = false;
		timerfinished = false;
		return EBTNodeResult::Failed;
	}
	//APawn* Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
	//float MaxRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("MaxRadius"));
	//UE_LOG(LogTemp, Warning, TEXT("MaxRadius:%f"), MaxRadius);
	//UE_LOG(LogTemp, Warning, TEXT("Dist:%f"), Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()));
	//if (Player->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) < MaxRadius)
	//{
	//	//esta a rango, disparo
	//	UE_LOG(LogTemp, Display, TEXT("PASO A DISPARAR"));
	//	return EBTNodeResult::Succeeded;
	//}
	////no esta en rango, paso a moverme
	//UE_LOG(LogTemp, Display, TEXT("PASO A MOVERME"));
	return EBTNodeResult::Failed;
}

void UBTTaskNode_WaitwithAnimation::Timer(UBehaviorTreeComponent* OwnerComp)
{
	UE_LOG(LogTemp, Display, TEXT("Finish Timer"));
	timerfinished = true;  // Marcar que el temporizador ha finalizado
	AShooterAIController* aic = Cast<AShooterAIController>(OwnerComp->GetAIOwner());
	aic->SetShootingMove(false);
	// Notificar al Behavior Tree que la tarea ha fallado
	/*UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>();*/
	/*if (OwnerComp)
	{*/
	//OwnerComp->GetBlackboardComponent()->ClearValue(TEXT("HasShootorMoved"));
	//OwnerComp->GetBlackboardComponent()->SetValueAsBool("HasShootorMoved", false);
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	//FinishLatentAbort(*OwnerComp);
	/*}*/
}