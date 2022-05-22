// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Tank.h"
#include "BasePawn.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = ProjectileMesh;	//RootComponent is already part of the actor class, if you ctrl+click on it it will open the actor.h file showing that it is declared with the USceneComponent data type.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle -> SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->InitialSpeed = 1300.f;
	ProjectileMovement->MaxSpeed = 1300.f;
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);			//OnComponentHit.AddDynamic() (built in UE function) is a HitEvent, which is a multitaskdelegate! which means it can store function and call them if the event occured (in this case, OnHit Function is being called On Component Hit(as the delegate name implies)) where OnHit is the function bound to the OnComponentHit Delegate
	if(HitSound){			//making sure the HitSound is assigned in the blueprint details tab (we set it to be EditAnywhere in the headers file), if we didn't it will result in null pointer and wo this if check it will crash unreal.
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());	//we put this in the begin play not that it is going to play when we hit the play button. since this is an actor and we only set this actor to existense when we fire the projectile, means that this BeginPlay will only get excecuted once the projectile is spawn and can be called many times not just during and one time like the normal begin play. PlaySoundAtLocation(World context object, What sound to play, where to play);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(LogTemp, Warning, TEXT("%f"), DamageAmount);
	if(Tank->bGunReceived && GetOwner() == Tank){
		DamageAmount = 50.f;
	}

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)			//when the projectile hit something		//we need this funciton as a function that is called when a hit event occured. Syntax: void var(the component doing the hitting(itc, the ProjectileMesh (a "UStaticMesh COMPONENT" that do all the hittings), actor that got hit, other component that was hit (e.g. static mesh component of other actor, itc other static mesh apart from our ProjectileMesh).. if we hit another UStaticMeshComponent of some other actor then this will be that StaticMeshComponent, If we simulating physics the physics engine will apply in response to the collision and this FVector represent the direction and rotation of that impulse, calling the hitresult to provide more information about the hit.)
{

	auto MyOwner = GetOwner();		//since we have set "Projectile->SetOwner(this)" in the fire function at BasePawn.cpp, whenever the Fire function is called, they will refer the GetOwner to the corresponding actor that spawns the pawn(projectile), if Tank spawn projectile then GetOwner is ATank, whereas if Tower spawns projectile, GetOwner() is ATower.
	if(MyOwner==nullptr){
		Destroy();				//making sure that there is no lingering object! so we destroy them then only we break the if function, say this if condidion gets executed (when MyOnwer==nullptr)
		return;
	}

	auto MyOwnerInstigator = MyOwner->GetInstigatorController();		//this is same as AController* MyOwnerInstigator = MyOwner->GetInstigatorController(); since GetInstigatorController returns a AController. Unreal kinda do it for u using 'auto'. This function returns an instigator controller to the owner (null if none). why need this? need to satisfy the parameter! instigator refers to the controller of the pawn(?)
	auto DamageTypeClass = UDamageType::StaticClass();					// if we just want a variable that returns a UClass pointer, i.e. UClass of any particular class to satisfy the input parameter number (cos if the funciton need n number of parameter and u only input 3, it will not be valid!). so if u just want to get away with using the function but not really want to use that specific parameter in the function, u need to have a variable that represent that class, and to do that u can use a function called "StaticClass". In this case, we set UDamageType::StaticClass() such that it will returns a UClass pointer which will represent the UDamageType cpp class.
	// if(ProjectileParticle){		// Null pointer prevention, checking if we have set the  HitParticle in the blueprint. 
	// 	UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileParticle, GetActorLocation(), GetActorRotation());	//We put this right after we destroy the actor such coz it just doesn't make any sense to spawn the effects only right after the actor is destroyed. There are 3 options to fill in the parameter SpawnEmitterAtLocation(), hover over and click on the down arrow button to know what it is. The one we're using SpawnEmitterAtLocation(World context object, Particle system (we define this in the header file by the UparticleSystem class), Location, Rotation)
	// }
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)				// this section is for when the projectile hit an actor (not only tank or tower! the floor, wall is also an actor what... so not exeptional to only the pawn, if want to do pawn need to do another if check!(if(ATower* HitTower = Cast<ATower>(OtherActor)){})). Also act as a null prevention for OtherActor, don't want to cause damage to ourselves if we are the projectile (don't wanna cause damage to the projectile), don't wannt cause damage to GetOwner(aka the actor spawning the pawn(projectile), it can either be Tank or Tower).
	{
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, MyOwnerInstigator, this, DamageTypeClass);	//ApplyDamage(DamagedActor, DamageAmount, Instigator(aka the controller that caused the damage (Acontroller*)), DamageCauser(always be 'this'?), DamageType)
		if(HitParticles){		// Null pointer prevention, checking if we have set the  HitParticle in the blueprint. 
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());	//We put this right after we destroy the actor such coz it just doesn't make any sense to spawn the effects only right after the actor is destroyed. There are 3 options to fill in the parameter SpawnEmitterAtLocation(), hover over and click on the down arrow button to know what it is. The one we're using SpawnEmitterAtLocation(World context object, Particle system (we define this in the header file by the UparticleSystem class), Location, Rotation)
		}
		if(ABasePawn* HitPawn = Cast<ABasePawn>(OtherActor)){
												//to distinguish! check if the one got hit is a pawn. if want to have action to only occur if a projectile hits a pawn, put it under this if statement. if want to do Tank only or Tower only, simply change ABasePawn to ATank or ATower.8888
				if(HitPawnParticles){		// Null pointer prevention, checking if we have set the  HitParticle in the blueprint. 
					UGameplayStatics::SpawnEmitterAtLocation(this, HitPawnParticles, GetActorLocation(), GetActorRotation());	//We put this right after we destroy the actor such coz it just doesn't make any sense to spawn the effects only right after the actor is destroyed. There are 3 options to fill in the parameter SpawnEmitterAtLocation(), hover over and click on the down arrow button to know what it is. The one we're using SpawnEmitterAtLocation(World context object, Particle system (we define this in the header file by the UparticleSystem class), Location, Rotation)
				}
		}	

		if(Tank->bGunReceived && GetOwner()==Tank){
			if(HitParticles2){		// Null pointer prevention, checking if we have set the  HitParticle in the blueprint. 
				UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles2, GetActorLocation(), GetActorRotation());	//We put this right after we destroy the actor such coz it just doesn't make any sense to spawn the effects only right after the actor is destroyed. There are 3 options to fill in the parameter SpawnEmitterAtLocation(), hover over and click on the down arrow button to know what it is. The one we're using SpawnEmitterAtLocation(World context object, Particle system (we define this in the header file by the UparticleSystem class), Location, Rotation)
				
			}
		}

		if(HitSound){			//making sure the HitSound is assigned in the blueprint details tab (we set it to be EditAnywhere in the headers file), if we didn't it will result in null pointer and wo this if check it will crash unreal.
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation()); //PlaySoundAtLocation(World context object, What sound to play, where to play);
		}

		if(HitCameraShakeClass){	//Null prevention, making sure we a class assigned to the HitCameraShake. the class must be a Martinee Camera Shake and we can set the ocillation stregth from there. Since we can choose what class we want to put in as the "HitCameraShake", we can basically set the stregth as we want depeding on the condition. e.g. when a pawn dies (this is what we did here!), shake harder, when just a simple projectile hitting something shake softer.
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);		//The ClientStartCameraShake() function belongs to the player controller and we are now in projectile.cpp so to access the playercontroller, we first need to GetFirstPlayerController() and this exist on UWorld class, thus the GetWorld()->GetFirstPlayerController() before the desired function.
		}
		Destroy();		//we put it here because we wanted to destroy the projectile actor! if we put the destroy() in the fire in the basepawn.cpp, it will destroy the pawn instead of the projectile...
	}
	// UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	// UE_LOG(LogTemp, Warning, TEXT("HitComp: %s"), *HitComp->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherActor->GetName());
	Destroy(); 	// destroy any of the projectiles if none of the condition above is satisfied. so that we dont have any lingering object for no reason.
}
