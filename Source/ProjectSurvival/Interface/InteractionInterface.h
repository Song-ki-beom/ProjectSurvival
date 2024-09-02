// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"


UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"), //플레이어 제외 NPC
	Device UMETA(DisplayName = "Device"), // Door , Window Interaction etc.
	Container UMETA(DisplayName = "Container"), // 별도의 인벤토리가 있는 아이템 
	Build UMETA(DisplayName = "Build") //플레이어가 월드에 설치한 아이템 
};


USTRUCT()
struct FInteractableData // 상호작용되는 액터 or Object에 대한 정보
{
	GENERATED_USTRUCT_BODY()
		//생성자 body 실행전에 아래와 같이 초기화 
		FInteractableData() : 
		InteractableType(EInteractableType::Pickup),
		Name(FText::GetEmpty()),
		Action(FText::GetEmpty()),
		Quantity(0),
		InteractionDuration(0.0f)
		{
		

		};

	//EditInstanceOnly : 해당 프로퍼티를 인스턴스 편집기에서만 수정할 수 있도록 제한할 수 있음, 에디터에서의 전역적 수정 불가능 
	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;

	UPROPERTY(EditInstanceOnly)
	int32 Id;

};


USTRUCT()
struct FInteractionData // 상호작용 자체에 대한 정보 
{
	GENERATED_USTRUCT_BODY()

		FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
		{
		}

	UPROPERTY()
	AActor* CurrentInteractable; //최근에 발견한 상호작용 가능 액터

	float LastInteractionCheckTime; // 인터렉션 검출 frequency 

};



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACSurvivor;
class PROJECTSURVIVAL_API IInteractionInterface // 언리얼 인터페이스는 Implementate 해야 할 것 중에 필요한 것만 선택적으로 찾아 상속 클래스에서의 구현이 가능하다.
{
	GENERATED_BODY()

	
public:
	virtual void BeginFocus()=0;
	virtual void EndFocus()=0;
	virtual void BeginInteract()=0; //인터렉션까지의 타이머 설정
	virtual void EndInteract()=0;
	virtual void Interact(ACSurvivor* PlayerCharacter)=0; // 인터렉트가 준비되면 바로 Interact 시작 

	FInteractableData InteractableData;





};
