// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCItemDragDropOperation : public UDragDropOperation //DragDropOperation = 사용자가 드래그를 시작할때 생성하는 드래그 작업 객체 , 드래그되고 있는 아이템이나 데이터와 관련된 정보를 담고 있음 
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		class UCItemBase* SourceItem;
	UPROPERTY()
		class UCInventoryComponent* SourceInventory;


};
