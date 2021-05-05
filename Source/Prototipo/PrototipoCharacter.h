// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototipoCharacter.generated.h"

class ASword;

UCLASS(config = Game)
class APrototipoCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the cameraa behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	APrototipoCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollision;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:
	void Tick(float DeltaSeconds) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	virtual void BeginPlay() override;


protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:

	//Vida

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;

	UPROPERTY(VisibleAnywhere)
		float Health;

	bool bHeal = false;

	bool Healok = false;
	
	UFUNCTION(BlueprintPure)
		bool AttackAnim() const;
	//Resistencia

	UFUNCTION(BlueprintPure)
		float GetResistencePercent() const;

	UFUNCTION(BlueprintPure)
		bool Death() const;

	//Guardar y Cargar Partida

	UFUNCTION(BlueprintCallable)
		void SaveGame(int ISlot, FString FSlot);

	UFUNCTION(BlueprintCallable)
		void LoadGame(int ISlot, FString FSlot);

	UFUNCTION(BlueprintPure)
		bool bSave() const;

	//Germ

	UFUNCTION(BlueprintPure)
		int GetGems() const;

	//Jump

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bJump = false;

private:

	//Resistence

	void vResistence(float DeltaSeconds);

	void Run();
	void NotRun();

	bool bResistence = false, bRStay = true, bLStay = true;

	UPROPERTY(EditAnywhere)
		float MaxResistence = 100, Resistence = 0;

	UPROPERTY(EditAnywhere)
		int Down = 20;

	UPROPERTY(EditAnywhere)
		int Up = 50;

	UPROPERTY(EditAnywhere)
		int DownAttack = 100;

	UPROPERTY(EditAnywhere)
		int SpeedRun = 1200;


	class UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();

	//Vida

	void vHeal(float DeltaSeconds);

	void vHealTime();

	UPROPERTY(EditAnywhere)
		float HealTime = 2.5f;

	
	UPROPERTY(EditAnywhere)
		float MaxHealth = 100;

	//Espada

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASword> SwordClass;

	UPROPERTY()
		ASword* Sword;

	//Shield

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Shield;

	UPROPERTY(EditAnywhere)
		class USceneComponent* SLocation;

public:

	UPROPERTY(EditAnywhere)
		float Damage = 20;

	//Location

	FVector UltimateLocation;

	void UpdateUltimateLocation(); 


	int Gems = 0;

	void Attack();

	void Defence();

	void Defenceoff();

	void AttackActive();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bAttack = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bRevive = false;
	
	FTimerHandle FAttack;
	FTimerHandle FHeal;
	FTimerHandle FDefence;
	FTimerHandle ULocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* M_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* M_Attack2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDefence = false;

	UPROPERTY(EditAnywhere)
		float TimeAttack = 1.0;

	int rnum = 0;

	int anum = 1;

	bool bAttackActive = false;

	bool AttackResistence = false;
	
	bool bDeath = false;

	void vDeath();

	UPROPERTY(EditAnywhere)
		FVector CoordinatesIsland[3];

	int IslandNumber = 0;

	bool  Battle = false;
};

