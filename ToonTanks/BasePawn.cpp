// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;															//RootComponent is already part of the actor class, if you ctrl+click on it it will open the actor.h file showing that it is declared with the USceneComponent data type. The data type assigned in this line can differ because UCapsuleComponent (CapsuleComp) inherits from USceneComponent(RootComponent), it is possible to set an equal sign to the variables. RootComponent is used for transform (knowing its rotation and translation) but it has no visual representation, thus we need to set this equal to UCapsuleComponent (its derivation) such that it can have a visual representation

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh -> SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh -> SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint -> SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::HandleDestruction(){			//this function is not being called anywher in this class but since this BasePawn class has 2 BP inherited from it (Tower and Tank), we can call the fucntion there instead! and if you check those classes, we did call the HandleDestruction function there when the respective(tank or tower) get destroyed.
	//Use visual and sound effects
	if(DeathParticle){		// Null pointer prevention, checking if we have set the  DeathParticle in the blueprint. DeathParticle is the cloudish effect when a pawn gets destroyed.
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());	//We put this right after we destroy the actor such coz it just doesn't make any sense to spawn the effects only right after the actor is destroyed. There are 3 options to fill in the parameter SpawnEmitterAtLocation(), hover over and click on the down arrow button to know what it is. The one we're using SpawnEmitterAtLocation(World context object, Particle system (we define this in the header file by the UparticleSystem class), Location, Rotation)
		
	}
	if(DeathSound){			//The sound when a pawn gets destroyed. making sure the DeathSound is assigned in the blueprint details tab (we set it to be EditAnywhere in the headers file), if we didn't it will result in null pointer and wo this if check it will crash unreal.
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());	//PlaySoundAtLocation(World context object, What sound to play, where to play);
	}

	if(HitCameraShakeClass){	//Null prevention, making sure we a class assigned to the HitCameraShake. the class must be a Martinee Camera Shake and we can set the ocillation stregth from there. Since we can choose what class we want to put in as the "HitCameraShake", we can basically set the stregth as we want depeding on the condition. e.g. when a pawn dies, shake harder, when just a simple projectile hitting something (this is what we did here!), shake softer. 
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass); //The ClientStartCameraShake() function belongs to the player controller and we are now in projectile.cpp so to access the playercontroller, we first need to GetFirstPlayerController() and this exist on UWorld class, thus the GetWorld()->GetFirstPlayerController() before the desired function
	}

}

void ABasePawn::RotateTurret(FVector LookAtTarget){								// LookAtTarget is an FVector corresponding to a point in the world
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();		//ToTarget is a world space direction vector, and we are taking the TurretMeshLocation in world space and substracting the look at location in world space. So "ToTarget" is a world space direction, and therefore the LookAtRotation is a world space rotation.
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);		//this is setting the rotation to the given xyz values, similar to SetActorRotation. It's calling one of the FRotator Constructor where it set the FRotator pitch yaw and roll value to the values given in the bracket. We did this so that the turret only rotates its yaw position and not the pitch and roll, otherwise it will look at the cursor directly causing the turret to look down to the floor as the GetHitResultUnderCursor() will take into account the impact and collision so the cursor will endup laying on the floor or on top of an object with collision. Thus, if we set the turret rotation to follow the cursor, the turret will face the ground or the up depending on the cursor's location... we don't want that! we want the turret to always be parrallel to the ground, that's why we set the pitch and roll to be 0 and only allow the Yaw(z axis rotation) to change.
	TurretMesh->SetWorldRotation(LookAtRotation);								//we use SetWorldRotation and not the SetActorRotation is because we have a rotation in world space! we are rotation the turret in world space (based on the location of the mouse which is located in the world) and not the local point.
	}

void ABasePawn::Fire(){

	FVector SpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();		//projectile is a component not an actor! so therefore its GetComponentLocation instead of GetActorLocation.
	FRotator SpawnPointRotation = ProjectileSpawnPoint ->GetComponentRotation();

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnPointLocation, SpawnPointRotation);			//if you hover into SpawnActor() it says that it return type is the parent class of the type, in this case it returns AProjectile* (a pointer). Why we need to store it in Projectile variable? bcs we wanted to make sure that its owner is set to the pawn that spawns it. "ProjectileClass" is the blueprint version of projectile.cpp! we want the mesh of the projectile and therefore we need to access the blueprint instances that we created out of projectile class, and this can be done by using TSubClassOF in BasePawn.h. After this, we can now open the blueprint instance of projectile.cpp and set the blueprint version of it as the "ProjectileClass" in the details tab.
	Projectile->SetOwner(this); //making sure that the owner is set to the pawn that spawns the projectile. so whenever we call "GetOwner" to the projectile, we are sure that it must be talking about the class that calls that projectile. if Tank called projectile then GetOwner will be ATank, whereas if tower call the projectile, ATower will be the GetOwner.
		
		// DrawDebugSphere(
		// 	GetWorld(),
		// 	SpawnPointLocation,                              //the center of the sphere
		// 	30.f,                                             //radius of the sphere
		// 	12,                                                //number of segments
		// 	FColor::Red,    
		// 	false,
		// 	3.f
		// );
}