//Jose E Velazquez Sepulveda
//Blocking.cpp

#include "Blocking.h"
#include "Kismet/GameplayStatics.h"
#include "DylanAdventureCharacter.h"


ABlocking::ABlocking()
{
	PrimaryActorTick.bCanEverTick = true;

	PMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PMesh"));
	PMesh->SetupAttachment(RootComponent);
}

void ABlocking::vWaves(int iWaves)
{
	if(Character->WavesComplete[iWaves] && Character->Accept[iWaves])
	{
		Destroy();
	}
}

void ABlocking::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ADylanAdventureCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABlocking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Character)
	{
		vWaves(Waves);
	}
}

