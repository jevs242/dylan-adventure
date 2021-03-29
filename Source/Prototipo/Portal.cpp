// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "PrototipoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortal::BeginOverlap);

	TeleportLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportLocation"));
	TeleportLocation->SetupAttachment(RootComponent);

}


// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	TargetTeleportLocation = TeleportLocation->GetComponentLocation();
	Personaje = Cast<APrototipoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::Teleport()
{
	Personaje->SetActorLocation(TargetTeleportLocation);
	GetWorld()->GetTimerManager().SetTimer(FMove, this, &APortal::Move, 2.f, false);
}

void APortal::Move()
{
	bTeleport = false;//
	Personaje->bAttack = false;
}


void APortal::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Personaje == OtherActor)
	{
		Personaje->bAttack = true;
		bTeleport = true;//Cambio
		GetWorld()->GetTimerManager().SetTimer(FMove, this, &APortal::Teleport, 1.f, false);
	}
}

