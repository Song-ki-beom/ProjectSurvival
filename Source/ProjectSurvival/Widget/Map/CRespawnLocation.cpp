#include "Widget/Map/CRespawnLocation.h"
#include "Build/CStructure_Placeable.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCRespawnLocation::SetOwnerActor(class AActor* InActor)
{
	if (!this->IsPendingKill() && InActor && !InActor->IsPendingKill())
	{
		InActor->OnDestroyed.AddDynamic(this, &UCRespawnLocation::DestroyWidget);
		class ACStructure_Placeable* placeableStructure = Cast<ACStructure_Placeable>(InActor);
		if (placeableStructure)
			placeableStructure->OnTextSet.AddDynamic(this, &UCRespawnLocation::SetRespawnLoctionName);
			
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

