#include "Widget/Map/CWorldMap.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "ActorComponents/CBuildComponent.h"
#include "Character/CSurvivorController.h"
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

	ButtonNormalBrush = GetBrushButton->WidgetStyle.Normal;
	ButtonPressedBrush = GetBrushButton->WidgetStyle.Pressed;

	// 위치정보 보내기
	FTimerHandle worldMapUpdateTimer;
	GetWorld()->GetTimerManager().SetTimer(worldMapUpdateTimer, this, &UCWorldMap::SetCharacterPosOnWorldMap, 0.025f, true, 3.0f);

	// 이름 보내기 중단 (이미 정해져서 업데이트 필요x)
	FTimerHandle nameTransmitTimer;
	GetWorld()->GetTimerManager().SetTimer(nameTransmitTimer, this, &UCWorldMap::DisableNameTransmit, 30.0f, false);

	// 리스폰 위치 검사
	//FTimerHandle respawnLocationCheckTimer;
	//GetWorld()->GetTimerManager().SetTimer(respawnLocationCheckTimer, this, &UCWorldMap::Test, 0.5f, true, 5.0f);
}

void UCWorldMap::SetCharacterPosOnWorldMap()
{
	// 캐릭터의 실제 위치를 WorldMap 텍스쳐 이미지 위치로 변환 (좌상단 0,0 기준)
	if (!this->GetOwningPlayerPawn()) return;
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

				addedPlayerLocation->RegisterPlayer(PersonalSurvivor);

				addedPlayerLocation->RegisterPlayerName(InText);

				UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(addedPlayerLocation->Slot);
				if (canvasSlot)
				{
					canvasSlot->SetPosition(FVector2D(370.0f, 50.0f));
				}

				//CDebug::Print("Added On Canvas Panel");
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
		PlayerLocationPtr = *playerLocationPtr;

		UCPlayerLocation* playerLocation = playerLocationPtr->Get();
		if (playerLocation)
		{
			playerLocation->UpdatePlayerLocation(LocationX, LocationY, RotationZ);
		}
	}
}

void UCWorldMap::SetActorOnWorldMap(class AActor* InActor)
{
	CDebug::Print("SetActorOnWorldMap Called", FColor::White);
	if (InActor)
	{
		CDebug::Print("InActor Is Valid");
		if (PersonalSurvivor->HasAuthority())
			PersonalSurvivor->GetBuildComponent()->BroadcastRegisterOnWorldMap(InActor);
	}
	else
		CDebug::Print("InActor Is Not Valid", FColor::Silver);
}

void UCWorldMap::CreateRespawnLocationOnWorldMap(AActor* InActor)
{
	// 빌드컴포넌트에서 멀티캐스트로 호출된 함수

	if (InActor)
	{
		CDebug::Print("CreateRespawnLocationOnWorldMap Called", FColor::Red);

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
			//RespawnLocationMap.Add(InActor, TWeakObjectPtr<UCRespawnLocation>(newRespawnLocation));

			// 방금 추가한 객체를 바로 캐시
			UCRespawnLocation* addedRespawnLocation = newRespawnLocation;

			// 위젯이 유효하면 WorldMapCanvasPanel에 추가
			if (addedRespawnLocation)
			{
				CDebug::Print("addedRespawnLocation is Valid", FColor::Red);

				WorldMapCanvasPanel->AddChild(addedRespawnLocation);

				UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(addedRespawnLocation->Slot);
				if (canvasSlot)
				{
					canvasSlot->SetPosition(FVector2D(370.0f, 50.0f));
				}
				addedRespawnLocation->SetOwnerActor(InActor);
				addedRespawnLocation->SetRespawnLocationOnWorldMap(translationX, translationY);

				//RespawnButtonArray.Add(addedRespawnLocation->GetRespawnButton());
				//
				//for (class UButton* tempButton : RespawnButtonArray)
				//{
				//	if (IsValid(tempButton))
				//	{
				//		CDebug::Print("Button Is Valid : ", tempButton, FColor::Cyan);
				//	}
				//	else
				//		CDebug::Print("Button Is Not Valid", FColor::Cyan);
				//}

				RespawnLocationPtrArray.Add(addedRespawnLocation);



				CDebug::Print("Array Num : ", RespawnLocationPtrArray.Num(), FColor::Cyan);
			}
			else
				CDebug::Print("addedRespawnLocation is Not Valid", FColor::Red);
		}
	}
	else
		CDebug::Print("InActor Is Not Valid At WorldMap", FColor::Red);
}

void UCWorldMap::RefreshRespawnLocationOnWorldMap()
{
	//TArray<TWeakObjectPtr<UCRespawnLocation>> tempArray = RespawnLocationPtrArray;



	
}

void UCWorldMap::Test()
{
	CDebug::Print("Array Num : ", RespawnLocationPtrArray.Num(), FColor::Cyan);
}

void UCWorldMap::HideSurvivorLocationOnWorldMap(uint32 NetGUIDValue)
{
	TWeakObjectPtr<UCPlayerLocation>* playerLocationPtr = PlayerLocationMap.Find(NetGUIDValue);

	if (playerLocationPtr && playerLocationPtr->IsValid())
	{
		UCPlayerLocation* playerLocation = playerLocationPtr->Get();
		if (playerLocation)
		{
			playerLocation->HidePlayerLocation();
		}
	}
}

void UCWorldMap::ShowSurvivorLocationOnWorldMap(uint32 NetGUIDValue)
{
	TWeakObjectPtr<UCPlayerLocation>* playerLocationPtr = PlayerLocationMap.Find(NetGUIDValue);

	if (playerLocationPtr && playerLocationPtr->IsValid())
	{
		UCPlayerLocation* playerLocation = playerLocationPtr->Get();
		if (playerLocation)
		{
			playerLocation->ShowPlayerLocation();
		}
	}
}

void UCWorldMap::SetRespawnButtonStyleToNormal()
{
	for (int32 i = RespawnLocationPtrArray.Num() - 1; i >= 0; --i)
	{
		if (RespawnLocationPtrArray[i].IsValid())
		{
			FButtonStyle style = RespawnLocationPtrArray[i].Get()->GetRespawnButton()->WidgetStyle;
			if (style.Normal == ButtonNormalBrush)
			{
				continue;
			}

			style.Normal = ButtonNormalBrush;
			style.Hovered = ButtonNormalBrush;
			RespawnLocationPtrArray[i].Get()->GetRespawnButton()->SetStyle(style);
		}
	}
}

void UCWorldMap::SetRespawnButtonStyle(UButton* InSelectedButton)
{
	for (int32 i = RespawnLocationPtrArray.Num() - 1; i >= 0; --i)
	{
		if (IsValid(InSelectedButton))
		{
			FButtonStyle selectedStyle = InSelectedButton->WidgetStyle;
			selectedStyle.Normal = ButtonPressedBrush;
			selectedStyle.Hovered = ButtonPressedBrush;
			InSelectedButton->SetStyle(selectedStyle);
		}
	}
}



//for (UButton* button : BuildButtons)
//{
//	if (IsValid(button))
//	{
//		FButtonStyle style = button->WidgetStyle;
//		if (style.Normal == ButtonNormalBrush)
//			continue;
//
//		style.Normal = ButtonNormalBrush;
//		button->SetStyle(style);
//	}
//}
//

