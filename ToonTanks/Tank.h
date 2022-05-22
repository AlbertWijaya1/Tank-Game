// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
// #include "GameFramework/SpringArmComponent.h"			// not needed here bcs we are only calling this function at the cpp file! so we can just put "class" here since we are only declaring it in th header file.
// #include "Camera/CameraComponent.h"
#include "Tank.generated.h"

UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();					//need to do this if we want to create a constructor at the Tank.cpp

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Move(float Value);
	void Turn(float Value);

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const{ 			//this is the getterfunction! where we put "Get" in front of the variable/ function we want to get. in this case PlayerController. We want to set this as public so that we can access it outside of this class, in this case, we want to access it in the ToonTanksGameMode class.
		return PlayerControllerRef;
	}
	bool bTankAlive = true;
	bool bGunReceived;
	bool bTokenReceived = false;
	float SpeedEndTime;


private:

	UPROPERTY(VisibleAnywhere, meta	= (AllowPrivateAccess = "true"))
	class USpringArmComponent* Arm;
	UPROPERTY(VisibleAnywhere, meta	= (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta	= (AllowPrivateAccess = "true"))
	float Speed = 800.f;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta	= (AllowPrivateAccess = "true"))
	float TurnRate = 200.f;

	APlayerController* PlayerControllerRef;			//simply just mean a pointer to player controller

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* MoveTrail;

	UPROPERTY(EditAnywhere)
	UAudioComponent* WheelSound;

	float GetTokenTime;
	float Time;
	UPROPERTY(EditAnywhere)
	UAudioComponent* TokenReceivedSound;
	UPROPERTY(EditAnywhere)
	UAudioComponent* TokenFinishSound;

	float TimeDifference;

	bool bTokenReceivedPlayed;
	bool bTokenFinishPlayed;

};