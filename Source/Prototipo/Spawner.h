// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "PrototipoCharacter.h"
#include "Spawner.generated.h"

UCLASS()
class PROTOTIPO_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* OutBoxRespawn;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapEnd2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
		class USceneComponent* Spawn1;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
		class USceneComponent* Spawn2;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
		class USceneComponent* Spawn3;
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly)
		class USceneComponent* Spawn4;
	
	UPROPERTY(EditAnywhere ,BlueprintReadOnly)
		class USceneComponent* Spawn5;

	UFUNCTION(BlueprintCallable)
		void Spawn();

	void FSpawn(USceneComponent* SpawnScene);

	UPROPERTY(EditAnywhere)
		int AmountEnemy = 0;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemy> SpawnObject;

	UFUNCTION(BlueprintPure)
		bool vBattle() const;

	UFUNCTION(BlueprintPure)
		bool vDeath() const;

	UFUNCTION(BlueprintPure)
		bool vRevive() const;

	class APrototipoCharacter* CharacterD;

	bool Battle = false;

	bool bRevive = false;

	bool OutBox = false;

	UPROPERTY(EditAnywhere)
		int Waves = 0;

	UPROPERTY(EditAnywhere)
		int NumberSpawn = 0;

	UFUNCTION(BlueprintPure)
	int vNumberSpawn();

	UFUNCTION(BlueprintPure)
		int vNumberSpawnPast();
	
	UFUNCTION(BlueprintPure)
		int vNumberSpawnCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
