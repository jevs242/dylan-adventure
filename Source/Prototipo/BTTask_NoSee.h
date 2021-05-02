// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NoSee.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTIPO_API UBTTask_NoSee : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_NoSee();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
