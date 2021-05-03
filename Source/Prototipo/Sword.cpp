// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

#include "Kismet/GameplayStatics.h"


#define print(x) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));
#define log(x) UE_LOG(LogTemp, Error, TEXT(x));

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
	BoxCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASword::BeginOverlap);

	Character = Cast<APrototipoCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASword::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CharacterSword)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);

		if (Enemy && Character->bAttackActive && !Enemy->bDeath)
		{
			Character->bAttackActive = false;
			Enemy->Health -= Character->Damage;
			if (Enemy->Health <= 0)
			{
				Enemy->Death();
			}
		}
	}
	else
	{
		if (Character == OtherActor)
		{
			if (!Character->bDefence && bAttackEnemy && !Character->bDeath)
			{
				bAttackEnemy = false;
				Character->Health -= Damage;
				if (Character->Health <= 0)
				{
					Character->vDeath();
				}
			}

		}
	}

}
