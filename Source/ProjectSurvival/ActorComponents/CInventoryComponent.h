// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/CItemDataStructures.h"
#include "Engine/World.h"
#include "CInventoryComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	NoItemAdded UMETA(DisplayName = "가져온 아이템이 없음"),
	PartialItemAdded UMETA(DisplayName = "일부분 가져옴 "),
	AllItemAdded UMETA(DisplayName = "아이템 개수 전부를 가져옴")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

		//생성자 변수 초기화 
		FItemAddResult() :
		ActualAmountAdded(0)
		, OperationResult(EItemAddResult::NoItemAdded)
		, ResultMessage(FText::GetEmpty()) {};

	UPROPERTY(BlueprintReadOnly , Category = "Item Read Result")
	int32 ActualAmountAdded; //실제 Item 의 증감된 량 
	UPROPERTY(BlueprintReadOnly, Category = "Item Read Result")
	EItemAddResult OperationResult; //Target 로부터 가져온 Quantity 상태를 나타내는 Enum
	UPROPERTY(BlueprintReadOnly, Category = "Item Read Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& Message)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::NoItemAdded;
		AddedNoneResult.ResultMessage = Message;
		return AddedNoneResult;
	};

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& Message) 
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::PartialItemAdded;
		AddedPartialResult.ResultMessage = Message;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AllAmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AllAmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(class UCItemBase* InItem); //추가하려는 Item 에 대한 처리 


	//Toggle Menu
	//void ToggleMenu();
	void DropItem(class UCItemBase* ItemToDrop, const int32 QuantityToDrop);
	void SwapItem(class UCItemBase* ItemOnBase, class UCItemBase* ItemFromDrag);
	bool CombineItem(class UCItemBase* ItemOnBase, class UCItemBase* ItemFromDrag);
	//Find

	//인벤토리에서 동일한 UCItemBase 포인터(데이터 저장소)가 존재하는지 확인
	UFUNCTION(Category = "Inventory")
	 UCItemBase* FindMatchingItem( UCItemBase* ItemIn) const; 
	UFUNCTION(Category = "Inventory")
	 UCItemBase* FindNextItemByID( UCItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	 UCItemBase* FindNextPartialStack( UCItemBase* ItemIn) const;
	
	 
	 //Remove
	UFUNCTION(Category = "Inventory")
	void RemoveSingleItem( UCItemBase* ItemIn); //아이템 자체 삭제
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UCItemBase* ItemIn, int32 DesiredAmountToRemove); //아이템 Quanity 일부 덜어내기
	UFUNCTION(Category = "Inventory")
	bool SplitExistingStack(UCItemBase* ItemIn, const int32 AmountToSplit); //ex) 6개 에서 3 Quantity 아이템 두개로 만들기 

	//Sort
	UFUNCTION(Category = "Inventory")
	void SortInventory();


	//Get
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; }; //현재 소지 용량 
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; }; //최대 용량
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; }; //슬롯 개수
	
	//내용물 호출 
	FORCEINLINE TArray<TWeakObjectPtr<UCItemBase>> GetInventoryContents() { return InventoryContents; };

	//Set

	//슬롯 개수를 변경할때 호출 
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };
	//최대 무게 변경 
	FORCEINLINE void SetWeightCapacity(const int32 NewWeightCapacity) {
		InventoryWeightCapacity = NewWeightCapacity;
	};
	//현재 무게 변경  
	FORCEINLINE void SetInventoryTotalWeight(const float NewTotalWeight) {
		InventoryTotalWeight = NewTotalWeight;
	};

	FORCEINLINE class ACMainHUD* GetHUDReference() const { return HUD;}; //현재 소지 용량 


protected:
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UCItemBase* ItemIn, int32 RequestedAddAmount);
	int32 HandleStackableItems(UCItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UCItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UCItemBase* StackableItem, int32 InitialRequestedAddAmount); //요청한 InitialRequestedAddAmount 에서 허용 가능한 용량을 새로 계산함 

	void AddNewItem(UCItemBase* InItem, const int32 AmountToAdd);

	void PerformDropItem(const  FTransform SpawnTransform, FName ItemID, const int32 RemovedQuantity);
	UFUNCTION(Server, Reliable)
	void RequestDropItem(const  FTransform SpawnTransform, FName ItemID, const int32 RemovedQuantity);
	int32 FindItemIndex(UCItemBase* Item);


private:
	void Merge(TArray<TWeakObjectPtr<UCItemBase>>& Array, int Left, int Mid, int Right);
	void MergeSort(TArray<TWeakObjectPtr<UCItemBase>>& Array, int Left, int Right);



//인벤토리에 변경사항이 있을때마다 호출할 델리게이트 함수 
public:
	FOnInventoryUpdated OnInventoryUpdated;

protected:
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	float InventoryWeightCapacity;

	//언리얼 4는 TObjectPtr을 지원하지 않으므로, UPROPERTY()와  TWeakObjectPtr을 통해 GC의 의해 삭제된 곳도 안전하게 포인터 접근이 가능하다.
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TWeakObjectPtr<UCItemBase>> InventoryContents;
	
private:
	class ACharacter* OwnerCharacter;
	UPROPERTY(Replicated)
		class ACMainHUD* HUD;
};
