// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CLoadingScreenManager.h"
#include "CGameInstance.h"


void ACLoadingScreenManager::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;

   
}

void ACLoadingScreenManager::Tick(float DeltaSeconds)
{

    if (!bLoadStarted)
    {
        bLoadStarted = true;
        GameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
        if (GameInstance)
            GameInstance->CreateLoadingScreen();
    }

}
