// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
// #include "GameFramework/Pawn.h"              // just such that the red swigglies at GetPawn() is gone, but wont compile error if not included as well!

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)         //this funciton is being called in the ToonTanksGameMode.cpp!
{
    if(bPlayerEnabled){                 //if 'true' is the parameter
        GetPawn()->EnableInput(this);   //GetPawn enables us to access the pawn that the PlayerController (because we're ina playercontroller (that is what we created in the first place when we click on the create cpp file)) is possessing. GetPawn returns an APawn and the pawn class has the function Enable/ Disable input(APlayerController* PlayerController). 
    }
    else                                // if 'false' is the parameter
    {
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnabled;      //setting the mouse cursor bool condidtion to be equal to that of playerinput, such that if player input is disabled means mouse cursor also disabled!
}