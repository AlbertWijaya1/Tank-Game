// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);			//OnTakeAnyDamage.AddDynamic() (built in UE function) is a multitaskdelegate! which means it can store function and call them if the event occured (in this case, "DamageTaken" Function is being called On Damage Taken(as the name implies)) where DamageTaken is the function bound to the OnTakeAnyDamage Delegate
	// ...
	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));		//accessing the gamemode that we are in, we can use the function GetGameMode
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent:: DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)				//callback parameter and this need to have the same amount of parameter as the delegate needs otherwise the callback function cannot be called: (the actor taking the damage, damage amount, custom damage type(fire, poison, explosive, etc), controller responsible for the damage (if damage is caused by a player controlling a pawn, then the instigator is the player that possessing that pawn), the actual actor causing the damage (in this case the projectile itself)). This is built in feature from UE! so the var name define what it does, damage accounts for amount of damage being received by the object getting hit, so if wanna talk about damageamount, we are talking about this variable, same thing with damageactor (who get hit?), type (what is hittin?), etc)
{
	if(Damage<=0.f)return;		//check if the damage is less or equal to 0, if it does.. exit the function. bad practice to put 'return' outside the bracket! but i just leave it this way for the sake of telling that this is doable...
	Health -= Damage;
	if(Health<=0.f){
		if(ToonTanksGameMode){
			ToonTanksGameMode->ActorDied(DamagedActor);
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
}

//test commit
