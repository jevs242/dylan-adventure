// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "AIController.h"
#include "Enemy.h"
#include "PrototipoCharacter.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->Attack();


	return EBTNodeResult::Succeeded;
}
