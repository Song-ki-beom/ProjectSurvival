// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4706)


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DestructibleMesh.h"
#include "CDestructibleStructures.generated.h"
/**
 * 
 */


USTRUCT(Atomic, BlueprintType)
struct FDestructibleStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
            UDestructibleMesh* DestructibleMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float MaxDamageThreshold;


};



