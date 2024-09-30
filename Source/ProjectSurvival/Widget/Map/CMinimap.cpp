#include "Widget/Map/CMinimap.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Character/CSurvivorController.h"
#include "Utility/CDebug.h"

UCMinimap::UCMinimap(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCMinimap::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* worldMapTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/PirateIsland/Include/Textures/UI/WorldMap.WorldMap'")));

	if (!worldMapTexture)
		return;

	WorldMapImage->SetBrushFromTexture(worldMapTexture);
	MinimapBrush.SetResourceObject(worldMapTexture);
	MinimapBrush.ImageSize = FVector2D(worldMapTexture->GetSizeX(), worldMapTexture->GetSizeY());
	MinimapBrush.DrawAs = ESlateBrushDrawType::Image;
}


void UCMinimap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D characterPos = GetCharacterPosOnWorldMap();

	// UV 설정 (0 ~ 1 사이 값)
	float uvMinX = (characterPos.X - 100.0f) / ImageSize.X;
	float uvMinY = (characterPos.Y - 100.0f) / ImageSize.Y;
	float uvMaxX = (characterPos.X + 100.0f) / ImageSize.X;
	float uvMaxY = (characterPos.Y + 100.0f) / ImageSize.Y;

	MinimapBrush.SetUVRegion(FBox2D(FVector2D(uvMinX, uvMinY), FVector2D(uvMaxX, uvMaxY)));
	WorldMapImage->SetBrush(MinimapBrush);
}

void UCMinimap::SetUpMinimap()
{
	CreateWidget<UUserWidget>(GetWorld(), MinimapClass);
	this->AddToViewport(5);
}

FVector2D UCMinimap::GetCharacterPosOnWorldMap()
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

