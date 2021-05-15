//Jose E Velazquez Sepulveda
//MySaveGame.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 *
 */
UCLASS()
class PROTOTIPO_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation;

	UPROPERTY(EditAnywhere)
		int Gems;

	UPROPERTY(EditAnywhere)
		int IslandNumber;
};
