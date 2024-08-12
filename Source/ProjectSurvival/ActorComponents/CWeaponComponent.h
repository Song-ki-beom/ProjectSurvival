// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Axe, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];


public: 
	EWeaponType  GetWeaponType() { return Type; }

	void SetUnarmedMode();
	void SetAxeMode();

	bool IsUnarmedMode();
	bool IsAxeMode(); 
	bool IsIdleMode();

	void DoAction();

	void DestroyWeapon();


private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();

public:
	FWeaponTypeChanged  OnWeaponTypeChanged;

private: 
	class ACharacter* OwnerCharacter;
	EWeaponType Type = EWeaponType::Max;

private:
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];

};
