// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleSystem"));
	IdleParticleSystem->SetupAttachment(GetRootComponent());

	// Don't need to CreateDefaultSubObject of OverlapParticleSystem

	bShouldRotate = false;
	RotateRate = 45.f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldRotate)
	{
		FRotator CurrentRotation = GetActorRotation();

		CurrentRotation.Yaw += RotateRate * DeltaTime;

		SetActorRotation(CurrentRotation);
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			if (OverlapParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticleSystem, GetActorLocation());
			}

			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			Destroy();
		}
	}
}