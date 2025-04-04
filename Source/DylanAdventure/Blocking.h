//Jose E Velazquez Sepulveda
//Blocking.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DylanAdventureCharacter.h"
#include "Blocking.generated.h"

UCLASS()
class DYLANADVENTURE_API ABlocking : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlocking();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* PMesh;

	UPROPERTY(EditAnywhere)
		int Waves;
	
	class ADylanAdventureCharacter* Character;

	void vWaves(int iWaves);
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
