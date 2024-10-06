#include "Widget/Map/CRespawnLocation.h"
#include "Widget/Map/CWorldMap.h"
#include "Build/CStructure_Placeable.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

void UCRespawnLocation::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UCRespawnLocation::Initialize()
{
	bool Sucess = Super::Initialize();

	if (!Sucess)
		return false;

	if (!IsValid(RespawnLocationButton)) { CDebug::Print("RespawnLocationButton is invalid"); return false; }
	RespawnLocationButton->OnClicked.AddDynamic(this, &UCRespawnLocation::OnClickRespawnLocationButton);

	return true;
}

void UCRespawnLocation::SetOwnerActor(class AActor* InActor)
{
	if (!this->IsPendingKill() && InActor && !InActor->IsPendingKill())
	{
		InActor->OnDestroyed.AddDynamic(this, &UCRespawnLocation::DestroyWidget);
		class ACStructure_Placeable* placeableStructure = Cast<ACStructure_Placeable>(InActor);
		if (placeableStructure)
		{
			BedActor = placeableStructure;
			placeableStructure->OnTextSet.AddDynamic(this, &UCRespawnLocation::SetRespawnLoctionName);
		}
			
	}
}

void UCRespawnLocation::SetRespawnLocationOnWorldMap(float LocationX, float LocationY)
{
	//FWidgetTransform imageTransform;
	//imageTransform.Translation = FVector2D(LocationX, LocationY);
	//imageTransform.Angle = RotationZ;
	//PlayerLocationImage->SetRenderTransform(imageTransform);
	//
	//FWidgetTransform textTransform;
	//textTransform.Translation = FVector2D(LocationX, LocationY + 20.0f);
	//PlayerName->SetRenderTransform(textTransform);

	FWidgetTransform widgetTransform;
	widgetTransform.Translation = FVector2D(LocationX, LocationY);
	RespawnLocationButton->SetRenderTransform(widgetTransform);
	RespawnLoctionName->SetRenderTransform(widgetTransform);
}

void UCRespawnLocation::DestroyWidget(class AActor* DestroyedActor)
{
	RemoveFromParent();
	MarkPendingKill();
}

void UCRespawnLocation::SetRespawnLoctionName(const FText& InText)
{
	RespawnLoctionName->SetText(InText);
}

void UCRespawnLocation::OnClickRespawnLocationButton()
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		gameInstance->WorldMap->SetRespawnButtonStyleToNormal();
		gameInstance->WorldMap->SetRespawnButtonStyle(RespawnLocationButton);

		if (UCRespawnConfirm* repawnConfirm = gameInstance->WorldMap->GetRespawnConfirm())
		{
			repawnConfirm->SelectRespawnLocation(RespawnLoctionName->GetText(), BedActor->GetActorLocation() + FVector(0,0,50));

			if (repawnConfirm->GetVisibility() == ESlateVisibility::Collapsed)
				repawnConfirm->SetVisibility(ESlateVisibility::Visible);
		}
	}
}