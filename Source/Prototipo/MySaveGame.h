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

	UPROPERTY(EditAnywhere)
		FString	TextMission;

	UPROPERTY(EditAnywhere)
		bool WavesComplete1;

	UPROPERTY(EditAnywhere)
		bool WavesComplete2;

	UPROPERTY(EditAnywhere)
		bool WavesComplete3;

	UPROPERTY(EditAnywhere)
		bool WavesComplete4;

	UPROPERTY(EditAnywhere)
		bool WavesComplete5;
		
};
