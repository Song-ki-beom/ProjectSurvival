#include "Widget/Map/CWorldMap.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Character/CSurvivor.h"
#include "Engine/PackageMapClient.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

void UCWorldMap::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetWorld()->GetNetDriver() && GetWorld()->GetNetDriver()->GuidCache)
	{
		FNetworkGUID playerGUID = GetWorld()->GetNetDriver()->GuidCache->GetOrAssignNetGUID(this->GetOwningPlayerPawn());
		if (playerGUID.IsValid())
		{
			PersonalNetGuid = playerGUID;
			PersonalNetGuidValue = playerGUID.Value;
		}
	}

	// 위치정보 보내기
	FTimerHandle worldMapUpdateTimer;
	GetWorld()->GetTimerManager().SetTimer(worldMapUpdateTimer, this, &UCWorldMap::SetCharacterPosOnWorldMap, 0.025f, true, 3.0f);

	// 이름 보내기 중단 (이미 정해져서 업데이트 필요x)
	FTimerHandle nameTransmitTimer;
	GetWorld()->GetTimerManager().SetTimer(nameTransmitTimer, this, &UCWorldMap::DisableNameTransmit, 30.0f, false);
}

void UCWorldMap::SetCharacterPosOnWorldMap()
{
	// 캐릭터의 실제 위치를 WorldMap 텍스쳐 이미지 위치로 변환 (좌상단 0,0 기준)
	FTransform ownerTransform = this->GetOwningPlayerPawn()->GetActorTransform();
	float ownerLocationX = ownerTransform.GetLocation().X;
	float ownerLocationY = ownerTransform.GetLocation().Y;
	float ownerRotationZ = ownerTransform.GetRotation().Rotator().Yaw;

	if (UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(WorldMap->Slot))
		ImageSize = canvasPanelSlot->GetSize();

	float translationX = (ownerLocationY - WorldMapLevelTopLeftLocation.Y) / WorldMapLevelWidth * ImageSize.X;
	float translationY = (WorldMapLevelTopLeftLocation.X - ownerLocationX) / WorldMapLevelHeight * ImageSize.Y;

	if (this->GetOwningPlayer()->HasAuthority())
	{
		if (GetWorld()->GetNetDriver() && GetWorld()->GetNetDriver()->GuidCache)
		{
			FNetworkGUID playerGUID = GetWorld()->GetNetDriver()->GuidCache->GetOrAssignNetGUID(this->GetOwningPlayerPawn());
			if (playerGUID.IsValid())
			{
				FText survivorName = FText::GetEmpty();
				UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
				if (gameInstance)
					survivorName = gameInstance->GetLobbySurvivorName();

				int32 playerGUIDValue = playerGUID.Value;
				ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
				if (survivor)
				{
					if (!bIsNameTransmitted)
					{
						survivor->BroadcastSetName(survivorName, playerGUIDValue);
					}
					survivor->BroadcastSetLocation(translationX, translationY, ownerRotationZ, playerGUIDValue);
				}
			}
		}
	}
	else
	{
		if (GetWorld()->GetNetDriver() && GetWorld()->GetNetDriver()->GuidCache)
		{
			FNetworkGUID playerGUID = GetWorld()->GetNetDriver()->GuidCache->GetOrAssignNetGUID(this->GetOwningPlayerPawn());
			if (playerGUID.IsValid())
			{
				FText survivorName = FText::GetEmpty();
				UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
				if (gameInstance)
					survivorName = gameInstance->GetLobbySurvivorName();

				int32 playerGUIDValue = playerGUID.Value;
				ACSurvivor* survivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
				if (survivor)
				{
					if (!bIsNameTransmitted)
					{
						survivor->RequestSetName(survivorName, playerGUIDValue);
					}

					survivor->RequestSetLocation(translationX, translationY, ownerRotationZ, playerGUIDValue);
				}
			}
		}
	}
}

void UCWorldMap::CreateSurvivorLocationOnWorldMap(const FText& InText, uint32 NetGUIDValue)
{
	TWeakObjectPtr<UCPlayerLocation>* playerLocationPtr = PlayerLocationMap.Find(NetGUIDValue);

	if (playerLocationPtr && playerLocationPtr->IsValid())
	{

	}
	else
	{
		if (PlayerLocationClass)
		{
			// 위젯 생성
			UCPlayerLocation* newPlayerLocation = CreateWidget<UCPlayerLocation>(GetWorld(), PlayerLocationClass);

			// 맵에 추가
			PlayerLocationMap.Add(NetGUIDValue, TWeakObjectPtr<UCPlayerLocation>(newPlayerLocation));

			// 방금 추가한 객체를 바로 캐시
			UCPlayerLocation* addedPlayerLocation = newPlayerLocation;

			// 위젯이 유효하면 WorldMapCanvasPanel에 추가
			if (addedPlayerLocation)
			{
				WorldMapCanvasPanel->AddChild(addedPlayerLocation);

				if (PersonalNetGuidValue == NetGUIDValue)
					addedPlayerLocation->CheckWidgetOwner(true);
				else
					addedPlayerLocation->CheckWidgetOwner(false);

				addedPlayerLocation->RegisterPlayerName(InText);

				UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(addedPlayerLocation->Slot);
				if (canvasSlot)
				{
					canvasSlot->SetPosition(FVector2D(370.0f, 50.0f));
				}

				CDebug::Print("Added On Canvas Panel");
			}
			else
			{
				CDebug::Print("addedPlayerLocation is not Valid");
			}
		}
		else
			CDebug::Print("PlayerLocationClass is not Valid");
	}
}

void UCWorldMap::RefreshSurvivorLocationOnWorldMap(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue)
{
	TWeakObjectPtr<UCPlayerLocation>* playerLocationPtr = PlayerLocationMap.Find(NetGUIDValue);

	if (playerLocationPtr && playerLocationPtr->IsValid())
	{
		UCPlayerLocation* playerLocation = playerLocationPtr->Get();
		if (playerLocation)
		{
			playerLocation->UpdatePlayerLocation(LocationX, LocationY, RotationZ);
		}
	}
}
