// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "CLoadingScreenManager.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACLoadingScreenManager : public AActor
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    void Tick(float DeltaSeconds) override;



private:
    class UCGameInstance* GameInstance;
    bool bLoadStarted = false;
};
