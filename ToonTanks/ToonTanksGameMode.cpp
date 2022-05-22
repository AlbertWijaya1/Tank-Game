// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor){

    if(DeadActor == Tank){        // Here, since we have set Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));, The tank will now store the PlayerPawn(which is the payer that posses our pawn on player 0) and thus can do the if checking if the DeadActor == Tank (that PlayerPawn)  if(ATank* DestroyedTower2 = Cast<ATank>(DeadActor)){
        Tank->HandleDestruction();                  //execute the HandleDestruction function in the Tank class. Actually this is doing GetPlayerPawn(this,0)->HandleDestruction? so get the pawn player 0 is possessing and execute the HandleDestruction function that is available under that pawn's datatype (in this case ATank, and we do have a HandleDestruction function in Tank.cpp class so that is the one going to be executed).

        if(ToonTanksPlayerController){        //        if(Tank->GetTankPlayerController()){


          ToonTanksPlayerController->SetPlayerEnabledState(false);  //SetPlayerEnabledState(bool bPlayerEnabled) is one of the function in ToonTanksPlayerController, and giving a false value as its paramter simply means we are accessing the "else" statement (check the function in ToonTanksPlayerController!) where we disable the input, and since we make "bShowMouseCursor = bPlayerEnabled", disabling the input will also mean we disable the cursor, coz of the = sign we set.                                                          // Tank->DisableInput(Tank->GetTankPlayerController());                       //Disable input requires a PlayerController as its parameter and so we put in what the GetPlayerController returns, itc, the PlayerController of the tank.        
                                                                    // Tank->GetTankPlayerController()->bShowMouseCursor = false;                  //disable the mouse cursor such that it is not shown. This line is replaced by: " ToonTanksPlayerController->SetPlayerEnabledState(false);" since by giving a "false" as its boolean parameter, it already includes disabling the cursor and input!, so more efficient...
        }
        GameOver(false);
    }

    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor)){          //Check if The one thats dead (DeadActor) is an a Tower? if the one dead is a Tower, means it will return ATower bcs Tower's data type is ATower, which mean that the cast will return true! since DeadActor will then be a tower which will have the class ATower and thus suffice the cast where we set DestroyTower dataType to be an ATank, since DeadTower is now also an ATank, DestroyTower=Cast<ATank>(DeadActor) will return true and thus execute the else if function. in other words, It checks ATower is the child of DeadActor? or if DeadActor is the parent of ATower? if this is true then execute the else if statement
        DestroyedTower->HandleDestruction();        //we want to do DeadActor -> HandleDestruction? but cant bcs its parent (AActor) ponting child (ATower which is a pawn, yes it is a pawn because it inherits from BasePawn)?
        TargetTower--;
        if(TargetTower==0){
            GameOver(true);
        }
    }

}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();                 //Super:: calls out all of the function and variables declared in its parent class.
	// ...
    HandleGameStart();
    
    UE_LOG(LogTemp, Warning, TEXT("Tower: %i"), GetTargetTowerCount());

}

void AToonTanksGameMode::HandleGameStart(){
	TargetTower = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0)); //set the player that possess our pawn to be player 0, this is true since our tank will always be player 0
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));     //Returns the player controller (the brain that possess our pawn)
    
    StartGame();    //we set this as UFUNCTION(BlueprintImplementableEvent), this means that we don't have to define the function here or anywhere in the cpp file bcs we are defining it in the BP! itc its in the ToonTanksGameMode_BP bcs that is where we are declaring the functin. Here we are just calling where we want the function to be located.

    if(ToonTanksPlayerController){
        ToonTanksPlayerController->SetPlayerEnabledState(false);            //initially, we set the input and mouse to false, then after 3 secs we set it to true.
        FTimerHandle PlayerEnableTimerHandle;

        FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);           //its a user input, but here we are using ToonTanksPlayerController instead of this like we used to bcs the callback function that we want to call is not in this class but on the ToonTanksPlayerController! and thus the respective class is being called instead. (Userinput, callbackfunction, what value want to pass in to the call back function? (in its parameter!)).
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, InputDelegate, StartDelay, false);     //Run InputDelegate after StartDelay duration, itc, the InputDelegate sets the SetPlayerEnabledState in the AToonTanksPlayerController to true (initially it was false, thats why we are unable to move within the first 3secs)
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount(){

    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);         //get how many actor are there in a class (in this case we choose ATower::StaticClass) and put them in an array, later on if we want to know how much  is the number of actor, can just use TArrayVar.Num() just like what we did in Bull and Cow game
    return Towers.Num();
}

