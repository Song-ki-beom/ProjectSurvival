#pragma once

#include "Build/CStructure.h"
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "BuildStructureDataType.generated.h"

UENUM()
enum class ESelectedStructure : uint8
{
	Q, W, E, A, S, D, Z, X, C
};

USTRUCT(BlueprintType)
struct FBuildStructureInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* StructureTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACStructure> StructureClass;
};