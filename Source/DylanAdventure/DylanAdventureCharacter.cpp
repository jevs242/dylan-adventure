//Jose E Velazquez Sepulveda
//DylanAdventureCharacter.cpp

#include "DylanAdventureCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MySaveGame.h"
#include "Sword.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

ADylanAdventureCharacter::ADylanAdventureCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetupAttachment(RootComponent);

	//Shield

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Shield_l"));
}

void ADylanAdventureCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADylanAdventureCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADylanAdventureCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Run

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ADylanAdventureCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ADylanAdventureCharacter::NotRun);

	//Attack

	PlayerInputComponent->BindAction("AttackSword", IE_Pressed, this, &ADylanAdventureCharacter::Attack);

	//Defence

	PlayerInputComponent->BindAction("Defence", IE_Pressed, this, &ADylanAdventureCharacter::Defence);
	PlayerInputComponent->BindAction("Defence", IE_Released, this, &ADylanAdventureCharacter::Defenceoff);
}

void ADylanAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Resistence = MaxResistence;

	if (SwordClass != nullptr)
	{
		Sword = GetWorld()->SpawnActor<ASword>(SwordClass);
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sword_r"));
		Sword->SetOwner(this);
	}
	GetWorld()->GetTimerManager().SetTimer(ULocation, this, &ADylanAdventureCharacter::UpdateUltimateLocation, 1.0f, false);
}

void ADylanAdventureCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	vResistence(DeltaSeconds);
	vHeal(DeltaSeconds);
	bJump = CharacterMovement->IsFalling();
}

/// <summary>
/// UI
/// </summary>
/// <returns></returns>

int ADylanAdventureCharacter::GetGems() const
{
	return Gems;
}

bool ADylanAdventureCharacter::GetDefence() const
{
	return bDefence;
}

float ADylanAdventureCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool ADylanAdventureCharacter::AttackAnim() const
{
	return bAttack;
}

float ADylanAdventureCharacter::GetResistencePercent() const
{
	return Resistence / MaxResistence;
}

bool ADylanAdventureCharacter::Death() const
{
	return bDeath;
}

//////////////////////////////////

/// <summary>
/// Movement
/// </summary>
/// <returns></returns>


void ADylanAdventureCharacter::MoveForward(float Value)
{
	if(!bDeath)
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
			bLStay = false;
		}
		else
		{
			bLStay = true;
		}
	}
}

void ADylanAdventureCharacter::MoveRight(float Value)
{
	if(!bDeath)
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
			bRStay = false;
		}
		else
		{
			bRStay = true;
		}
	}
}


void ADylanAdventureCharacter::Run()
{
	if(!bLStay || !bRStay)
	{
		if(!bAttack)
		{
			CharacterMovement->MaxWalkSpeed = SpeedRun;
			bResistence = true;
		}
	}
}

void ADylanAdventureCharacter::NotRun()
{
	CharacterMovement->MaxWalkSpeed = 600;
	bResistence = false;
}

//////////////////////////////////

/// <summary>
/// Recovery
/// </summary>
/// <returns></returns>

void ADylanAdventureCharacter::vHeal(float DeltaSeconds)
{
	if(!Battle)
	{
		if (Health <= MaxHealth && bHeal)
		{
			Health += Down * DeltaSeconds;
			if(Health >= MaxHealth && bHeal)
			{
				Health = MaxHealth;
			}
		}
		else if(Health <= MaxHealth && !Healok)
		{
			GetWorld()->GetTimerManager().SetTimer(FHeal, this, &ADylanAdventureCharacter::vHealTime, HealTime, false);
			Healok = true;
		}
	}
}

void ADylanAdventureCharacter::vHealTime()
{
	bHeal = true;
}

void ADylanAdventureCharacter::vResistence(float DeltaSeconds)
{
	if (bLStay && bRStay && !bAttack && !bJump)
	{
		NotRun();
	}

	
	if ((bResistence && Resistence > 0 && !bRStay && !bJump) || (bResistence && Resistence > 0 && !bLStay && !bJump))
	{
		Resistence -= Down * DeltaSeconds;
		
	}
	else if (!bResistence && Resistence <= MaxResistence && !bAttack && !bJump)
	{
		Resistence += Up * DeltaSeconds;
	}
	else if (Resistence <= 0)
	{
		NotRun();
	}
	
	if(bAttack)
	{
		if(AttackResistence && Resistence > DownAttack)
		{
			Resistence -= DownAttack;
			AttackResistence = false;
		}
	}
}

/// <summary>
/// Location
/// </summary>
/// <returns></returns>

void ADylanAdventureCharacter::UpdateUltimateLocation()
{
	if (bSave())
	{
		UltimateLocation = GetActorLocation();
	}
	GetWorld()->GetTimerManager().SetTimer(ULocation, this, &ADylanAdventureCharacter::UpdateUltimateLocation, 3.0f, false);
}

void ADylanAdventureCharacter::TeleportDeath()
{
	FOutputDeviceNull ar;
	
	this->CallFunctionByNameWithArguments(TEXT("RevivePlayer"), ar, NULL, true);
	SetActorLocation(CoordinatesIsland[IslandNumber]);
	Health = MaxHealth;
	bRevive = true;
	Battle = false;
}

//////////////////////////////////

/// <summary>
/// Battle
/// </summary>
/// <returns></returns>

void ADylanAdventureCharacter::Attack()
{
	if (M_Attack && !bAttack && !bDefence && !bJump && Resistence > DownAttack)
	{
		if (AttackSound != NULL)
		{
			UGameplayStatics::SpawnSoundAttached(AttackSound, RootComponent, TEXT("SwordSocket"));
		}
		bAttackActive = true;
		rnum = FMath::RandRange(1, 2);

		if (rnum == anum)
		{
			Attack();
		}

		anum = rnum;

		switch (rnum)
		{
		case 1:
			PlayAnimMontage(M_Attack, 1, NAME_None);
			break;
		case 2:
			PlayAnimMontage(M_Attack2, 1, NAME_None);
			break;
		default:
			break;
		}

		bAttack = true;
		NotRun();

		AttackResistence = true;

		GetWorld()->GetTimerManager().SetTimer(FAttack, this, &ADylanAdventureCharacter::AttackActive, TimeAttack, false);
	}
}

void ADylanAdventureCharacter::Defence()
{
	if (!bAttack && !bDefence && !bJump)
	{
		UGameplayStatics::SpawnSoundAttached(ShieldSound, RootComponent, TEXT("SwordSocket"));
		bDefence = true;
		CharacterMovement->MaxWalkSpeed = 300;
		bResistence = false;
	}
}

void ADylanAdventureCharacter::Defenceoff()
{
	if (bDefence)
	{
		bDefence = false;
		CharacterMovement->MaxWalkSpeed = 600;
		AttackActive();
	}
}

void ADylanAdventureCharacter::AttackActive()
{
	bAttackActive = false;
	bAttack = false;
}


void ADylanAdventureCharacter::vDeath()
{
	FOutputDeviceNull ar;
	
	bDeath = true;

	Cancelstreak(Waves);
	this->CallFunctionByNameWithArguments(TEXT("DeathPlayer") , ar , NULL , true);
	GetWorld()->GetTimerManager().SetTimer(FDeath, this, &ADylanAdventureCharacter::TeleportDeath, 4.0f, false);
}

//////////////////////////////////

/// <summary>
/// Mission
/// </summary>
/// <returns></returns>

void ADylanAdventureCharacter::viWavesComplete(int iWaves)
{
	if (NumMissionG == iWaves)
	{
		if (WavesComplete[iWaves] == false)
		{
			WavesComplete[iWaves] = true;
			if (NumMissionG == 2)
			{
				TextMission = "-Go to the seller's house on the right of the island";
				
			}
			else if(NumMissionG == 4)
			{
				TextMission = "-Go to the seller on the back of the island";
				
			}
			else if(NumMissionG == 5)
			{
				TextMission = " ";
				FOutputDeviceNull ar;

				this->CallFunctionByNameWithArguments(TEXT("Final"), ar, NULL, true);
			}
			else
			{
				TextMission = "-Return to the store for the next mission";
			}
			Gems += 5;
		}
	}
	Cancelstreak(iWaves);
}

void ADylanAdventureCharacter::vWavesComplete()
{
	if (Waves >= 1 || Waves <= 6)
	{
		if (EnemyKill >= EnemyWaves)
		{
			viWavesComplete(Waves);
		}
	}
}

void ADylanAdventureCharacter::Cancelstreak(int iWaves)
{
	if (WavesComplete[iWaves] == false)
	{
		EnemyKill = 0;
	}
}

int ADylanAdventureCharacter::vNumberSpawn()
{
	return NumberSpawn;
}

FString ADylanAdventureCharacter::vMissionText()
{
	return TextMission;
}

void ADylanAdventureCharacter::Mission(int NumMission)
{
	NumMissionG = NumMission;
	switch (NumMission)
	{
	case 1:
		if (!WavesComplete[1] && TextMission != "-Kill all 3 enemies in the island 1")
		{
			TextMission = "-Kill all 3 enemies in the island 1";
		}
		break;
	case 2:
		if (WavesComplete[1] && !WavesComplete[2] && TextMission != "-Kill all 5 enemies in the center island 2")
		{
			TextMission = "-Kill all 5 enemies in the center island 2";
			Accept[1] = true;
		}
		break;
	case 3:
		if (WavesComplete[2] && !WavesComplete[3] && TextMission != "-Kill all 3 enemies on the left of island 2")
		{
			TextMission = "-Kill all 3 enemies on the left of island 2";
		}
		break;
	case 4:
		if (WavesComplete[3] && !WavesComplete[4] && TextMission != "You have unlocked the top of the island!\n-Go up kill all 4 enemies on the top of the island.")
		{
			TextMission = "You have unlocked the top of the island!\n-Go up kill all 4 enemies on the top of the island.";
			Accept[3] = true;
		}
		break;
	case 5:
		if (WavesComplete[4] && TextMission != "-Kill the boss on island 3")
		{
			Accept[4] = true;
			TextMission = "-Kill the boss on island 3";
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////

/// <summary>
/// Sound
/// </summary>
/// <returns></returns>

void ADylanAdventureCharacter::vDamageSound()
{
	if (DamageSound != NULL)
	{
		UGameplayStatics::SpawnSoundAttached(DamageSound, RootComponent, TEXT("DamageSound"));
	}
}

//////////////////////////////////

/// <summary>
/// Upgrade
/// </summary>
/// <returns></returns>

void ADylanAdventureCharacter::vUpgrade(int Upgrade , int Cost)
{
	Mensaje1 = " ";
	Mensaje2 = " ";
	Mensaje3 = " ";
	switch (Upgrade)
	{
	case 1:
		if (MaxHealth <= 300)
		{
			if (Gems >= Cost)
			{
				MaxHealth += 25;
				Gems -= Cost;
				Mensaje1 = "Successful upgrade";
			}
			else
			{
				Mensaje1 = "Not enough gems";
			}
		}
		else
		{
			Mensaje1 = "completed update";
		}
		break;
	case 2:
		if (MaxResistence <= 300)
		{
			if (Gems >= Cost)
			{
				MaxResistence += 25;
				Gems -= Cost;
				Mensaje2 = "Successful upgrade";
			}
			else
			{
				Mensaje2 = "Not enough gems";
			}
		}
		else
		{
			Mensaje2 = "completed update";
		}
		break;
	case 3:
		if (Damage <= 75)
		{
			if (Gems >= Cost)
			{
				Damage += 15;
				Gems -= Cost;
				Mensaje3 = "Successful upgrade";
			}
			else
			{
				Mensaje3 = "Not enough gems";
			}
		}
		else
		{
			Mensaje3 = "completed update";
		}
		break;
	default:
		break;
	}
}

void ADylanAdventureCharacter::ClearString()
{
	Mensaje1 = " ";
	Mensaje2 = " ";
	Mensaje3 = " ";
}

FString ADylanAdventureCharacter::vUpgradeText(int Opc)
{
	switch (Opc)
	{
	case 1:
		return Mensaje1;
		break;
	case 2:
		return Mensaje2;
		break;
	case 3:
		return Mensaje3;
		break;
	default:
		return "";
		break;
	}
}

//////////////////////////////////


////////Save////////

void ADylanAdventureCharacter::SaveGame(int ISlot, FString FSlot)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	SaveGameInstance->Gems = Gems;
	SaveGameInstance->IslandNumber = IslandNumber;
	SaveGameInstance->TextMission = "-Return to the store for the next mission";
	SaveGameInstance->WavesComplete1 = WavesComplete[0];
	SaveGameInstance->WavesComplete2 = WavesComplete[1];
	SaveGameInstance->WavesComplete3 = WavesComplete[2];
	SaveGameInstance->WavesComplete4 = WavesComplete[3];
	SaveGameInstance->WavesComplete5 = WavesComplete[4];
	SaveGameInstance->Accept1 = Accept[1];
	SaveGameInstance->Accept2 = Accept[3];
	SaveGameInstance->Accept3 = Accept[4];
	SaveGameInstance->MaxHealth = MaxHealth;
	SaveGameInstance->Damage = Damage;
	SaveGameInstance->MaxResistence = MaxResistence;
	SaveGameInstance->NumberSpawnPast = NumberSpawnPast;
	SaveGameInstance->NumberSpawn = NumberSpawn;
	SaveGameInstance->EnemyWaves = EnemyWaves;
	SaveGameInstance->Waves = Waves;
	SaveGameInstance->EnemyKill = 0;
	SaveGameInstance->Battle = Battle;
	

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, FSlot, ISlot);
}

void ADylanAdventureCharacter::LoadGame(int ISlot, FString FSlot)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if(Cast <UMySaveGame>(UGameplayStatics::LoadGameFromSlot(FSlot, ISlot)) != NULL)
	{

		SaveGameInstance = Cast <UMySaveGame>(UGameplayStatics::LoadGameFromSlot(FSlot, ISlot));

		this->SetActorLocation(SaveGameInstance->PlayerLocation);
		Gems = SaveGameInstance->Gems;
		IslandNumber = SaveGameInstance->IslandNumber;
		TextMission = SaveGameInstance->TextMission;
		WavesComplete[0]= SaveGameInstance->WavesComplete1;
		WavesComplete[1] = SaveGameInstance->WavesComplete2;
		WavesComplete[2] = SaveGameInstance->WavesComplete3;
		WavesComplete[3] = SaveGameInstance->WavesComplete4;
		WavesComplete[4] = SaveGameInstance->WavesComplete5;
		Accept[1] = SaveGameInstance->Accept1;
		Accept[3] = SaveGameInstance->Accept2;
		Accept[4] = SaveGameInstance->Accept3;
		MaxHealth = SaveGameInstance->MaxHealth;
		Damage = SaveGameInstance->Damage;
		MaxResistence = SaveGameInstance->MaxResistence;
		NumberSpawnPast = SaveGameInstance->NumberSpawnPast;
		NumberSpawn = SaveGameInstance->NumberSpawn;
		EnemyWaves = SaveGameInstance->EnemyWaves;
		Waves = SaveGameInstance->Waves;
		EnemyKill = SaveGameInstance->EnemyKill;
		Battle = SaveGameInstance->Battle;
	}
	else
	{
		
	}

}

bool ADylanAdventureCharacter::bSave() const
{

	if (!bDefence && !bAttack && !bJump && !bDeath && !Battle)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool ADylanAdventureCharacter::vJump() const
{
	if (!bDefence && !bAttack && !bJump && !bDeath)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ADylanAdventureCharacter::vRevive() const
{
	return bRevive;
}

//////////////////////
