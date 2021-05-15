// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Misc/OutputDeviceNull.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Spawn1 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn1"));
	Spawn2 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn2"));
	Spawn3 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn3"));
	Spawn4 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn4"));
	Spawn5 = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn5"));
	
	Spawn1->SetupAttachment(RootComponent);
	Spawn2->SetupAttachment(RootComponent);
	Spawn3->SetupAttachment(RootComponent);
	Spawn4->SetupAttachment(RootComponent);
	Spawn5->SetupAttachment(RootComponent);


	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	OutBoxRespawn = CreateDefaultSubobject<UBoxComponent>(TEXT("OutBoxRespawn"));
	OutBoxRespawn->SetupAttachment(RootComponent);

}

void ASpawner::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrototipoCharacter* Character = Cast<APrototipoCharacter>(OtherActor);
	if(Character)
	{
		Character->NumberSpawn = NumberSpawn;
		Character->EnemyWaves = AmountEnemy;
		Battle = true;
		Character-> Battle = true;
		Character->Waves = Waves;
	}
}

void ASpawner::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	APrototipoCharacter* Character = Cast<APrototipoCharacter>(OtherActor);
	if(Character)
	{	
		Battle = false;
		Character->Battle = false;
		Character->Healok = false;
		Character->bHeal = false;
		Character->NumberSpawnPast = NumberSpawn;
		Character->NumberSpawn = 0;
	}
}

void ASpawner::OnOverlapEnd2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	APrototipoCharacter* Character = Cast<APrototipoCharacter>(OtherActor);
	if (Character)
	{		
		FOutputDeviceNull ar;

		this->CallFunctionByNameWithArguments(TEXT("Respawns"), ar, NULL, true);
	}

}

void ASpawner::Spawn()
{
	if (SpawnObject != NULL)
	{
		if(AmountEnemy >= 1)
		{
			FSpawn(Spawn1);
			if (AmountEnemy >= 2)
			{
				FSpawn(Spawn2);
				if (AmountEnemy >= 3)
				{
					FSpawn(Spawn3);
					if (AmountEnemy >= 4)
					{
						FSpawn(Spawn4);
						if (AmountEnemy >= 5)
						{
							FSpawn(Spawn5);
						}
					}
				}
			}
		}
	}
}

void ASpawner::FSpawn(USceneComponent* SpawnScene)
{
	FActorSpawnParameters SpawnParams;

	FVector SpawnLocation = SpawnScene->GetComponentLocation();
	FRotator Spawnrotation = FRotator::ZeroRotator;
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>
		(SpawnObject, SpawnLocation, Spawnrotation, SpawnParams);

	Enemy->NumberSpawn = NumberSpawn;
}

bool ASpawner::vBattle() const
{
	return Battle;
}

bool ASpawner::vDeath() const
{
	if(CharacterD!= NULL)
	{
		return CharacterD->bDeath;
		
	}
	else
	{
		return true;
	}
}

bool ASpawner::vRevive() const
{
	return CharacterD->bRevive;
}


int ASpawner::vNumberSpawn()
{
	return NumberSpawn;
}

int ASpawner::vNumberSpawnPast()
{
	return CharacterD->NumberSpawnPast;
}

int ASpawner::vNumberSpawnCharacter()
{
	return CharacterD->NumberSpawn;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpawner::BeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASpawner::OnOverlapEnd);

	OutBoxRespawn->OnComponentEndOverlap.AddDynamic(this, &ASpawner::OnOverlapEnd2);
	
	CharacterD = Cast<APrototipoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Spawn();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

