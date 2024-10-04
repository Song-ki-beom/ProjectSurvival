#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/Map/CPlayerLocation.h"
#include "Widget/Map/CRespawnLocation.h"
#include "CWorldMap.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCWorldMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void SetCharacterPosOnWorldMap();

public:
	void CreateSurvivorLocationOnWorldMap(const FText& InText, uint32 NetGUIDValue);
	void RefreshSurvivorLocationOnWorldMap(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue);
	FNetworkGUID GetPersonalGUID() { return PersonalNetGuid; }

	class ACSurvivor* GetPersonalSurvivor() { return PersonalSurvivor; }
	class ACSurvivorController* GetPersonalSurvivorController() { return PersonalSurvivorController; }

	void SetActorOnWorldMap(class AActor* InActor);
	void CreateRespawnLocationOnWorldMap(class AActor* InActor);
	void RefreshRespawnLocationOnWorldMap();

	TWeakObjectPtr<UCPlayerLocation> GetPlayerLocationPtr() { return PlayerLocationPtr; }

	void HideSurvivorLocationOnWorldMap(uint32 NetGUIDValue);
	void ShowSurvivorLocationOnWorldMap(uint32 NetGUIDValue);

private:
	void DisableNameTransmit() { bIsNameTransmitted = true; }

private:
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* WorldMapCanvasPanel;
	UPROPERTY(meta = (BindWidget))
		class UImage* WorldMap;

	float WorldMapLevelWidth = 41859.0f;
	float WorldMapLevelHeight = 39879.0f;
	FVector2D WorldMapLevelTopLeftLocation = FVector2D(15700.0f, -31483.0f);
	FVector2D ImageSize;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCPlayerLocation> PlayerLocationClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCRespawnLocation> RespawnLocationClass;

	UPROPERTY()
		TMap<uint32, TWeakObjectPtr<UCPlayerLocation>> PlayerLocationMap;

	TWeakObjectPtr<UCPlayerLocation> PlayerLocationPtr;

	int32 PersonalNetGuidValue;
	FNetworkGUID PersonalNetGuid;

	UPROPERTY()
		class ACSurvivor* PersonalSurvivor;

	UPROPERTY()
		class ACSurvivorController* PersonalSurvivorController;

	bool bIsNameTransmitted = false;

	UPROPERTY()
		TMap<class AActor*, TWeakObjectPtr<UCRespawnLocation>> RespawnLocationMap;
};
