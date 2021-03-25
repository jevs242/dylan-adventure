// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

// Sets default values
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PMesh"));
	PMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	PMesh->SetupAttachment(RootComponent);

	BoxCollision->SetupAttachment(PMesh);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASword::BeginOverlap);
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
