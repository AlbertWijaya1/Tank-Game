// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	float Health = 100.f;
		
private:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components", meta	= (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);				//callback parameter and this need to have the same amount of parameter as the delegate needs other wise the callback function cannot be called: (the actor taking the damage, damage amount, custom damage type(fire, poison, explosive, etc), controller responsible for the damage (if damage is caused by a player controlling a pawn, then the instigator is the player that possessing that pawn), the actual actor causing the damage (in this case the projectile itself))

	class AToonTanksGameMode* ToonTanksGameMode;
};
