// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Materials/Material.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ACustomPlatform::ACustomPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformStaticMesh"));
	// Set StaticMesh mobility to Static
	StaticMesh->SetMobility(EComponentMobility::Static);
	// Set RootComponent to StaticMesh
	RootComponent = StaticMesh;

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Steel.M_Metal_Steel'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> CheckpointMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FinalPlatformMaterialAsset(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));

	if (DefaultMaterialAsset.Succeeded())
	{
		DefaultMaterial = DefaultMaterialAsset.Object;
	}

	if (CheckpointMaterialAsset.Succeeded())
	{
		CheckpointPlatformMaterial = CheckpointMaterialAsset.Object;
	}

	if (FinalPlatformMaterialAsset.Succeeded())
	{
		FinalPlatformMaterial = FinalPlatformMaterialAsset.Object;
	}

	StaticMesh->SetMaterial(0, DefaultMaterial);
	// Material for bIsCheckpoint
	// Material for bIsFinalPlatform
	bIsMoving = false;
	bIsCheckpoint = false;
	bIsFinalPlatform = false;
	MovementDirectionX = 0;
	MovementDirectionY = 0;
	MovementDirectionZ = 0;
	MovementDirection = FVector::ForwardVector;
	PlatformSize = Size::Medium;
	MovementDistance = 400.0f;
	MovementSpeedMultiplier = 1.0f;
	bShouldPauseBeforeMoving = false;
	PauseTime = 1;
	Speed = 100.0f;
	Direction = 1.0f;
}

// Called when the game starts or when spawned
void ACustomPlatform::BeginPlay()
{
	Super::BeginPlay();

	InitValues();
}

// Called every frame
void ACustomPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving && bShouldMove)
	{
		MovePlatform(DeltaTime);
	}

}

void ACustomPlatform::InitValues()
{
	MovementDirection = FVector(MovementDirectionX, MovementDirectionY, MovementDirectionZ);

	MaxDistance = GetActorLocation() + (MovementDistance * MovementDirection);
	StartingLocation = GetActorLocation();

	bShouldMove = true;

	if (bShouldPauseBeforeMoving)
	{
		TimeRemaining = PauseTime;
	}


	DeterminePlatformSize();
	DeterminePlatformDirection();
}

void ACustomPlatform::DeterminePlatformSize()
{
	FVector NewSize = FVector(1.0f);

	if (PlatformSize == Size::Small)
	{
		NewSize = FVector(0.5f);
	}
	else if(PlatformSize == Size::Large)
	{
		NewSize = FVector(1.5f);
	}
	else if (PlatformSize == Size::ExtraLarge)
	{
		NewSize = FVector(2.0f);
	}

	SetActorScale3D(NewSize);
}

void ACustomPlatform::DeterminePlatformDirection()
{

}

void ACustomPlatform::MovePlatform(float DeltaTime)
{
	FVector CurrentActorLocation = GetActorLocation();

	FVector NewLocation = MovementDirection * ((Speed * MovementSpeedMultiplier) * DeltaTime);
	CurrentActorLocation = CurrentActorLocation + (NewLocation * Direction);
	SetActorLocation(CurrentActorLocation);

	if (HasReachedMaxDistance(CurrentActorLocation))
	{
		Direction = -Direction;

		if (bShouldPauseBeforeMoving)
		{
			bShouldMove = false;
			GetWorld()->GetTimerManager().SetTimer(PausePlatformTimerHandle, this, &ACustomPlatform::TriggerCountdownToMovement, 1.0f, true);
		}
		
	}
}

bool ACustomPlatform::HasReachedMaxDistance(FVector CurrentLocation)
{
	if (HasAxisExceededMax(CurrentLocation.X, MaxDistance.X, StartingLocation.X)
		|| HasAxisExceededMax(CurrentLocation.Y, MaxDistance.Y, StartingLocation.Y)
		|| HasAxisExceededMax(CurrentLocation.Z, MaxDistance.Z, StartingLocation.Z))
	{
		return true;
	}

	return false;
}

bool ACustomPlatform::HasAxisExceededMax(float CurrentAxisValue, float MaxAxisValue, float StartingAxisValue)
{
	if (MaxAxisValue == StartingAxisValue)
	{
		return false;
	}

	if (MaxAxisValue < 0 || StartingAxisValue > 0)
	{
		if (CurrentAxisValue < MaxAxisValue || CurrentAxisValue > StartingAxisValue)
		{
			return true;
		}
	}
	else if (MaxAxisValue > 0 || StartingAxisValue < 0)
	{
		if (CurrentAxisValue > MaxAxisValue || CurrentAxisValue < StartingAxisValue)
		{
			return true;
		}
	}

	return false;
}

void ACustomPlatform::TriggerCountdownToMovement()
{
	if (--TimeRemaining <= 0)
	{
		bShouldMove = true;
		TimeRemaining = PauseTime;
		GetWorld()->GetTimerManager().ClearTimer(PausePlatformTimerHandle);
	}
}

void ACustomPlatform::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString PropertyName = PropertyChangedEvent.GetPropertyName().ToString();
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *PropertyName);
	// If bIsMoving is changed, update mobility as necessary
	if (PropertyName == TEXT("bIsMoving"))
	{
		if (bIsMoving)
		{
			StaticMesh->SetMobility(EComponentMobility::Movable);
		}
		else
		{
			StaticMesh->SetMobility(EComponentMobility::Static);
		}
	}
	else if (PropertyName == TEXT("bIsCheckpoint"))
	{
		if (bIsCheckpoint)
		{
			StaticMesh->SetMaterial(0, CheckpointPlatformMaterial);
		}
		else
		{
			StaticMesh->SetMaterial(0, DefaultMaterial);
		}
	}
	else if (PropertyName == TEXT("bIsFinalPlatform"))
	{
		if (bIsFinalPlatform)
		{
			StaticMesh->SetMaterial(0, FinalPlatformMaterial);
		}
		else
		{
			StaticMesh->SetMaterial(0, DefaultMaterial);
		}
	}
	else if (PropertyName == TEXT("PlatformSize"))
	{
		DeterminePlatformSize();
	}
}

