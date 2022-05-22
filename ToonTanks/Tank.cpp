// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"



ATank::ATank()              //this is a constructor! need to initiate this in the Tank.h public section
{
    Arm =  CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    Arm -> SetupAttachment(RootComponent);
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera -> SetupAttachment(Arm);
    
	MoveTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MoveTrail"));
	MoveTrail -> SetupAttachment(RootComponent);

    WheelSound = CreateDefaultSubobject<UAudioComponent>(TEXT("WheelSound"));

    TokenReceivedSound = CreateDefaultSubobject<UAudioComponent>(TEXT("TokenReceivedSound"));
    TokenFinishSound = CreateDefaultSubobject<UAudioComponent>(TEXT("TokenFinishSound"));

    

}

// Called when the game starts or when spawned
void ATank::BeginPlay()     //if want to assign playercontroller(controller which possess the pawn) need to be at begin play! bcs at the constructor it is too early! no controller has been assign there just yet... so no GetOwner and no GetWorld just yet.
{
	Super::BeginPlay();
	PlayerControllerRef = Cast<APlayerController>(GetController());     //Essentialy we want to store the GetController() function inside the PlyerControlRef Variable, controller is the brain of a pawn/ actor that can possess a pawn to control it, it can be PlayerController (human) or AIController (AI). We want to do "PlayerControlRef = GetController()" but GetController() is the parent class of GetControllerRef and we cannot store the parent class in ITS CHILD'S Pointer, thus we need casting. The Cast change the one in the bracket data type (in this case GetController) to <APlayerController>
    // WheelSound->Play();
    // DrawDebugSphere(
    //     GetWorld(),
    //     GetActorLocation()+FVector(0,0,200.f),             //the center of the sphere plus an FVector with the length of 200 (+200 to the Z axis)
    //     100.f,                                             //radius of the sphere
    //     12,                                                //number of segments
    //     FColor::Red,    
    //     true,                                              //Persistent line
    //     30.f                                                    //lifetime, aka long it last? -1.0F means it last for a single frame, putting 30.f is so that we get the sphere for 30 seconds
    // );
}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FHitResult HitResult;       //this doesn't mean anything yet! just a declaration of an empty variable under the data type 'HitResult', later we will call the 'GetHitResultUnderCursor()' function to overwrite this empty variable into something more meaningful, itc getting the data of the linetrace (objectbeinghitbythecursor) to this HitResult varialbe.
    // UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), Speed);
    //to get the 'hit' under the cursor. This 'hit' can later be used for other usage.
    if(PlayerControllerRef){                        //null pointer prevention
        PlayerControllerRef->GetHitResultUnderCursor(           //what we want to do is actually GetController()->GetHitResultUnderCursor()? and since we cant call the child function out from a parent's class we then need to store GetController in the child's data type first? and to do that we need casting?
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult           //this "HitResult" will be filled in with data about the line trace. For example we can use HitResult.Actor to get the actor that was hit, or HitResult.ImpactPoint to get the point in space where we get the hit event. So the HitResult is what we will use to get information about the linetrace. Since this function takes in out paramter, we can overwrite the current HitResult that doesn't contain anything yet with this data about the line trace. How to know if this is an out paramter? just go to definition to the function and u'll see 'FHitResult& HitResult', the & means that it is an outparameter and it can overwrite that variable into something meaningful, itc overwriting the HitResult with the data gotten by the LineTrace.          
        );
    }
    RotateTurret(HitResult.ImpactPoint);        //we can access the RotateTurret function which suppose to be the function in BasePawn class bcs we set the function under the protected section which means any of the child's class will have access to those functions as well (same goes to variable). The HitResult.ImpactPoint is a vector! so we can use that directly... how to know? just hover to the function/variable!
    //the red dot for debug
    DrawDebugSphere(
        GetWorld(),
        HitResult.ImpactPoint,                              //the center of the sphere
        10.f,                                             //radius of the sphere
        12,                                                //number of segments
        FColor::Red,    
        false                                              //Persistent line
                                                                //lifetime is empty bcs by default it is set to -1.f! and  it will keep on showing bcs it's in tick component where the function is being called every frame... diffrnt from begin play where you need to specify how long it last...
    );

    //this part is to arrange the token for increase speed. we want to increase speed for 10sec after TokenReceived
    Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("TimeDifference: %f"), TimeDifference);
	UE_LOG(LogTemp, Warning, TEXT("Time: %f"), Time);
	UE_LOG(LogTemp, Warning, TEXT("TokenTime: %f"), GetTokenTime);
	UE_LOG(LogTemp, Warning, TEXT("TokenReceivedPlayed: %s"), bTokenReceivedPlayed? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("TokenFinishPlayed: %s"), bTokenFinishPlayed? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("TokenReceivd: %s"), bTokenReceived? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("SpeedEndTime: %f"), SpeedEndTime);

    TimeDifference = Time-GetTokenTime;
    if(bTokenReceived){
        Speed = 1500.f;
        if(TimeDifference<=SpeedEndTime){
            // UE_LOG(LogTemp, Warning, TEXT("TimeDifference: %f"), TimeDifference);
            // UE_LOG(LogTemp, Warning, TEXT("TokenTime: %f"), GetTokenTime);
            // UE_LOG(LogTemp, Warning, TEXT("Time: %f"), Time);

            if(bTokenReceivedPlayed==false){
                if(TokenReceivedSound){			//making sure the HitSound is assigned in the blueprint details tab (we set it to be EditAnywhere in the headers file), if we didn't it will result in null pointer and wo this if check it will crash unreal.
                    // UGameplayStatics::PlaySoundAtLocation(this, TokenReceivedSound, GetActorLocation());	//we put this in the begin play not that it is going to play when we hit the play button. since this is an actor and we only set this actor to existense when we fire the projectile, means that this BeginPlay will only get excecuted once the projectile is spawn and can be called many times not just during and one time like the normal begin play. PlaySoundAtLocation(World context object, What sound to play, where to play);
                    TokenReceivedSound->Activate();
                }
                bTokenReceivedPlayed = true;
                bTokenFinishPlayed = false;
            }
        }
        if(TimeDifference>SpeedEndTime){
                // UE_LOG(LogTemp, Warning, TEXT("TokenTime: %f"), GetTokenTime);
            TokenReceivedSound->Deactivate();
            if(bTokenFinishPlayed==false){
                if(TokenFinishSound){			//making sure the HitSound is assigned in the blueprint details tab (we set it to be EditAnywhere in the headers file), if we didn't it will result in null pointer and wo this if check it will crash unreal.
                    TokenFinishSound->Activate();
                    // UGameplayStatics::PlaySoundAtLocation(this, TokenFinishSound, GetActorLocation());	//we put this in the begin play not that it is going to play when we hit the play button. since this is an actor and we only set this actor to existense when we fire the projectile, means that this BeginPlay will only get excecuted once the projectile is spawn and can be called many times not just during and one time like the normal begin play. PlaySoundAtLocation(World context object, What sound to play, where to play);
                }
                bTokenFinishPlayed = true;
                bTokenReceivedPlayed = false;
            }
            
                SpeedEndTime = 0;
                bTokenReceived = false;
                
        }
    }
    else if(bTokenReceived == false){
        Speed = 800.f;
		GetTokenTime = GetWorld()->GetTimeSeconds();
    }
    					
	        // 	UE_LOG(LogTemp, Warning, TEXT("TokenTime: %f"), GetTokenTime);
			// FVector SpawnPointLocation = BasePawn->ProjectileSpawnPoint->GetComponentLocation();		//projectile is a component not an actor! so therefore its GetComponentLocation instead of GetActorLocation.
			// FRotator SpawnPointRotation = BasePawn->ProjectileSpawnPoint ->GetComponentRotation();
			// auto Gun = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnPointLocation, SpawnPointRotation);			//if you hover into SpawnActor() it says that it return type is the parent class of the type, in this case it returns AProjectile* (a pointer). Why we need to store it in Projectile variable? bcs we wanted to make sure that its owner is set to the pawn that spawns it. "ProjectileClass" is the blueprint version of projectile.cpp! we want the mesh of the projectile and therefore we need to access the blueprint instances that we created out of projectile class, and this can be done by using TSubClassOF in BasePawn.h. After this, we can now open the blueprint instance of projectile.cpp and set the blueprint version of it as the "ProjectileClass" in the details tab.
			// Gun->SetOwner(this); //making sure that the owner is set to the pawn that spawns the projectile. so whenever we call "GetOwner" to the projectile, we are sure that it must be talking about the class that calls that projectile. if Tank called projectile then GetOwner will be ATank, whereas if tower call the projectile, ATower will be the GetOwner.


}

void ATank::HandleDestruction(){    //a function to determine what happen when the pawn or tank dies?, this function is first initialize in the BasePawn as the parent of Tank and Tower since both pawn will experience HandleDestuction when it dies
    Super::HandleDestruction();   //this calls the HandleDestruction in the parent class (itc the BasePawn.cpp) and thus we can have the visual and sound effect.
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bTankAlive = false;
}   


// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent -> BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent -> BindAxis(TEXT("Turn"), this, &ATank::Turn);

    PlayerInputComponent -> BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);

}

void ATank::Move(float value){
    // UE_LOG(LogTemp, Warning, TEXT("%f"), value);
    FVector DeltaLocation(0.f); 

                                    /* DI FVector.h ada constructor:

                                        FVector::FVector(float InF)
                                        : X(InF), Y(InF), Z(InF)
                                    {
                                        DiagnosticCheckNaN();
                                    }

                                    means that inputing a single float value to FVector parameter will automatically assign 3 float values to the x y and z coordinate.
                                    => with constructor, a single datatype can have many execution possibilities depending on its parameter being assign in. different type, different number of parameter will link to different constructor and thus execute different set of code.
                                    Want to know just how many constructor a function have? just go to the definition of that functon (f12)

                                    */
    
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

    DeltaLocation.X = value*DeltaTime*Speed;           // Changing the deltalocation.X axis value to value*DeltaTime*Speed. since DeltaTime is the difference in time between each frame, it will have a really small and thus need to be times with "speed" to make it larger.
    // DeltaLocation.X = value;
    // UE_LOG(LogTemp, Warning, TEXT("%f"), DeltaLocation.X);
    if(value==0){
        MoveTrail->Deactivate();
        WheelSound->Deactivate();
        return;
    }
  
        MoveTrail->Activate();
        WheelSound->Activate();
        // WheelSound->Stop();

    
    // if(MoveTrail){		// Null pointer prevention, checking if we have set the  HitParticle in the blueprint. 
    //     UGameplayStatics::SpawnEmitterAtLocation(this, MoveTrail, GetActorLocation(), GetActorRotation());	//We put this right after we destroy the actor such coz it just doesn't make any sense to spawn the effects only right after the actor is destroyed. There are 3 options to fill in the parameter SpawnEmitterAtLocation(), hover over and click on the down arrow button to know what it is. The one we're using SpawnEmitterAtLocation(World context object, Particle system (we define this in the header file by the UparticleSystem class), Location, Rotation)
    // }
    // UE_LOG(LogTemp, Warning, TEXT("%f"), value);     //print the 1 1 1 1 1 1 of the input value
    AddActorLocalOffset(DeltaLocation, true);           // Setting the location to the new FVector, true to sweep means setting collision to true (if collision is off, this will be false even if this set to true). Since we set the Scale value for press 'W' button to 1, means it will set the "value" to 1 every frame. And we set DeltaTime to be value so it will +1 +1 +1 every frame thus our actor can move forwars.
}

void ATank::Turn(float Value){
    FRotator DeltaRotation = FRotator::ZeroRotator;              //this is similar to FRotator(0); in this case ZeroRotator is a static variable (ada di FRotator de definition!)
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaRotation.Yaw = Value*DeltaTime*TurnRate;               // use delta time so that the turning rate is framerate independent (aka regardless how fast or slow your computer is, the time will be the same since the one taken is the delta time which is the time difference between each frame)
    // UE_LOG(LogTemp, Warning, TEXT("%f"), DeltaRotation.Yaw);

    AddActorLocalRotation(DeltaRotation, true);                    //setting the rotation to the new FRotator

}