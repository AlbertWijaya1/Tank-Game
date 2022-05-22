// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckFireCondition();
	bool InFireRange();
	void HandleDestruction();

private: 
	class ATank* Tank;		// need class so that we don't need the Tank.h to be included at the top. This is to access the Tank Pawn at the tank class.
	UPROPERTY(EditAnywhere)
	float FireRange;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.f;
};