// Fill out your copyright notice in the Description page of Project Settings.


#include "Speedometer.h"
#include "Kismet/GameplayStatics.h"
#include "BasePawn.h"
#include "Tank.h"
#include "Projectile.h"
#include "Components/AudioComponent.h"



// Sets default values
ASpeedometer::ASpeedometer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpeedometerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Speedometer"));
	RootComponent = SpeedometerMesh;	//RootComponent is already part of the actor class, if you ctrl+click on it it will open the actor.h file showing that it is declared with the USceneComponent data type.
}

// Called when the game starts or when spawned
void ASpeedometer::BeginPlay()
{
	Super::BeginPlay();
	BasePawn = Cast<ABasePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	SpeedometerMesh->OnComponentHit.AddDynamic(this, &ASpeedometer::OnHit);
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	
}

// Called every frame
void ASpeedometer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = FRotator::ZeroRotator;              //this is similar to FRotator(0); in this case ZeroRotator is a static variable (ada di FRotator de definition!)
    float DeltaTime3 = UGameplayStatics::GetWorldDeltaSeconds(this);
    Rotation.Yaw = 1*DeltaTime3*200.f;  
	AddActorLocalRotation(Rotation, true);
	// UE_LOG(LogTemp, Warning, TEXT("Time: %f"), Time);

}

void ASpeedometer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){

	if(ABasePawn* HitPawn = Cast<ABasePawn>(OtherActor)){								//to distinguish! check if the one got hit is a pawn. if want to have action to only occur if a projectile hits a pawn, put it under this if statement. if want to do Tank only or Tower only, simply change ABasePawn to ATank or ATower.
		Tank->bTokenReceived = true;
		Destroy();
		Tank->SpeedEndTime+=10;
		return;
	}
}

//float FloatNumb = 35.234;
//  int32 IntNumb = (int32)FloatNumb;
//if(IntNumb%10 != 10){}				For non-multipication of 10
//if(IntNumb%10 == 10){}				For Multiplication of 10

