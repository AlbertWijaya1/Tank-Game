// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "BasePawn.h"
#include "Tank.h"
#include "Projectile.h"
// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	RootComponent = GunMesh;	//RootComponent is already part of the actor class, if you ctrl+click on it it will open the actor.h file showing that it is declared with the USceneComponent data type.
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	GunMesh->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	BasePawn = Cast<ABasePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	// BasePawn = Cast<ABasePawn>(UGameplayStatics::GetGameMode(this));
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = FRotator::ZeroRotator;              //this is similar to FRotator(0); in this case ZeroRotator is a static variable (ada di FRotator de definition!)
    float DeltaTime2 = UGameplayStatics::GetWorldDeltaSeconds(this);
    Rotation.Yaw = 1*DeltaTime2*200.f;  
	AddActorLocalRotation(Rotation, true);

}

void AWeapon::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){

	if(ABasePawn* HitPawn = Cast<ABasePawn>(OtherActor)){								//to distinguish! check if the one got hit is a pawn. if want to have action to only occur if a projectile hits a pawn, put it under this if statement. if want to do Tank only or Tower only, simply change ABasePawn to ATank or ATower.
		Destroy();
		if(GunReceivedSound){			//making sure the HitSound is assigned in the blueprint details tab (we set it to be EditAnywhere in the headers file), if we didn't it will result in null pointer and wo this if check it will crash unreal.
			UGameplayStatics::PlaySoundAtLocation(this, GunReceivedSound, GetActorLocation());	//we put this in the begin play not that it is going to play when we hit the play button. since this is an actor and we only set this actor to existense when we fire the projectile, means that this BeginPlay will only get excecuted once the projectile is spawn and can be called many times not just during and one time like the normal begin play. PlaySoundAtLocation(World context object, What sound to play, where to play);
		}
			Tank->bGunReceived = true;
			UE_LOG(LogTemp, Warning, TEXT("No Tank Found!"));
		if(Tank){
		}
		
		// FVector SpawnPointLocation = BasePawn->ProjectileSpawnPoint->GetComponentLocation();		//projectile is a component not an actor! so therefore its GetComponentLocation instead of GetActorLocation.
		// FRotator SpawnPointRotation = BasePawn->ProjectileSpawnPoint ->GetComponentRotation();
		// auto Gun = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPointLocation, SpawnPointRotation);			//if you hover into SpawnActor() it says that it return type is the parent class of the type, in this case it returns AProjectile* (a pointer). Why we need to store it in Projectile variable? bcs we wanted to make sure that its owner is set to the pawn that spawns it. "ProjectileClass" is the blueprint version of projectile.cpp! we want the mesh of the projectile and therefore we need to access the blueprint instances that we created out of projectile class, and this can be done by using TSubClassOF in BasePawn.h. After this, we can now open the blueprint instance of projectile.cpp and set the blueprint version of it as the "ProjectileClass" in the details tab.
		// Gun->SetOwner(this); //making sure that the owner is set to the pawn that spawns the projectile. so whenever we call "GetOwner" to the projectile, we are sure that it must be talking about the class that calls that projectile. if Tank called projectile then GetOwner will be ATank, whereas if tower call the projectile, ATower will be the GetOwner.
	}
}


