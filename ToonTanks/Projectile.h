// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);				//we need this funciton as a function that is called when a hit event occured. The name of the variable can be upto us but the parameter need to satisfy what the delegate needs (in this case, what the "OnComponentHit.AddDynamic"delegate needs otherwise the invocation list can't be called. Syntax: void var(actor being hit, actor that got hit, other component that was hit (e.g. static component of other actor), If we simulating physics the physics engine will apply in response to the collision and this FVector represent the direction and rotation of that impulse, calling the hitresult to provide more information about the hit.)

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere)
	float DamageAmount = 10.f;

	// UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	// float 
	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitPawnParticles;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitParticles2;

	class ATank* Tank;


	// UPROPERTY(EditAnywhere)
	// class UParticleSystem* ProjectileParticle;



	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* TrailParticle;

	UPROPERTY(EditAnywhere)
	class USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;		//need to make this TSubclassof bcs ClientStartCameraShake() (Function to play the camera shake) needs to take in TSubClassOf as its first parameter input. and this will assign which of the 2 BP class we have set (DeathCameraShake or HitCameraShake (we set different vibration oscillation strength frequency and amplitude to each of the class)) will be the one used to play the CameraShake (we'll be choosing this manually from the details tab BP_Projectile, that is why we set this as VisibleAnywhere). UCameraShakeBase is the class to define camera shake 


};
