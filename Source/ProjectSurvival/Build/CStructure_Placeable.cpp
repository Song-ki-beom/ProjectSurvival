#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Ceiling.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ActorComponents/CActorInventoryComponent.h"

#include "Widget/CMainHUD.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Blueprint/UserWidget.h"

#include "Net/UnrealNetwork.h"
#include "Widget/Inventory/CItemBase.h"

#include "Widget/Inventory/CInventoryPanel_WorkingBench.h"

#include "ActorComponents/CInventoryComponent.h"

#include "Utility/CDebug.h"

ACStructure_Placeable::ACStructure_Placeable()
{
	//bReplicates = true;

	DownBox = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownBox->SetupAttachment(PickupMesh);
}

void ACStructure_Placeable::BeginPlay()
{
	Super::BeginPlay();
	ActorInventoryWidget = CreateWidget<UUserWidget>(GetWorld(), ActorInventoryWidgetClass);
	UCInventoryPanel_WorkingBench* workingBenchWidget = Cast<UCInventoryPanel_WorkingBench>(ActorInventoryWidget);
	if (workingBenchWidget)
		workingBenchWidget->SetOwnerActor(this);
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
					CDebug::Print("ActorInventoryWidget is not Valid");
			}
			else
				CDebug::Print("ActorInventoryWidgetClass is not Valid");
		}
		}
	}
	else
		CDebug::Print("Survivor is not valid");
}

void ACStructure_Placeable::PerformAddID(FName InID, int32 InQuantity, FItemNumericData InNumericData)
{
	InventoryQuantityArray.Add(InQuantity);
	SharedInventoryQuantityArray = InventoryQuantityArray;

	InventoryIDArray.Add(InID);
	SharedInventoryIDArray = InventoryIDArray;

	InventoryNumericDataArray.Add(InNumericData);
	SharedInventoryNumericDataArray = InventoryNumericDataArray;

	BroadCastTrigger = !BroadCastTrigger;

	if (HasAuthority())
		AddItemInfoToWidget();
}

void ACStructure_Placeable::OnRep_BroadCastTrigger()
{
	AddItemInfoToWidget();
}

void ACStructure_Placeable::AddItemInfoToWidget()
{
	CDebug::Print("BroadCastAddItemInfo_Implementation Called", FColor::Cyan);
	CDebug::Print("SharedInventoryIDArray.Num() :", SharedInventoryIDArray.Num());

	ActorInventoryContents.Empty();

	for (int32 tempIndex = 0; tempIndex < SharedInventoryIDArray.Num(); tempIndex++)
	{
		FName tempID = SharedInventoryIDArray[tempIndex];
		FItemData* itemData = ItemDataTable->FindRow<FItemData>(tempID, TEXT(""));
		if (itemData)
		{
			UCItemBase* ItemCopy = NewObject<UCItemBase>(StaticClass());
			ItemCopy->ID = tempID;
			ItemCopy->Quantity = SharedInventoryQuantityArray[tempIndex];
			ItemCopy->ItemType = itemData->ItemType;
			ItemCopy->TextData = itemData->TextData;
			ItemCopy->ItemStats = itemData->ItemStats;
			ItemCopy->NumericData = itemData->NumericData;
			ItemCopy->AssetData = itemData->AssetData;
			ItemCopy->bIsCopy = true;
	
			ActorInventoryContents.Add(ItemCopy);
	
			UCInventoryPanel_WorkingBench* workingBenchWidget = Cast<UCInventoryPanel_WorkingBench>(ActorInventoryWidget);
			if (workingBenchWidget)
			{
				workingBenchWidget->SetWidgetItems(ActorInventoryContents);
				workingBenchWidget->OnWorkingBenchUpdated.Broadcast();
			}
		}
		else
			CDebug::Print("Itemdata is not Valid", FColor::Magenta);
	}
	
	CDebug::Print("Every Item Added by Multicast And Number is : ", ActorInventoryContents.Num(), FColor::Silver);
}

void ACStructure_Placeable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACStructure_Placeable, SharedInventoryIDArray);
	DOREPLIFETIME(ACStructure_Placeable, SharedInventoryQuantityArray);
	DOREPLIFETIME(ACStructure_Placeable, SharedInventoryNumericDataArray);
	DOREPLIFETIME(ACStructure_Placeable, BroadCastTrigger);
	
}
