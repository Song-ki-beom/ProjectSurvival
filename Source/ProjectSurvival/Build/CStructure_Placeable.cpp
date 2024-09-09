#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponents/CActorInventoryComponent.h"
//#include "ActorComponents/CInventoryComponent.h"
#include "Widget/CMainHUD.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Blueprint/UserWidget.h"

#include "Net/UnrealNetwork.h"
#include "Widget/Inventory/CItemBase.h"

#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"

#include "Utility/CDebug.h"

ACStructure_Placeable::ACStructure_Placeable()
{
	//bReplicates = true;

	DownBox = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownBox->SetupAttachment(PickupMesh);

	//ActorInventoryComponent = CreateDefaultSubobject<UCActorInventoryComponent>(TEXT("ActorInventory"));
	//ActorInventoryComponent->SetIsReplicated(true);
}

void ACStructure_Placeable::BeginPlay()
{
	Super::BeginPlay();
}

void ACStructure_Placeable::CheckDown_FoundationAndCeiling()
{
	// 액터 체크
	FHitResult actorHitResult;
	FVector startLocation = PreviewBox->GetComponentLocation();
	FVector endLocation = startLocation - FVector(0, 0, 300);
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeQuery;
	objectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	TArray<AActor*> ignores;
	ignores.Add(this);
	FCollisionQueryParams collisionParams;
	bool bDownActorHit = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		startLocation,
		endLocation,
		objectTypeQuery,
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		actorHitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::Red
	);

	if (bDownActorHit) // 라인트레이스가 어떤 액터에 히트했는지 확인
	{
		AActor* hitActor = actorHitResult.GetActor();
		if (hitActor && (hitActor->IsA(ACStructure_Foundation::StaticClass()) || hitActor->IsA(ACStructure_Ceiling::StaticClass())))
		{
			//CDebug::Print("hitActor: ", hitActor, FColor::Cyan);
			bDown_FoundationAndCeilingActorHit = true;
			PlaceableHeight = actorHitResult.ImpactPoint.Z;
		}
		else
		{

		}
	}
	else
	{
		bDown_FoundationAndCeilingActorHit = false;
		PlaceableHeight = GetOwner()->GetActorLocation().Z;
	}
}

void ACStructure_Placeable::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = PreviewBox->GetComponentLocation();
	FVector centerBoxSize = PreviewBox->GetScaledBoxExtent() - FVector(0,0,1);
	FRotator centerBoxOrientation;
	centerBoxOrientation = this->GetActorRotation();
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery2;
	bool bCenterBoxTraceComplex = false;
	TArray<AActor*> centerBoxActorsToIgnore;
	TArray<FHitResult> centerBoxHitResults;
	bCenterHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		centerBoxLocation,
		centerBoxLocation,
		centerBoxSize,
		centerBoxOrientation,
		centerBoxTraceTypeQuery,
		bCenterBoxTraceComplex,
		centerBoxActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		centerBoxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
}

//void ACStructure_Placeable::CreateActorInventoryComponent()
//{
//	ActorInventoryComponent = NewObject<UCActorInventoryComponent>(this);
//
//	if (ActorInventoryComponent)
//	{
//		ActorInventoryComponent->RegisterComponent();
//		ActorInventoryComponent->SetIsReplicated(true);
//	}
//}

void ACStructure_Placeable::OpenActorInventory(const ACSurvivor* Survivor, class AActor* Actor)
{
	CDebug::Print("OpenActorInventory Called");
	if (Survivor)
	{
		switch (WidgetCaller)
		{
		case EWidgetCall::WorkBench:
		{
			if (ActorInventoryWidgetClass)
			{
				if (ActorInventoryWidget)
				{
					ACSurvivorController* survivorController = Cast<ACSurvivorController>(Survivor->GetController());
					if (survivorController)
					{
						ACMainHUD* mainHUD = Cast<ACMainHUD>(survivorController->GetHUD());
						if (mainHUD)
						{
							mainHUD->SetWidgetVisibility(EWidgetCall::WorkBench, ActorInventoryWidget, Actor);
						}
					}
				}
				else
				{
					ActorInventoryWidget = CreateWidget<UUserWidget>(GetWorld(), ActorInventoryWidgetClass);

					ACSurvivorController* survivorController = Cast<ACSurvivorController>(Survivor->GetController());
					if (survivorController)
					{
						ACMainHUD* mainHUD = Cast<ACMainHUD>(survivorController->GetHUD());
						if (mainHUD)
							mainHUD->SetWidgetVisibility(EWidgetCall::WorkBench, ActorInventoryWidget, Actor);
					}
				}
			}
		}
		}
	}
	else
		CDebug::Print("Survivor is not valid");
}

void ACStructure_Placeable::PerformAddID(FName InID)
{
	//ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));


	FItemData* itemData = ItemDataTable->FindRow<FItemData>(InID, TEXT(""));
	if (itemData)
	{
	    UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
	    ItemCopy->ID = InID;
	    ItemCopy->Quantity = 1;
	    ItemCopy->ItemType = itemData->ItemType;
	    ItemCopy->TextData = itemData->TextData;
	    ItemCopy->ItemStats = itemData->ItemStats;
	    ItemCopy->NumericData = itemData->NumericData;
	    ItemCopy->AssetData = itemData->AssetData;
	    ItemCopy->bIsCopy = true;
		
	    SharedInventoryObject.Add(ItemCopy);
	}
	
	for (UCItemBase* tempItem : SharedInventoryObject)
	{
	    CDebug::Print("Item :", tempItem->ID);
	}

	//SharedInventoryID.Add(InID);
	
	//for (FName tempName : SharedInventoryID)
	//{
	//    CDebug::Print("TempName : ", tempName, FColor::Magenta);
	//
	//
	//}
}

void ACStructure_Placeable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACStructure_Placeable, SharedInventoryObject);
}
