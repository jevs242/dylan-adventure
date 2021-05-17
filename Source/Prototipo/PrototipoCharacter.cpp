#include "PrototipoCharacter.h"
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

#define print(x) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));
#define log(x) UE_LOG(LogTemp, Error, TEXT(x));
#define string(x) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Here : %s"), (x).ToString());


APrototipoCharacter::APrototipoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetupAttachment(RootComponent);

	Resistence = MaxResistence;

	Health = MaxHealth;

	//Shield

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Shield_l"));

}

void APrototipoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APrototipoCharacter::JumpCheck);

	PlayerInputComponent->BindAxis("MoveForward", this, &APrototipoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APrototipoCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Run

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APrototipoCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APrototipoCharacter::NotRun);

	//Attack

	PlayerInputComponent->BindAction("AttackSword", IE_Pressed, this, &APrototipoCharacter::Attack);

	//Defence

	PlayerInputComponent->BindAction("Defence", IE_Pressed, this, &APrototipoCharacter::Defence);
	PlayerInputComponent->BindAction("Defence", IE_Released, this, &APrototipoCharacter::Defenceoff);

}

//////UI//////

int APrototipoCharacter::GetGems() const
{
	return Gems;
}

bool APrototipoCharacter::GetDefence() const
{
	return bDefence;
}

float APrototipoCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool APrototipoCharacter::AttackAnim() const
{
	return bAttack;
}

float APrototipoCharacter::GetResistencePercent() const
{
	return Resistence / MaxResistence;
}

bool APrototipoCharacter::Death() const
{
	return bDeath;
}

///////////////////////////////////

void APrototipoCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SwordClass != nullptr)
	{
		Sword = GetWorld()->SpawnActor<ASword>(SwordClass);
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sword_r"));
		Sword->SetOwner(this);
	}
	GetWorld()->GetTimerManager().SetTimer(ULocation, this, &APrototipoCharacter::UpdateUltimateLocation, 1.0f, false);
}

void APrototipoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	vResistence(DeltaSeconds);

	vHeal(DeltaSeconds);
	
	bJump = CharacterMovement->IsFalling();

	
}

void APrototipoCharacter::MoveForward(float Value)
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

void APrototipoCharacter::MoveRight(float Value)
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


void APrototipoCharacter::Run()
{
	if(!bAttack)
	{
		CharacterMovement->MaxWalkSpeed = SpeedRun;
		bResistence = true;
	}
}

void APrototipoCharacter::NotRun()
{
	if (!bAttack)
	{
		CharacterMovement->MaxWalkSpeed = 600;
		bResistence = false;
	}
}

void APrototipoCharacter::vHeal(float DeltaSeconds)
{
	if(!Battle)
	{
		if (Health <= 100 && bHeal)
		{
			Health += Down * DeltaSeconds;
		}
		else if(Health <= 100 && !Healok)
		{
			GetWorld()->GetTimerManager().SetTimer(FHeal, this, &APrototipoCharacter::vHealTime, HealTime, false);
			Healok = true;
		}

	}
}

void APrototipoCharacter::vHealTime()
{
	bHeal = true;
}

void APrototipoCharacter::UpdateUltimateLocation()
{
	if (bSave())
	{
		UltimateLocation = GetActorLocation();
	}

	GetWorld()->GetTimerManager().SetTimer(ULocation, this, &APrototipoCharacter::UpdateUltimateLocation, 3.0f, false);
}

void APrototipoCharacter::Attack()
{
	if (M_Attack && !bAttack && !bJump && Resistence > DownAttack)
	{
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

		AttackResistence = true;

		GetWorld()->GetTimerManager().SetTimer(FAttack, this, &APrototipoCharacter::AttackActive, TimeAttack, false);

	}
}

void APrototipoCharacter::Defence()
{
	if (!bAttack && !bDefence && !bJump)
	{
		bDefence = true;
		CharacterMovement->MaxWalkSpeed = 300;
		bResistence = false;
		bAttack = true;
	}
}

void APrototipoCharacter::Defenceoff()
{
	if (bAttack && bDefence)
	{
		bDefence = false;
		CharacterMovement->MaxWalkSpeed = 600;
		AttackActive();
	}
}

void APrototipoCharacter::AttackActive()
{
	bAttackActive = false;
	bAttack = false;
}

void APrototipoCharacter::TeleportDeath()
{
	FOutputDeviceNull ar;
	
	this->CallFunctionByNameWithArguments(TEXT("RevivePlayer"), ar, NULL, true);
	SetActorLocation(CoordinatesIsland[IslandNumber]);
	Health = MaxHealth;
	bRevive = true;

}

void APrototipoCharacter::vDeath()
{
	FOutputDeviceNull ar;
	
	bDeath = true;

	Cancelstreak(Waves);
	this->CallFunctionByNameWithArguments(TEXT("DeathPlayer") , ar , NULL , true);
	GetWorld()->GetTimerManager().SetTimer(FDeath, this, &APrototipoCharacter::TeleportDeath, 4.0f, false);
	
}

void APrototipoCharacter::viWavesComplete(int iWaves)
{
	if (NumMissionG == iWaves)
	{
		if (WavesComplete[iWaves] == false)
		{
			WavesComplete[iWaves] = true;
			if (WavesComplete[2])
			{
				TextMission = "-Go to the seller's house on the right of the island";
			}
			else
			{
				TextMission = "-Return to the store for the next mission";
			}
		}
	}
	Cancelstreak(iWaves);
}

void APrototipoCharacter::vWavesComplete()
{
	if (Waves >= 1 || Waves <= 6)
	{
		if (EnemyKill >= EnemyWaves)
		{
			viWavesComplete(Waves);
		}
	}
}

void APrototipoCharacter::Cancelstreak(int iWaves)
{
	if (WavesComplete[iWaves] == false)
	{
		EnemyKill = 0;
	}
}

int APrototipoCharacter::vNumberSpawn()
{
	return NumberSpawn;
}

FString APrototipoCharacter::vMissionText()
{
	return TextMission;
}

void APrototipoCharacter::Mission(int NumMission)
{
	NumMissionG = NumMission;
	switch (NumMission)
	{
	case 1:
		if (!WavesComplete[1])
		{
			TextMission = "-Kill the 3 enemies on island 1";
		}
		break;
	case 2:
		if (WavesComplete[1] && !WavesComplete[2])
		{
			TextMission = "-Kill the 5 enemies on island 2";
		}
		break;
	case 3:
		if (WavesComplete[2] && !WavesComplete[3])
		{
			TextMission = "-Kill the 3 enemies on island 2";
		}
		break;
	case 4:
		if (WavesComplete[3] && !WavesComplete[4])
		{
			TextMission = "-Kill the 4 enemies on island 2";
		}
		break;
	case 5:
		if (WavesComplete[4] && !WavesComplete[5])
		{
			TextMission = "-Kill the boss on island 3";
		}
		break;
	default:
		break;
	}
}


void APrototipoCharacter::vResistence(float DeltaSeconds)
{
	if (bResistence && Resistence > 0 && !bRStay && !bJump || bResistence && Resistence > 0 && !bLStay && !bJump)
	{
		Resistence -= Down * DeltaSeconds;
	}
	else if (!bResistence && Resistence <= 100)
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

////////Save////////

void APrototipoCharacter::SaveGame(int ISlot, FString FSlot)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	SaveGameInstance->Gems = Gems;
	SaveGameInstance->IslandNumber = IslandNumber;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, FSlot, ISlot);

	print("Game Saved");
}

void APrototipoCharacter::LoadGame(int ISlot, FString FSlot)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance = Cast <UMySaveGame>(UGameplayStatics::LoadGameFromSlot(FSlot, ISlot));

	this->SetActorLocation(SaveGameInstance->PlayerLocation);
	Gems = SaveGameInstance->Gems;
	IslandNumber = SaveGameInstance->IslandNumber;
	

	print("Game Load");
}

bool APrototipoCharacter::bSave() const
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

bool APrototipoCharacter::vJump() const
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

bool APrototipoCharacter::vRevive() const
{
	return bRevive;
}

//////////////////////
