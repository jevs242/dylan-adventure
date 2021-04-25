// Fill out your copyright notice in the Description page of Project Settings.


#include "OutMap.h"

#include "PrototipoCharacter.h"
#include "Components/BoxComponent.h"

#define print(x) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));

// Sets default values
AOutMap::AOutMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetupAttachment(RootComponent);


}


// Called when the game starts or when spawned
void AOutMap::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AOutMap::BeginOverlap);
}

// Called every frame
void AOutMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AOutMap::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrototipoCharacter* Character = Cast<APrototipoCharacter>(OtherActor);
	if (Character)
	{
		Character->bJump = false;
		Character->SetActorLocation(Character->UltimateLocation);
	}
}