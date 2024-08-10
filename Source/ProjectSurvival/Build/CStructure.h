#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CStructure.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	ACStructure();
	UStaticMeshComponent* GetStaticMesh() { return StaticMesh; }
	UMaterialInterface* GetOriginMaterial() { return OriginMaterial; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	void SaveOriginMaterial();

protected:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* StaticMesh;

	UMaterialInterface* OriginMaterial;

};
