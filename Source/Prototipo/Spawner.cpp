// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"



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

}

void ASpawner::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
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
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	Spawn();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

