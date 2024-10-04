#include "Widget/Map/CWorldMap.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "ActorComponents/CBuildComponent.h"
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
			PersonalSurvivor = Cast<ACSurvivor>(this->GetOwningPlayerPawn());
			PersonalSurvivorController = Cast<ACSurvivorController>(this->GetOwningPlayer());
		}
	}

	// 위치정보 보내기
	FTimerHandle worldMapUpdateTimer;
	GetWorld()->GetTimerManager().SetTimer(worldMapUpdateTimer, this, &UCWorldMap::SetCharacterPosOnWorldMap, 0.025f, true, 3.0f);

	// 이름 보내기 중단 (이미 정해져서 업데이트 필요x)
	FTimerHandle nameTransmitTimer;
	GetWorld()->GetTimerManager().SetTimer(nameTransmitTimer, this, &UCWorldMap::DisableNameTransmit, 30.0f, false);

	// 리스폰 위치 검사
	FTimerHandle respawnLocationCheckTimer;
	GetWorld()->GetTimerManager().SetTimer(respawnLocationCheckTimer, this, &UCWorldMap::RefreshRespawnLocationOnWorldMap, 0.5f, true, 5.0f);
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

void UCWorldMap::SetActorOnWorldMap(class AActor* InActor)
{
	if (InActor)
	{
		PersonalSurvivor->GetBuildComponent()->BroadcastRegisterOnWorldMap(InActor);
	}
}

void UCWorldMap::CreateRespawnLocationOnWorldMap(AActor* InActor)
{
	// 빌드컴포넌트에서 멀티캐스트로 호출된 함수

	if (InActor)
	{
		float actorLocationX = InActor->GetActorLocation().X;
		float actorLocationY = InActor->GetActorLocation().Y;

		if (UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(WorldMap->Slot))
			ImageSize = canvasPanelSlot->GetSize();

		float translationX = (actorLocationY - WorldMapLevelTopLeftLocation.Y) / WorldMapLevelWidth * ImageSize.X;
		float translationY = (WorldMapLevelTopLeftLocation.X - actorLocationX) / WorldMapLevelHeight * ImageSize.Y;

		if (RespawnLocationClass)
		{
			// 위젯 생성
			UCRespawnLocation* newRespawnLocation = CreateWidget<UCRespawnLocation>(GetWorld(), RespawnLocationClass);

			// 맵에 추가
			RespawnLocationMap.Add(InActor, TWeakObjectPtr<UCRespawnLocation>(newRespawnLocation));

			// 방금 추가한 객체를 바로 캐시
			UCRespawnLocation* addedRespawnLocation = newRespawnLocation;

			// 위젯이 유효하면 WorldMapCanvasPanel에 추가
			if (addedRespawnLocation)
			{
				WorldMapCanvasPanel->AddChild(addedRespawnLocation);

				UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(addedRespawnLocation->Slot);
				if (canvasSlot)
				{
					canvasSlot->SetPosition(FVector2D(370.0f, 50.0f));
				}
				addedRespawnLocation->SetOwnerActor(InActor);
				addedRespawnLocation->SetRespawnLocationOnWorldMap(translationX, translationY);
			}
		}
	}
}

void UCWorldMap::RefreshRespawnLocationOnWorldMap()
{
	//// 유효하지 않은 키들을 따로 저장하기 위한 배열
	//TArray<AActor*> InvalidKeys;

	//// 맵을 순회하며 nullptr 키를 찾음
	//for (const TPair<AActor*, TWeakObjectPtr<UCRespawnLocation>>& Elem : RespawnLocationMap)
	//{
	//	AActor* Key = Elem.Key;

	//	// 키가 nullptr인 경우
	//	if (Key == nullptr)
	//	{
	//		// 해당 값이 유효한지 확인
	//		if (Elem.Value.IsValid())
	//		{
	//			// 값을 얻어와 파괴 (MarkPendingKill 호출)
	//			UCRespawnLocation* RespawnLocation = Elem.Value.Get();
	//			if (RespawnLocation)
	//			{
	//				RespawnLocation->RemoveFromParent();  // 위젯을 UI에서 제거 (필요한 경우)
	//				RespawnLocation->MarkPendingKill();   // UObject 파괴
	//			}
	//		}

	//		// 유효하지 않은 키를 배열에 저장
	//		InvalidKeys.Add(Key);
	//	}
	//}

	//// 유효하지 않은 키들을 맵에서 제거
	//for (AActor* InvalidKey : InvalidKeys)
	//{
	//	RespawnLocationMap.Remove(InvalidKey);
	//}
}
