// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinPickup.h"
#include "PlayerCharacter.h"

ACoinPickup::ACoinPickup()
{

}


void ACoinPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			PlayerCharacter->IncrementCoinCount();
		}
	}
}