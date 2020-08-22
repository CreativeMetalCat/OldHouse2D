// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorButton.h"

void ADoorButton::ToggleDoors()
{
    if(Doors.Num()>0)
    {
        for(int i=0;i<Doors.Num();i++)
        {
            Doors[i]->Toggle();
        }
    }
}

void ADoorButton::OnButtonPressed()
{
    Super::OnButtonPressed();
    ToggleDoors();
}

void ADoorButton::OnButtonUnPressed()
{
    Super::OnButtonUnPressed();
    ToggleDoors();
}
