// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Speedometer.generated.h"

UCLASS()
class TOONTANKS_API ASpeedometer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeedometer();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SpeedometerMesh;
	class ABasePawn* BasePawn;
	class ATank* Tank;




};
