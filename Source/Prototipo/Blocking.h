// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrototipoCharacter.h"
#include "Blocking.generated.h"

UCLASS()
class PROTOTIPO_API ABlocking : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlocking();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* PMesh;

	class APrototipoCharacter* Character;

	UPROPERTY(EditAnywhere)
		int Waves;

	void vWaves(int iWaves);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
