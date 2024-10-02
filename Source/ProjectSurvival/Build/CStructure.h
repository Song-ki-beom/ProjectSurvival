#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "World/CPickup.h" 
#include "CStructure.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure : public ACPickUp
{
	GENERATED_BODY()
	
public:	
	ACStructure();
	UStaticMeshComponent* GetStaticMesh() { return PickupMesh; }
	UMaterialInterface* GetOriginMaterial() { return OriginMaterial; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void OpenActorInventory(const class ACSurvivor* Survivor, class AActor* Actor) override;

public:
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastDestroyPreviewBox();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override; //인터렉션까지의 타이머 설정
	virtual void EndInteract() override;
	//virtual void Interact(class ACSurvivor* PlayerCharacter) override; // 인터렉트가 준비되면 바로 Interact 시작 

	void SetBuildStructureID(FName InItemID) { BuildStructureID = InItemID; }
	FName GetBuildStructureID() { return BuildStructureID; }

	bool CheckIsBuildOnLandScape() { return bIsBuildOnLandScape; }
	bool CheckIsInteractableBuildTypeStructure() { return bIsInteractableBuildTypeStructure; }

protected:
	void SaveOriginMaterial();


protected:
	FName BuildStructureID;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* PreviewBox;
	UPROPERTY()
		UMaterialInterface* OriginMaterial;

	UPROPERTY(EditAnywhere)
		bool bIsBuildOnLandScape;

	bool bIsInteractableBuildTypeStructure = false;
};
