#include "Widget/Map/CWorldMap.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UCWorldMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetCharacterPosOnWorldMap();
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

	FWidgetTransform widgetTransform;
	widgetTransform.Translation = FVector2D(translationX, translationY); // 월드맵은 텍스쳐가 정지하고 캐릭터 커서가 움직임
	widgetTransform.Angle = ownerRotationZ;
	PlayerLocation->SetRenderTransform(widgetTransform);
}