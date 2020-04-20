// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlatform.h"
#include "Components/BoxComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
ACustomPlatform::ACustomPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformStaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	bIsMoving = false;
	bIsCheckpoint = false;
	bIsFinalPlatform = false;
	MovementDirectionX = 0;
	MovementDirectionY = 0;
	MovementDirectionZ = 0;
	MovementDirection = FVector::ForwardVector;
	PlatformSize = ESize::ES_Medium;
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

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ACustomPlatform::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ACustomPlatform::OnOverlapEnd);
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
}

void ACustomPlatform::DeterminePlatformSize()
{
	FVector NewSize = FVector(1.0f);

	if (PlatformSize == ESize::ES_Medium)
	{
		NewSize = FVector(2.f);
	}
	else if(PlatformSize == ESize::ES_Large)
	{
		NewSize = FVector(3.f);
	}
	else if (PlatformSize == ESize::ES_ExtraLarge)
	{
		NewSize = FVector(5.f);
	}

	SetActorScale3D(NewSize);
}

void ACustomPlatform::MovePlatform(float DeltaTime)
{
	FVector CurrentActorLocation = GetActorLocation();

	// Caculate the distance vector to add to current actor location and add it
	CurrentActorLocation += MovementDirection * (((Speed * MovementSpeedMultiplier) * DeltaTime) * Direction);

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
	if (HasAxisExceededMax(CurrentLocation.X, MaxDistance.X, StartingLocation.X, MovementDirectionX)
		|| HasAxisExceededMax(CurrentLocation.Y, MaxDistance.Y, StartingLocation.Y, MovementDirectionY)
		|| HasAxisExceededMax(CurrentLocation.Z, MaxDistance.Z, StartingLocation.Z, MovementDirectionZ))
	{
		return true;
	}

	return false;
}

bool ACustomPlatform::HasAxisExceededMax(float CurrentAxisValue, float MaxAxisValue, float StartingAxisValue, int AxisDirection)
{
	if (MaxAxisValue == StartingAxisValue || AxisDirection == 0)
	{
		return false;
	}

	if (AxisDirection == -1)
	{
		return MaxAxisValue > CurrentAxisValue || CurrentAxisValue > StartingAxisValue;
	}
	else
	{
		return MaxAxisValue < CurrentAxisValue || CurrentAxisValue < StartingAxisValue;
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
	
	if (DefaultMaterial && CheckpointPlatformMaterial && FinalPlatformMaterial)
	{
		if (PropertyName == TEXT("bIsCheckpoint"))
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
	}
	
	if (PropertyName == TEXT("PlatformSize"))
	{
		DeterminePlatformSize();
	}
}

void ACustomPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsCheckpoint)
	{
		if (OtherActor)
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

			if (PlayerCharacter)
			{
				PlayerCharacter->SpawnPos.X = GetActorLocation().X;
				PlayerCharacter->SpawnPos.Y = GetActorLocation().Y;
			}
		}
	}
	else if (bIsFinalPlatform)
	{
		if (OtherActor)
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

			if (PlayerCharacter)
			{
				UE_LOG(LogTemp, Warning, TEXT("Final Platform Detected!"));
			}
		}
	}

	
}

void ACustomPlatform::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}