// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class USoundBase;

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Pawn Mesh")
	float time = 10.f;
	void HandleDestruction();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;
protected:

	void RotateTurret(FVector LookAtTarget);					// we put functions/ variables under "protected" section so that they are accessible to its parent class, itc the tank and tower class
	void Fire();
private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))		//visibleanywhere instead of edit bcs we dont wanna change the pointer its pointed to in the constructor.
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;


	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	float angle = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile>ProjectileClass;			//here we are storing the AProjectile UClass in variable called ProjectileClass. This allows all the associated class related to AProjectile to be called, including the child and instances, in this case since we create the blueprint version of AProjectile, we can also assign the blueprint to the ProjectileClass variable. Thus, using this approach we can then call the blueprint side of the class in our other cpp class (by calling the ProjectileClass variable)

	UPROPERTY(EditAnywhere)
	UParticleSystem* DeathParticle;
	

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;		//need to make this TSubclassof bcs ClientStartCameraShake() (Function to play the camera shake) needs to take in TSubClassOf as its first parameter input. and this will assign which of the 2 BP class we have set (DeathCameraShake or HitCameraShake (we set different vibration oscillation strength frequency and amplitude to each of the class)) will be the one used to play the CameraShake (we'll be choosing this manually from the details tab BP_Projectile, that is why we set this as VisibleAnywhere). UCameraShakeBase is the class to define camera shake 


};
