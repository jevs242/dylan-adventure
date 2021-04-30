// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("%s"), PlayerPawn.ToString());
	//UE_LOG(LogTemp, Error, TEXT("%s Es el Pawn"), *PlayerPawn->GetName());

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

	}


}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
