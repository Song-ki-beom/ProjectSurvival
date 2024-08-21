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
	virtual void BeginFocus() override;
	virtual void EndFocus()override;
	virtual void BeginInteract() override; //인터렉션까지의 타이머 설정
	virtual void EndInteract() override;
protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR //WITH_EDITOR: 에디터에서 실행하는 코드의 경우 매크로 설정 
	// PostEditChangeProperty = 인게임에서 Property가 Change 되었을때, 변경사항을 참조해주는 언리얼 Built In 함수 
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 
	virtual void Interact(class ACSurvivor* PlayerCharacter) override; // 인터렉트가 준비되면 바로 Interact 시작 
	
	//플레이어의 인벤토리와 상호작용하는 함수 
	void TakePickup(const class ACSurvivor* Taker);

	//Pickup은 월드에 등록된 데이터베이스에서 PickUp 데이터의 참조가 가능함 
	void InitializePickup(const TSubclassOf<class UCItemBase> BaseClass , const int32 InQuantity);
	//반면 아이템을 Drop 할 때는 에디터와 데이터테이블에 등록이 되어 있지 않아 별도의 함수로 처리를 해 주어야 함
	void InitializeDrop(class UCItemBase* ItemToDrop, const int32 InQuantity);

	void UpdateInteractableData(); //PickUp 의 Quantity 일부만 가져갈 때의 상황을 가정하면 매번 데이터 업데이트 함수가 필요함 
	

	

public:
	//DataTable은 처음 시작하고 게임이 Start 될 때만 초기화 된다.
	UPROPERTY(EditInstanceOnly, Category = "PickupInitialization")
	class UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "PickupInitialization")
	FName DesiredItemID; // ItemDataTable Row Search에 쓰이는 키

	UPROPERTY(EditInstanceOnly, Category = "PickupInitialization")
		int32 ItemQuantity;


	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	class UCItemBase* ItemReference; //참조된 아이템 데이터, 값을 에디터 내에 수정할수 있게 VisibleAnywhere로..(포인터) , PickUp, Drop 에 쓰이는 중간 매개체

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup")
	FInteractableData InstanceInteractableData;

protected:

	FORCEINLINE class UCItemBase* GetItemData() { return ItemReference; };


};
