#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Build/CBuildStructure.h"
#include "CBuildComponent.generated.h"

USTRUCT(BlueprintType)
struct FBuildStructureInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* BuildStructureTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class TSubclassOf<ACBuildStructure> BuildStructureClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBuildComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ToggleBuildWidget();
	void SelectQ();
	void SelectW();
	void SelectE();
	void SelectA();
	void SelectS();
	void SelectD();
	void SelectZ();
	void SelectX();
	void SelectC();

private:
	bool bIsBuildWidgetOn;
	UPROPERTY()
		class UCBuildWidget* BuildWidget;

	class UDataTable* BuildStructureData;
};
