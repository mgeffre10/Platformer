// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"

UENUM()
enum CritterType
{
	Spiderling,
	Golem,
	Gruntling
};

UCLASS()
class SIMPLEPLATFORMER_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	TEnumAsByte<CritterType> CritterMesh;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
private:
	UPROPERTY()
	class USkeletalMeshComponent* SkeletalMeshComponent;

	FORCEINLINE class USkeletalMesh* GetSpiderlingSkeletalMesh()
	{
		return LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Spiderling/SK_Spiderling.SK_Spiderling'"), NULL, LOAD_None, NULL);
	}

	FORCEINLINE class USkeletalMesh* GetGolemSkeletalMesh()
	{
		return LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Golem/SK_Fire_Golem.SK_Fire_Golem'"), NULL, LOAD_None, NULL);
	}
	FORCEINLINE class USkeletalMesh* GetGruntlingSkeletalMesh()
	{
		return LoadObject<USkeletalMesh>(NULL, TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/SK_Exodus_Gruntling.SK_Exodus_Gruntling'"), NULL, LOAD_None, NULL);
	}

	void InitSkeletalMeshAssets();
	void DetermineSkeletalMesh();
};
