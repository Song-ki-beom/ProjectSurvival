// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "CPickUp.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACPickUp : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACPickUp();



public:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus()override;
	virtual void BeginInteract() override; //인터렉션까지의 타이머 설정
	virtual void EndInteract() override;

protected:
	virtual void BeginPlay() override;
	virtual void Interact(class UCInteractionComponent* InteractComponent) override; // 인터렉트가 준비되면 바로 Interact 시작 

	//플레이어의 인벤토리와 상호작용하는 함수 
	void TakePickup(const class ACSurvivor* Taker);

	//Pickup은 월드에 등록된 데이터베이스에서 PickUp 데이터의 참조가 가능함 
	void InitializePickup(const TSubclassOf<class UItemBase> BaseClass , const int32 InQuantity);
	//반면 아이템을 Drop 할 때는 에디터와 데이터테이블에 등록이 되어 있지 않아 별도의 함수로 처리를 해 주어야 함
	void InitializeDrop(class UItemBase* ItemToDrop, const int32 InQuantity);


	

public:
	UPROPERTY(VisibleAnywhere , Category = "Pickup")
	UStaticMeshComponent* PickupMesh;
	
	
	//DataTable은 처음 시작하고 게임이 Start 될 때만 초기화 된다.
	UPROPERTY(EditInstanceOnly, Category = "Pickup")
	class UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup")
	FName DesiredItemID; // ItemDataTable Row Search에 쓰이는 키


	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	class UCItemBase* ItemReference; //참조된 아이템 데이터, 값을 에디터 내에 수정할수 있게 VisibleAnywhere로..(포인터)

	UPROPERTY(EditInstanceOnly, Category = "Pickup")
	int32 ItemQuantity;


	UPROPERTY(VisibleInstanceOnly, Category = "Pickup")
	FInteractableData InsatanceInteractableData;

protected:

	FORCEINLINE class UCItemBase* GetItemData() { return ItemReference; };


};
