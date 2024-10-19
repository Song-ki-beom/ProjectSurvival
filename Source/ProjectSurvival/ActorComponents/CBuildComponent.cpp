#include "ActorComponents/CBuildComponent.h"
#include "ActorComponents/CInventoryComponent.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Wall.h"
#include "Build/CStructure_Ceiling.h"
#include "Build/CStructure_Ramp.h"
#include "Build/CStructure_DoorFrame.h"
#include "Build/CStructure_Door.h"
#include "Build/CStructure_Stair.h"
#include "Build/CStructure_Placeable.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Widget/Build/CBuildWidget.h"
#include "Widget/Build/CBuildItemSlot.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryPanel.h"
#include "Widget/Inventory/CQuickSlot.h"
#include "Widget/Menu/CInventoryMenu.h"
#include "Widget/Map/CWorldMap.h"
#include "CGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/DataTable.h"
#include "Utility/CDebug.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bIsBuilding = false;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> redMaterialFinder(TEXT("MaterialInstanceConstant'/Game/PirateIsland/Include/Materials/Builds/MI_Build_Red.MI_Build_Red'"));
	if (redMaterialFinder.Succeeded())
	{
		RedMaterial = redMaterialFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> greenMaterialFinder(TEXT("MaterialInstanceConstant'/Game/PirateIsland/Include/Materials/Builds/MI_Build_Green.MI_Build_Green'"));
	if (greenMaterialFinder.Succeeded())
	{
		GreenMaterial = greenMaterialFinder.Object;
	}
}

void UCBuildComponent::BeginPlay()
{
	Super::BeginPlay();
	Survivor = Cast<ACSurvivor>(this->GetOwner());
	ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwner()->GetInstigatorController());
	if (survivorController)
		survivorController->SetupBuildWidget();

	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'")));
}

void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(SpawnedStructure))
	{
		if (!bIsBuilding)
			bIsBuilding = true;
	}
	else
		bIsBuilding = false;

	switch (StructureElement)
	{
	case EBuildStructureElement::Foundation:
	{
		BuildStartFoundation();
		break;
	}
	case EBuildStructureElement::TriFoundation:
		break;
	case EBuildStructureElement::Wall:
	{
		BuildStartWall();
		break;
	}
	case EBuildStructureElement::WindowWall:
		break;
	case EBuildStructureElement::TriLeftWall:
		break;
	case EBuildStructureElement::TriRightWall:
		break;
	case EBuildStructureElement::TriTopWall:
		break;
	case EBuildStructureElement::Ceiling:
	{
		BuildStartCeiling();
		break;
	}
	case EBuildStructureElement::TriCeiling:
		break;
	case EBuildStructureElement::Roof:
		break;
	case EBuildStructureElement::HalfRoof:
		break;
	case EBuildStructureElement::DoorFrame:
	{
		BuildStartDoorFrame();
		break;
	}
	case EBuildStructureElement::Door:
	{
		BuildStartDoor();
		break;
	}
	case EBuildStructureElement::Fence:
		break;
	case EBuildStructureElement::Ramp:
	{
		BuildStartRamp();
		break;
	}
	case EBuildStructureElement::Stair:
	{
		BuildStartStair();
		break;
	}
	case EBuildStructureElement::Placeable:
	{
		BuildStartPlaceable();
		break;
	}
	case EBuildStructureElement::None:
		break;
	default:
		break;
	}
}

void UCBuildComponent::SelectQ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectQ");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectW(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectW");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectE(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectE");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectA(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectA");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectS(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectS");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectD(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectD");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectZ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectZ");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectX(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectX");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::SelectC(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	//CDebug::Print("SelectC");

	SpawnBuildStructureElement(InClass, InElement, InItemID);
}

void UCBuildComponent::BuildSpawnedStructure()
{
	if (!bIsBuildable)
	{
		//CDebug::Print("You Can't Build There", FColor::Cyan);
		return;
	}

	FName itemID = SpawnedStructure->GetBuildStructureID();

	for (TWeakObjectPtr<UCItemBase> itemPtr : Survivor->GetInventoryComponent()->GetInventoryContents())
	{
		if (itemPtr->ID == itemID)
		{
			float inventoryTotalWeight = Survivor->GetInventoryComponent()->GetInventoryTotalWeight();
			float usedTotalWeight = itemPtr->NumericData.Weight;
			float newTotalWeight = inventoryTotalWeight - usedTotalWeight;
			Survivor->GetInventoryComponent()->SetInventoryTotalWeight(newTotalWeight);
			itemPtr->SetQuantity(itemPtr->Quantity - 1);
			Survivor->GetInventoryComponent()->OnInventoryUpdated.Broadcast();
			
			ACSurvivorController* survivorController = Cast<ACSurvivorController>(Survivor->GetController());
			if (survivorController)
			{
				for (UCBuildItemSlot* buildItemSlot : survivorController->GetBuildWidget()->GetBuildItemSlotArray())
				{
					if (!buildItemSlot)
						continue;

					if (buildItemSlot->GetBuildItemSlotID() == itemID)
					{
						if (survivorController)
						{
							if (survivorController->GetBuildWidget())
								survivorController->GetBuildWidget()->RefreshBuildWidgetQuantity(itemID);
						}

						break;
					}
				}
			}
		}
	}

	UGameplayStatics::PlaySound2D(this, BuildSound);

	if (IsValid(SpawnedStructure))
		SpawnedStructure->SetReplicates(true);

	if (Survivor->HasAuthority())
	{
		StructureTransform = SpawnedStructure->GetActorTransform();
		PerformBuild(StructureClass, StructureTransform);
		SpawnedStructure->SetReplicates(false);
	}
	else
	{
		//CDebug::Print("Spawned Structure : ", SpawnedStructure);
		StructureTransform = SpawnedStructure->GetActorTransform();
		RequestBuild(StructureClass, StructureTransform);
	}
}

void UCBuildComponent::ClearSpawnedStructure()
{
	if (SpawnedStructure)
		SpawnedStructure->Destroy();
	bIsSnapped = false;
}

void UCBuildComponent::BroadcastRegisterOnWorldMap_Implementation(class AActor* InActor)
{
	if (InActor)
	{
		class UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
		if (gameInstance)
		{
			gameInstance->WorldMap->CreateRespawnLocationOnWorldMap(InActor);
		}
	}
}

void UCBuildComponent::SpawnBuildStructureElement(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	StructureClass = InClass;
	StructureElement = InElement;

	switch (StructureElement)
	{
	case EBuildStructureElement::Foundation:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedFoundation = GetWorld()->SpawnActor<ACStructure_Foundation>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedFoundation->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedFoundation);
		break;
	}
	case EBuildStructureElement::TriFoundation:
		break;
	case EBuildStructureElement::Wall:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedWall = GetWorld()->SpawnActor<ACStructure_Wall>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedWall->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedWall);
		break;
	}
	case EBuildStructureElement::WindowWall:
		break;
	case EBuildStructureElement::TriLeftWall:
		break;
	case EBuildStructureElement::TriRightWall:
		break;
	case EBuildStructureElement::TriTopWall:
		break;
	case EBuildStructureElement::Ceiling:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedCeiling = GetWorld()->SpawnActor<ACStructure_Ceiling>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedCeiling->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedCeiling);
		break;
	}
	case EBuildStructureElement::TriCeiling:
		break;
	case EBuildStructureElement::Roof:
		break;
	case EBuildStructureElement::HalfRoof:
		break;
	case EBuildStructureElement::DoorFrame:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedDoorFrame = GetWorld()->SpawnActor<ACStructure_DoorFrame>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedDoorFrame->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedDoorFrame);
		break;
	}
	case EBuildStructureElement::Door:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedDoor = GetWorld()->SpawnActor<ACStructure_Door>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedDoor->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedDoor);
		break;
	}
	case EBuildStructureElement::Fence:
		break;
	case EBuildStructureElement::Ramp:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedRamp = GetWorld()->SpawnActor<ACStructure_Ramp>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedRamp->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedRamp);
		break;
	}
	case EBuildStructureElement::Stair:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedStair = GetWorld()->SpawnActor<ACStructure_Stair>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedStair->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedStair);
		break;
	}
	case EBuildStructureElement::Placeable:
	{
		if (IsValid(SpawnedStructure))
			SpawnedStructure->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedPlaceable = GetWorld()->SpawnActor<ACStructure_Placeable>(StructureClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedPlaceable->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedPlaceable);
		break;
	}
	case EBuildStructureElement::None:
		break;
	default:
		break;
	}

	if (IsValid(SpawnedStructure))
	{
		SpawnedStructure->SetBuildStructureID(InItemID);
		SpawnedStructure->SetReplicates(false);
	}
}

void UCBuildComponent::BuildStartFoundation()
{
	if (IsValid(SpawnedFoundation))
	{
		FVector structureLocation;
		FRotator structureRotation;

		SpawnedFoundation->CheckHeight();
		SpawnedFoundation->CheckCenter();

		bIsBuildable = ((SpawnedFoundation->GetFoundationHeightHit()) && (!SpawnedFoundation->GetFoundationCenterHit()));
		if (bIsBuildable)
		{
			SpawnedFoundation->CheckRight();
			if (SpawnedFoundation->GetFoundationRightHit())
				bIsSnapped = SpawnedFoundation->GetFoundationRightHit();
			else
			{
				SpawnedFoundation->CheckLeft();
				if (SpawnedFoundation->GetFoundationLeftHit())
					bIsSnapped = SpawnedFoundation->GetFoundationLeftHit();
				else
				{
					SpawnedFoundation->CheckBackward();
					if (SpawnedFoundation->GetFoundationBackwardHit())
						bIsSnapped = SpawnedFoundation->GetFoundationBackwardHit();
					else
					{
						SpawnedFoundation->CheckForward();
						if (SpawnedFoundation->GetFoundationForwardHit())
							bIsSnapped = SpawnedFoundation->GetFoundationForwardHit();
					}
				}
			}
		}

		if (bIsSnapped)
		{
			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f;
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;
			
			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);

			if (!tempBool)
			{
				bIsSnapped = false;
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = SpawnedFoundation->GetFoundationHeight();
				SpawnedFoundation->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation();
				SpawnedFoundation->SetActorRotation(structureRotation);
			}
		}
		else
		{
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
			structureLocation.Z = SpawnedFoundation->GetFoundationHeight();
			SpawnedFoundation->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation();
			SpawnedFoundation->SetActorRotation(structureRotation);
		}

		if (bIsBuildable && SpawnedFoundation->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			//CDebug::Print("Change To GreenMaterial");
			SpawnedFoundation->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedFoundation->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			//CDebug::Print("Change To RedMaterial");
			SpawnedFoundation->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartWall()
{
	if (IsValid(SpawnedWall))
	{
		FVector structureLocation;
		FRotator structureRotation;

		if (!bIsSnapped)
		{
			SpawnedWall->CheckDown_Foundation();
			if (SpawnedWall->GetWallDown_FoundationHit())
				bIsSnapped = SpawnedWall->GetWallDown_FoundationHit();
			else
			{
				SpawnedWall->CheckDown_Wall();
				if (SpawnedWall->GetWallDown_WallHit())
					bIsSnapped = SpawnedWall->GetWallDown_WallHit();
				else
				{
					SpawnedWall->CheckDown_DoorFrame();
					if (SpawnedWall->GetWallDown_DoorFrameHit())
						bIsSnapped = SpawnedWall->GetWallDown_DoorFrameHit();
					else
					{
						SpawnedWall->CheckDown_Ceiling();
						if (SpawnedWall->GetWallDown_CeilingHit())
							bIsSnapped = SpawnedWall->GetWallDown_CeilingHit();
					}
				}
			}
		}

		if (bIsSnapped)
		{
			SpawnedWall->CheckCenter();
			if (!SpawnedWall->GetWallCenterHit())
				bIsBuildable = (!SpawnedWall->GetWallCenterHit());
			else
			{
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedWall->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedWall->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}

			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f;
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;
			
			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);

			if (!tempBool)
			{
				// Preview Box를 벗어났을 때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedWall->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedWall->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
		}
		else
		{
			// DownHit로 Foundation을 못 찾았을 때
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
			if (Survivor->GetControlRotation().Vector().Z > 0)
				structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f + 100.0f;
			else
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
			SpawnedWall->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
			SpawnedWall->SetActorRotation(structureRotation);
			bIsBuildable = false;
		}
	
		if (bIsBuildable && SpawnedWall->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			SpawnedWall->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedWall->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedWall->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartCeiling()
{
	if (IsValid(SpawnedCeiling))
	{
		FVector structureLocation;
		FRotator structureRotation;

		if (!bIsSnapped)
		{
			SpawnedCeiling->CheckRight_Wall();
			if (SpawnedCeiling->GetCeilingRight_WallHit())
				bIsSnapped = SpawnedCeiling->GetCeilingRight_WallHit();
			else
			{
				SpawnedCeiling->CheckLeft_Wall();
				if (SpawnedCeiling->GetCeilingLeft_WallHit())
					bIsSnapped = SpawnedCeiling->GetCeilingLeft_WallHit();
				else
				{
					SpawnedCeiling->CheckBackward_Wall();
					if (SpawnedCeiling->GetCeilingBackward_WallHit())
						bIsSnapped = SpawnedCeiling->GetCeilingBackward_WallHit();
					else
					{
						SpawnedCeiling->CheckForward_Wall();
						if (SpawnedCeiling->GetCeilingForward_WallHit())
							bIsSnapped = SpawnedCeiling->GetCeilingForward_WallHit();
						else
						{
							SpawnedCeiling->CheckRight_Ceiling();
							if (SpawnedCeiling->GetCeilingRight_CeilingHit())
								bIsSnapped = SpawnedCeiling->GetCeilingRight_CeilingHit();
							else
							{
								SpawnedCeiling->CheckLeft_Ceiling();
								if (SpawnedCeiling->GetCeilingLeft_CeilingHit())
									bIsSnapped = SpawnedCeiling->GetCeilingLeft_CeilingHit();
								else
								{
									SpawnedCeiling->CheckBackward_Ceiling();
									if (SpawnedCeiling->GetCeilingBackward_CeilingHit())
										bIsSnapped = SpawnedCeiling->GetCeilingBackward_CeilingHit();
									else
									{
										SpawnedCeiling->CheckForward_Ceiling();
										if (SpawnedCeiling->GetCeilingForward_CeilingHit())
											bIsSnapped = SpawnedCeiling->GetCeilingForward_CeilingHit();
									}
								}
							}
						}
					}
				}
			}
		}

		if (bIsSnapped)
		{
			SpawnedCeiling->CheckCenter();
			if (!SpawnedCeiling->GetCeilingCenterHit())
			{
				bIsBuildable = true;
			}
			else
			{
				// Hit로 Wall을 찾았지만 이미 다른구조물이나 무언가가있을때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;

				if (Survivor->GetControlRotation().Vector().Z > 0)
					structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f;
				else
					structureLocation.Z = Survivor->GetActorLocation().Z - 100.0f;

				SpawnedCeiling->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation();
				SpawnedCeiling->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
			
			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f + FVector(0, 0, 200);
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;
			
			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);
			
			if (!tempBool)
			{
				// Preview Box를 벗어났을 때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;

				if (Survivor->GetControlRotation().Vector().Z > 0)
					structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f;
				else
					structureLocation.Z = Survivor->GetActorLocation().Z - 100.0f;

				SpawnedCeiling->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation();
				SpawnedCeiling->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
		}
		else
		{
			// Wall을 못 찾았을 때
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;

			if (Survivor->GetControlRotation().Vector().Z > 0)
				structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f;
			else
				structureLocation.Z = Survivor->GetActorLocation().Z - 100.0f;

			SpawnedCeiling->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation();
			SpawnedCeiling->SetActorRotation(structureRotation);
			bIsBuildable = false;
		}

		if (bIsBuildable && SpawnedCeiling->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			SpawnedCeiling->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedCeiling->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedCeiling->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartRamp()
{
	if (IsValid(SpawnedRamp))
	{
		FVector structureLocation;
		FRotator structureRotation;

		SpawnedRamp->CheckHeight();

		if (!bIsSnapped)
		{
			SpawnedRamp->CheckForward();
			if (SpawnedRamp->GetRampForwardHit())
				bIsSnapped = SpawnedRamp->GetRampForwardHit();
		}

		if (bIsSnapped)
		{
			SpawnedRamp->CheckCenter();
			if (!SpawnedRamp->GetRampCenterHit())
			{
				bIsBuildable = true;
			}
			else
			{
				// Hit로 Foundation을 찾았지만 이미 다른구조물이나 무언가가있을때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = SpawnedRamp->GetRampHeight();

				SpawnedRamp->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);;
				SpawnedRamp->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}

			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f + FVector(0, 0, 200);
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;
			
			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);
			
			if (!tempBool)
			{
				// Preview Box를 벗어났을 때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = SpawnedRamp->GetRampHeight();
			
				SpawnedRamp->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);;
				SpawnedRamp->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
		}
		else
		{
			// Foundation을 못 찾았을 때
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
			structureLocation.Z = SpawnedRamp->GetRampHeight();

			SpawnedRamp->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
			SpawnedRamp->SetActorRotation(structureRotation);
			bIsBuildable = false;
		}

		if (bIsBuildable && SpawnedRamp->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			SpawnedRamp->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedRamp->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedRamp->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartDoorFrame()
{
	if (IsValid(SpawnedDoorFrame))
	{
		FVector structureLocation;
		FRotator structureRotation;

		if (!bIsSnapped)
		{
			SpawnedDoorFrame->CheckDown_Foundation();
			if (SpawnedDoorFrame->GetDoorFrameDown_FoundationHit())
				bIsSnapped = SpawnedDoorFrame->GetDoorFrameDown_FoundationHit();
			else
			{
				SpawnedDoorFrame->CheckDown_Ceiling();
				if (SpawnedDoorFrame->GetDoorFrameDown_CeilingHit())
					bIsSnapped = SpawnedDoorFrame->GetDoorFrameDown_CeilingHit();
			}
		}

		if (bIsSnapped)
		{
			SpawnedDoorFrame->CheckCenter();
			if (!SpawnedDoorFrame->GetDoorFrameCenterHit())
				bIsBuildable = (!SpawnedDoorFrame->GetDoorFrameCenterHit());
			else
			{
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedDoorFrame->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedDoorFrame->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}

			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f;
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;

			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);

			if (!tempBool)
			{
				// Preview Box를 벗어났을 때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedDoorFrame->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedDoorFrame->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
		}
		else
		{
			// DownHit로 Foundation을 못 찾았을 때
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
			if (Survivor->GetControlRotation().Vector().Z > 0)
				structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f + 100.0f;
			else
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
			SpawnedDoorFrame->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
			SpawnedDoorFrame->SetActorRotation(structureRotation);
			bIsBuildable = false;
		}

		if (bIsBuildable && SpawnedDoorFrame->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			SpawnedDoorFrame->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedDoorFrame->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedDoorFrame->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartDoor()
{
	if (IsValid(SpawnedDoor))
	{
		FVector structureLocation;
		FRotator structureRotation;

		if (!bIsSnapped)
		{
			SpawnedDoor->CheckUp_DoorFrame();
			if (SpawnedDoor->GetDoorUp_DoorFrameHit())
				bIsSnapped = SpawnedDoor->GetDoorUp_DoorFrameHit();
		}

		if (bIsSnapped)
		{
			SpawnedDoor->CheckCenter();
			if (!SpawnedDoor->GetDoorCenterHit())
			{
				bIsBuildable = (!SpawnedDoor->GetDoorCenterHit());
			}
			else
			{
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedDoor->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedDoor->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}

			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f;
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;
			
			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);

			if (!tempBool)
			{
				// Preview Box를 벗어났을 때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedDoor->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedDoor->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
		}
		else
		{
			// DownHit로 Foundation을 못 찾았을 때
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
			if (Survivor->GetControlRotation().Vector().Z > 0)
				structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f + 100.0f;
			else
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
			SpawnedDoor->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
			SpawnedDoor->SetActorRotation(structureRotation);
			bIsBuildable = false;
		}

		if (bIsBuildable && SpawnedDoor->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			SpawnedDoor->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedDoor->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedDoor->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartStair()
{
	if (IsValid(SpawnedStair))
	{
		FVector structureLocation;
		FRotator structureRotation;

		if (!bIsSnapped)
		{
			SpawnedStair->CheckDown_Foundation();
			if (SpawnedStair->GetStairDown_FoundationHit())
				bIsSnapped = SpawnedStair->GetStairDown_FoundationHit();
		}

		if (bIsSnapped)
		{
			SpawnedStair->CheckCenter();
			if (!SpawnedStair->GetStairCenterHit())
				bIsBuildable = (!SpawnedStair->GetStairCenterHit());
			else
			{
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedStair->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedStair->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}

			TArray<FHitResult> tempHitResults;
			FVector tempStartLocation = Survivor->GetActorLocation();
			FVector tempEndLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 750.0f;
			TArray<TEnumAsByte<EObjectTypeQuery>> tempObjectTypeQuery;
			tempObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			TArray<AActor*> tempActorsIgnore;
			FCollisionObjectQueryParams tempObjectQueryParams;
			FCollisionQueryParams tempQueryParams;
			
			bool tempBool = UKismetSystemLibrary::LineTraceMultiForObjects(
				GetWorld(),
				tempStartLocation,
				tempEndLocation,
				tempObjectTypeQuery,
				false,
				tempActorsIgnore,
				EDrawDebugTrace::None,
				tempHitResults,
				true,
				FLinearColor::Green,
				FLinearColor::Red
			);
			
			if (!tempBool)
			{
				// Preview Box를 벗어났을 때
				structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				SpawnedStair->SetActorLocation(structureLocation);
				structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				SpawnedStair->SetActorRotation(structureRotation);
				bIsSnapped = false;
				bIsBuildable = false;
			}
		}
		else
		{
			// DownHit로 Foundation을 못 찾았을 때
			structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
			structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
			if (Survivor->GetControlRotation().Vector().Z > 0)
				structureLocation.Z = Survivor->GetActorLocation().Z + Survivor->GetControlRotation().Vector().Z * 1000.0f + 100.0f;
			else
				structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
			SpawnedStair->SetActorLocation(structureLocation);
			structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
			SpawnedStair->SetActorRotation(structureRotation);
			bIsBuildable = false;
		}

		if (bIsBuildable && SpawnedStair->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			SpawnedStair->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedStair->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedStair->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}

void UCBuildComponent::BuildStartPlaceable()
{
	if (IsValid(SpawnedPlaceable))
	{
		FVector structureLocation;
		FRotator structureRotation;

		if (SpawnedPlaceable->CheckIsBuildOnLandScape())
		{
			SpawnedPlaceable->CheckHeight();
			SpawnedPlaceable->CheckCenter();

			bIsBuildable = ((SpawnedPlaceable->GetPlaceableLandScapeHit()) && (!SpawnedPlaceable->GetPlaceableCenterHit()));
		}
		else
		{
			SpawnedPlaceable->CheckDown_FoundationAndCeiling();
			if (SpawnedPlaceable->GetPlaceableDown_FoundationAndCeilingHit())
			{
				SpawnedPlaceable->CheckCenter();
				bIsBuildable = !SpawnedPlaceable->GetPlaceableCenterHit();
			}
			else
				bIsBuildable = false;
		}

		structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
		structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
		structureLocation.Z = SpawnedPlaceable->GetPlaceableHeight();
		SpawnedPlaceable->SetActorLocation(structureLocation);
		structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
		SpawnedPlaceable->SetActorRotation(structureRotation);

		if (bIsBuildable && SpawnedPlaceable->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			//CDebug::Print("Change To GreenMaterial");
			SpawnedPlaceable->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedPlaceable->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			//CDebug::Print("Change To RedMaterial");
			SpawnedPlaceable->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}

		
	}
}

void UCBuildComponent::PerformBuild(TSubclassOf<ACStructure> InClass, FTransform InTransform)
{
	if (IsValid(InClass))
	{
		ACStructure* buildstructure = GetWorld()->SpawnActor<ACStructure>(InClass, InTransform);
		buildstructure->BroadcastDestroyPreviewBox();
		ACStructure_Placeable* placeableStructure = Cast<ACStructure_Placeable>(buildstructure);
		if (placeableStructure)
		{
			if (placeableStructure->GetPlaceableStructureType() == EPlaceableStructureType::Bed)
			{
				//placeableStructure->SetReplicates(true);
				class UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
				if (gameInstance)
					gameInstance->WorldMap->SetActorOnWorldMap(placeableStructure);
			}
		}
		bIsSnapped = false;

		//if (buildstructure || (placeableStructure && placeableStructure->GetPlaceableStructureType() != EPlaceableStructureType::Bed))
		//{
		//	UGameplayStatics::SpawnSoundAtLocation(
		//		GetWorld(),
		//		BuildSound,
		//		buildstructure->GetActorLocation(),
		//		buildstructure->GetActorRotation(),
		//		1.0f,
		//		1.0f,
		//		0.0f,
		//		BuildSoundAttenuation
		//	);
		//}
	}
	else
	{
		CDebug::Print("InClass Is Not Valid");
	}
}

void UCBuildComponent::RequestBuild_Implementation(TSubclassOf<ACStructure> InClass, FTransform InTransform)
{
	if (IsValid(InClass))
	{
		PerformBuild(InClass, InTransform);
	}
	else
	{
		CDebug::Print("InClass Is Not Valid", FColor::Silver);
		PRINTLINE_DETAIL(FColor::Silver, 10.0f);
	}
}

bool UCBuildComponent::RequestBuild_Validate(TSubclassOf<ACStructure> InClass, FTransform InTransform)
{
	return true;
}

void UCBuildComponent::PerformCreateBackPack(TSubclassOf<ACStructure> InClass, FTransform InTransform)
{
	if (IsValid(InClass))
	{
		FTransform spawnTransform;
		spawnTransform.SetLocation(InTransform.GetLocation() - FVector(0, 0, 60));
		spawnTransform.SetRotation(InTransform.GetRotation());

		ACStructure* buildstructure = GetWorld()->SpawnActor<ACStructure>(InClass, spawnTransform);
		buildstructure->BroadcastDestroyPreviewBox();
		ACStructure_Placeable* placeableStructure = Cast<ACStructure_Placeable>(buildstructure);
		if (placeableStructure)
		{
			placeableStructure->SetReplicates(true);
			ReceiveSpawnedActor(placeableStructure);
		}
	}
	else
	{
		CDebug::Print("InClass Is Not Valid");
	}
}

void UCBuildComponent::RequestCreateBackPack_Implementation(TSubclassOf<ACStructure> InClass, FTransform InTransform)
{
	if (IsValid(InClass))
	{
		PerformCreateBackPack(InClass, InTransform);
	}
}

void UCBuildComponent::ReceiveSpawnedActor_Implementation(class ACStructure_Placeable* SpawnedActor)
{
	if (SpawnedActor)
	{
		//CDebug::Print("Clinet Function Called And Spawnd Actor Is Valid : ", SpawnedActor);

		ACSurvivorController* survivorController = Cast<ACSurvivorController>(this->GetOwner()->GetInstigatorController());
		
		if (survivorController)
		{

			for (TWeakObjectPtr<UCItemBase> tempPtr : Survivor->GetInventoryComponent()->GetInventoryContents())
			{
				if (tempPtr.IsValid())
				{
					survivorController->RequestAddItem(tempPtr.Get()->ID, tempPtr.Get()->Quantity, SpawnedActor, tempPtr.Get()->NumericData, tempPtr.Get()->ItemType, tempPtr.Get()->ItemStats, tempPtr.Get()->HuntData.WeaponType);
					Survivor->GetInventoryComponent()->RemoveSingleItem(tempPtr.Get());
				}
			}

			ACMainHUD* mainHUD = Cast<ACMainHUD>(survivorController->GetHUD());
			if (mainHUD)
			{
				for (USizeBox* tempSizeBox : mainHUD->GetQuickSlotWidget()->GetSizeBoxArray())
				{
					if (tempSizeBox)
					{
						UCInventoryItemSlot* itemSlot = Cast<UCInventoryItemSlot>(tempSizeBox->GetChildAt(0));
						if (itemSlot)
						{
							if (itemSlot->GetItemReference()->ItemType == EItemType::Hunt)
							{
								survivorController->RequestAddItem(itemSlot->GetItemReference()->ID, 1, SpawnedActor, itemSlot->GetItemReference()->NumericData, itemSlot->GetItemReference()->ItemType, itemSlot->GetItemReference()->ItemStats, itemSlot->GetItemReference()->HuntData.WeaponType);
								itemSlot->RemoveFromParent();
								itemSlot->MarkPendingKill();
							}
						}
					}
				}
			}

			ACSurvivor* survivor = Cast<ACSurvivor>(survivorController->GetPawn());
			if (survivor)
			{
				EWeaponType weaponType = survivor->GetWeaponComponent()->GetUsingWeapon()->HuntData.WeaponType;
				if (survivor->GetWeaponComponent()->GetWeaponType() != EWeaponType::Max)
					survivor->GetWeaponComponent()->SetMode(weaponType);
			}
		}
	}
}

void UCBuildComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCBuildComponent, SpawnedStructure);
}