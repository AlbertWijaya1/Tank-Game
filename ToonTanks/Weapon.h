// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TOONTANKS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GunMesh;

	class ABasePawn* BasePawn;
	class AProjectile* Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor>ActorToSpawn;			//here we are storing the AProjectile UClass in variable called ProjectileClass. This allows all the associated class related to AProjectile to be called, including the child and instances, in this case since we create the blueprint version of AProjectile, we can also assign the blueprint to the ProjectileClass variable. Thus, using this approach we can then call the blueprint side of the class in our other cpp class (by calling the ProjectileClass variable)


	UPROPERTY(EditAnywhere)
	USoundBase* GunReceivedSound;

	class ATank* Tank;

};
