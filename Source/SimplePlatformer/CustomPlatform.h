// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomPlatform.generated.h"

UENUM()
enum Size
{
	Small UMETA(DisplayName = "Small"),
	Medium UMETA(DisplayName = "Medium"),
	Large UMETA(DisplayName = "Large"),
	ExtraLarge UMETA(DisplayName = "ExtraLarge")
};

UCLASS()
class SIMPLEPLATFORMER_API ACustomPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomPlatform();

	UPROPERTY(VisibleDefaultsOnly, Category = "Platform Variables")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Platform Variables | Materials")
	class UMaterial* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Platform Variables | Materials")
	class UMaterial* CheckpointPlatformMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Platform Variables | Materials")
	class UMaterial* FinalPlatformMaterial;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables")
	TEnumAsByte<Size> PlatformSize;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables", meta = (EditCondition = "!bIsFinalPlatform"))
	bool bIsCheckpoint;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables", meta = (EditCondition = "!bIsCheckpoint"))
	bool bIsFinalPlatform;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement")
	bool bIsMoving;

	// Below Properties shown only if bIsMoving is true

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving", UIMin = "-1.0", UIMax = "1.0"))
	int MovementDirectionX;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving", UIMin = "-1.0", UIMax = "1.0"))
	int MovementDirectionY;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving", UIMin = "-1.0", UIMax = "1.0"))
	int MovementDirectionZ;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving") )
	float MovementDistance;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving"))
	float MovementSpeedMultiplier;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving") )
	bool bShouldPauseBeforeMoving;

	// Property should show only if bShouldPauseBeforeMoving is true
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Platform Variables | Movement", meta = (EditCondition = "bIsMoving && bShouldPauseBeforeMoving") )
	int PauseTime;

protected:

	FVector MaxDistance;
	FVector StartingLocation;
	FVector MovementDirection;
	FTimerHandle PausePlatformTimerHandle;

	float Speed;
	int Direction;
	int TimeRemaining;
	bool bShouldMove;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool HasReachedMaxDistance(FVector CurrentLocation);
	bool HasAxisExceededMax(float CurrentAxisValue, float MaxAxisValue, float StartingAxisValue);

	void MovePlatform(float DeltaTime);
	void TriggerCountdownToMovement();
	void InitValues();
	void DeterminePlatformSize();
	void DeterminePlatformDirection();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
