// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DestructibleMesh.h"
#include "DestructibleStruct.generated.h"
/**
 * 
 */
class PROJECTSURVIVAL_API DestructibleStruct
{	
public:
	DestructibleStruct();
	~DestructibleStruct();
};


USTRUCT(Atomic, BlueprintType)
struct FDestructibleStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
            UDestructibleMesh* DestructibleMesh;
    




};