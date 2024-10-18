// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	StonePick, StoneAxe, WoodClub, WoodSpear, Bow, IronPick, IronAxe, IronSpear, IronPolearm, CrossBow, Axe, Max,
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
	bool IsBowMode();
	void DoAction();

	UFUNCTION(BlueprintCallable) void SubAction_Pressed();
	UFUNCTION(BlueprintCallable) void SubAction_Released();


	void DestroyWeapon();


	void SetMode(EWeaponType InNewType);
	UFUNCTION(Server, Reliable)
		void RequestSetMode(EWeaponType InType);

	void SetUsingWeapon(class UCItemBase* InItem, class UCInventoryItemSlot* InItemSlot)
	{
		UsingWeapon = InItem;
		UsingWeaponSlot = InItemSlot;
	}
	class UCItemBase* GetUsingWeapon() { return UsingWeapon; }
	class UCInventoryItemSlot* GetUsingWeaponSlot() { return UsingWeaponSlot; }

private:
	void SetModeReplicate();
	void ChangeType(EWeaponType InType);
	UFUNCTION(NetMulticast , Reliable)
		void Broadcast_Equip(const FString& InName);
	UFUNCTION(Server, Reliable)
		void RequestDoAction();
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastPlayDoAction();
	UFUNCTION(Server, Reliable)
		void RequestPressSubAction();
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastPressSubAction();
	UFUNCTION(Server, Reliable)
		void RequestReleaseSubAction();
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastReleaseSubAction();



	UFUNCTION()
		void OnRef_PrevTypeChanged();
	UFUNCTION()
		void OnRef_TypeChanged();

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();
public:
	FWeaponTypeChanged  OnWeaponTypeChanged;

private:  
	UPROPERTY()
		class ACharacter* OwnerCharacter;
	UPROPERTY(VisibleAnyWhere, Replicated , ReplicatedUsing = OnRef_PrevTypeChanged)
	EWeaponType PrevType = EWeaponType::Max;
	UPROPERTY(VisibleAnyWhere,Replicated , ReplicatedUsing = OnRef_TypeChanged)
	EWeaponType Type = EWeaponType::Max;
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];

	UPROPERTY()
		class UCItemBase* UsingWeapon;
	UPROPERTY()
		class UCInventoryItemSlot* UsingWeaponSlot;

};
