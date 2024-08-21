// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

//UENUM(BlueprintType)
//enum class EItemAddResult : uint8
//{
//	NoItemAdded UMETA(DisplayName = "가져온 아이템이 없음"),
//	PartialItemAdded UMETA(DisplayName = "일부분 가져옴 "),
//	AllItemAdded UMETA(DisplayName = "아이템 개수 전부를 가져옴")
//};
//
//USTRUCT(BlueprintType)
//struct FItemAddResult
//{
//	GENERATED_BODY()
//
//	FItemAddResult(): ActualAmountAdded;
//
//
//
//	int32 ActualAmountAdded;
//	EItemAddResult OperationResult;
//	FText ResultMessage;
//
//
//};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
