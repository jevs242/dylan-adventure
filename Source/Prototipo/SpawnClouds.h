// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnClouds.generated.h"

UCLASS()
class PROTOTIPO_API ASpawnClouds : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnClouds();


private:

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud1;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud2;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud3;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud4;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud5;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud6;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud7;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Cloud8;

	UPROPERTY(EditAnywhere)
		class USceneComponent* BeginLocation;

	UPROPERTY(EditAnywhere)
		class USceneComponent* EndLocation;

	UPROPERTY(EditAnywhere)
		class USceneComponent* MinY;

	UPROPERTY(EditAnywhere)
		class USceneComponent* MaxY;


	UPROPERTY(EditAnywhere)
		int Speed = 250;

	void MoveClouds(float DeltaTime, UStaticMeshComponent* Clouds);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
