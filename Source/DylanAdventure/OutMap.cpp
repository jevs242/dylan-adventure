//Jose E Velazquez Sepulveda
//OutMap.cpp

#include "OutMap.h"

#include "DylanAdventureCharacter.h"
#include "Components/BoxComponent.h"

AOutMap::AOutMap()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

void AOutMap::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AOutMap::BeginOverlap);
}

void AOutMap::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADylanAdventureCharacter* Character = Cast<ADylanAdventureCharacter>(OtherActor);
	if (Character)
	{
		Character->bJump = false;
		Character->SetActorLocation(Character->UltimateLocation);
	}
}