// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"           //thus we can also access the function and variable created in the tank.h class!
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ATower::BeginPlay()     //if want to assign playercontroller(controller which possess the pawn) need to be at begin play! bcs at the constructor it is too early! no controller has been assign there just yet... so no GetOwner and no GetWorld just yet.
{
    Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));    //(worldcontextobject, which player (on auto possess player)). need casting! cos we actually need "Tank = UGamePlayStatics::GetPlayerPawn(this, 0)"
    // UE_LOG(LogTemp, Warning, TEXT("Distance"));
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true, FireRate);    //(FTimerHandle, UserClass (The class we are currently working in), callback function, how much time before our callback get called again, loop?). We call this in the begin play so that the timer will start as soon as we hit the play button (?)
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());           //finding the distance between two vector points (itc, between the tank and the tower)
        if(InFireRange()){                         //fire range is set to be a fixed float number
            RotateTurret(Tank->GetActorLocation());     //the rotate turret function is used for both the tank and the tower (basepawn's classes), thus the function is declared at the parent's header and cpp file instead.
        }
        // UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance)

    // UE_LOG(LogTemp, Warning, TEXT("Distance"));

}

void ATower::HandleDestruction(){        //a function to determine what happen when the pawn or tank dies?, this function is first initialize in the BasePawn as the parent of Tank and Tower since both pawn will experience HandleDestuction when it dies
    Super::HandleDestruction();       //this calls the HandleDestruction in the parent class (itc the BasePawn.cpp) and thus we can have the visual and sound effect.
    Destroy();                       //destroy the actor
}


void ATower::CheckFireCondition(){          //due to the GetWorldTimerManager().SetTimer() function being called in the beginplay, this function will only be called every "FireRate" seconds.

    // float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());           //finding the distance between two vector points (itc, between the tank and the tower)
    if(InFireRange() && Tank-> bTankAlive){      //check if Tank is in tower's fire range and check if bTankAlive is true at that moment. If we just but a bool function in it without putting == true/ false means that we want it to be true! we are accessing Tanks's class in this case thats why we need to call the bool variable in here using pointers: Tank-> bTankAlive.
        Fire();                             //the Fire function is used for both the tank and the tower (basepawn's classes), thus the function is declared at the parent's header and cpp file instead.
    }

}

bool ATower::InFireRange(){

    if(Tank){        //null pointer prevention
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());           //finding the distance between two vector points (itc, between the tank and the tower)
        return Distance<FireRange;                  //if the this returns true, it will then exit the if function without executing the other code beneath it. Thats why we still need a "return false" statement outside the if function as a bool function must return something (not like a void!)
    }
    return false;
}