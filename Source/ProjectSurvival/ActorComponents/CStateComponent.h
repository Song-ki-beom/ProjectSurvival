// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
    Idle, Equip, Hit, Dead, Action, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSURVIVAL_API UCStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCStateComponent();

protected:
    virtual void BeginPlay() override;

public:
    void ChangeType(EStateType InType);

private:
    void SetIdleMode();
    void SetEquipMode();
    void SetHitMode();
    void SetActionMode();
    void SetDeadMode();

public:
    FStateTypeChanged OnStateTypeChanged;

private:
    class ACharacter* OwnerCharacter;
    EStateType  Type = EStateType::Idle;
    bool        bInSubActionMode = false;


public:
    FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
    FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
    FORCEINLINE bool IsHitMode() { return Type == EStateType::Hit; }
    FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
    FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
    FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }


};
