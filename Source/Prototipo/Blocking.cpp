// Fill out your copyright notice in the Description page of Project Settings.


#include "Blocking.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABlocking::ABlocking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PMesh"));
	PMesh->SetupAttachment(RootComponent);
}

void ABlocking::vWaves(int iWaves)
{
	if(Character->WavesComplete[iWaves])
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ABlocking::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<APrototipoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ABlocking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	vWaves(Waves);
	
}

