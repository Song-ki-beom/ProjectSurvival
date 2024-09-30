#include "Widget/Map/CMiniMap.h"
#include "Components/Image.h"

void UCMiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	// 확대된 부분 그리기 위한 Brush 생성
	if (UTexture2D* worldMapTexture = Cast<UTexture2D>(WorldMap->Brush.GetResourceObject()))
	{
		ImageSize = FVector2D(worldMapTexture->GetSizeX(), worldMapTexture->GetSizeY());
		WorldMap->SetBrushFromTexture(worldMapTexture);
		MiniMapBrush.SetResourceObject(worldMapTexture);
		MiniMapBrush.ImageSize = FVector2D(worldMapTexture->GetSizeX(), worldMapTexture->GetSizeY());
		MiniMapBrush.DrawAs = ESlateBrushDrawType::Image;
	}
}


void UCMiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D characterPos = GetCharacterPosOnWorldMap();

	// UV 설정 (0 ~ 1 사이 값)
	float uvMinX = (characterPos.X - 100.0f) / ImageSize.X;
	float uvMinY = (characterPos.Y - 100.0f) / ImageSize.Y;
	float uvMaxX = (characterPos.X + 100.0f) / ImageSize.X;
	float uvMaxY = (characterPos.Y + 100.0f) / ImageSize.Y;

	MiniMapBrush.SetUVRegion(FBox2D(FVector2D(uvMinX, uvMinY), FVector2D(uvMaxX, uvMaxY)));
	WorldMap->SetBrush(MiniMapBrush);
}

FVector2D UCMiniMap::GetCharacterPosOnWorldMap()
{
	// 캐릭터의 실제 위치를 WorldMap 텍스쳐 이미지 위치로 변환 (좌상단 0,0 기준)
	FTransform ownerTransform = this->GetOwningPlayerPawn()->GetActorTransform();
	float ownerLocationX = ownerTransform.GetLocation().X;
	float ownerLocationY = ownerTransform.GetLocation().Y;
	float ownerRotationZ = ownerTransform.GetRotation().Rotator().Yaw;

	float translationX = (ownerLocationY - WorldMapLevelTopLeftLocation.Y) / WorldMapLevelWidth * ImageSize.X;
	float translationY = (WorldMapLevelTopLeftLocation.X - ownerLocationX) / WorldMapLevelHeight * ImageSize.Y;

	FWidgetTransform widgetTransform;
	widgetTransform.Angle = ownerRotationZ;
	PlayerLocation->SetRenderTransform(widgetTransform);

	return FVector2D(translationX, translationY);
}

