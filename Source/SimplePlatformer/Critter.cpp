// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitSkeletalMeshAssets();
	DetermineSkeletalMesh();
}

void ACritter::InitSkeletalMeshAssets()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;

	
}

void ACritter::DetermineSkeletalMesh()
{
	switch (CritterMesh)
	{
		case CritterType::Golem:
		{
			USkeletalMesh* GolemMesh = GetGolemSkeletalMesh();

			if (GolemMesh != nullptr)
			{
				SkeletalMeshComponent->SetSkeletalMesh(GolemMesh);
			}
			break;
		}
		case CritterType::Gruntling:
		{
			USkeletalMesh* GruntlingMesh = GetGruntlingSkeletalMesh();

			if (GruntlingMesh != nullptr)
			{
				SkeletalMeshComponent->SetSkeletalMesh(GruntlingMesh);
			}
			break;
		}
		case CritterType::Spiderling:
		default:
		{
			USkeletalMesh* SpiderlingMesh = GetSpiderlingSkeletalMesh();

			if (SpiderlingMesh != nullptr)
			{
				SkeletalMeshComponent->SetSkeletalMesh(SpiderlingMesh);
			}
			break;
		}
	}
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACritter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == TEXT("CritterMesh"))
	{
		DetermineSkeletalMesh();
	}
}