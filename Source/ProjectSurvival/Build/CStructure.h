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
	UStaticMeshComponent* GetStaticMesh() { return StaticMesh; }
	UMaterialInterface* GetOriginMaterial() { return OriginMaterial; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastDestroyPreviewBox();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override; //인터렉션까지의 타이머 설정
	virtual void EndInteract() override;
	virtual void Interact(class ACSurvivor* PlayerCharacter) override; // 인터렉트가 준비되면 바로 Interact 시작 

protected:
	void SaveOriginMaterial();

protected:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* PreviewBox;

	UMaterialInterface* OriginMaterial;
};
