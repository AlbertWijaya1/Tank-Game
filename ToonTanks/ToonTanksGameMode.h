// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HealthComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
void ActorDied(AActor* DeadActor);
// class ATower* DestroyedTower;

private:
class ATank* Tank;
class ATower* Tower;
class AToonTanksPlayerController* ToonTanksPlayerController;
float StartDelay = 3.0f;
void HandleGameStart();
int32 TargetTower = 0;
int32 GetTargetTowerCount();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)			//this is so that we are defining the function in BP and not cpp! we don't even need to give it a function body, UE will expect us to implement the functionality of this in BP
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)			//this is so that we are defining the function in BP and not cpp! we don't even need to give it a function body, UE will expect us to implement the functionality of this in BP
	void   GameOver(bool bWonGame);





};


