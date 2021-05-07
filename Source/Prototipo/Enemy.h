#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototipoCharacter.h"
#include "Sword.h"
#include "Gem.h"
#include "Enemy.generated.h"

UCLASS(config = Game)

class AEnemy : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	void Attack();

	void NotAttack();

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
		void EnemyHeal();

	UFUNCTION(BlueprintPure)
		bool GetDead() const;
	
	UPROPERTY(EditAnywhere)
		float MaxHealth = 100;

	UPROPERTY(EditAnywhere)
		float Damage = 20;

	float Health = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool AttackActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDeath = false;
	

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);



protected:

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	class APrototipoCharacter* Character;

	FRotator RotationCharacter;

	void RotatorEnemy(FVector LookAtTarget);

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASword> SwordClass;

	UPROPERTY(EditAnywhere)
		class USceneComponent* DropObject;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AGem> SpawnObject;


	ASword* Sword;

	void Death();

	void DestroyEnemy();

	FTimerHandle FDeath;

	bool See = false;

	bool Ok = false;

};