#include "ActorComponents/CBuildComponent.h"
#include "Build/CStructure_Foundation.h"
#include "Build/CStructure_Wall.h"
#include "Build/CStructure_Ceiling.h"
#include "Build/CStructure_Ramp.h"
#include "Build/CStructure_DoorFrame.h"
#include "Build/CStructure_Door.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Widget/Build/CBuildWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
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
	ACSurvivorController* survivorController = Cast<ACSurvivorController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(survivorController) && IsValid(BuildWidget))
	{
		BuildWidget = survivorController->GetBuildWidget();
	}
}

void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(SpawnedStructure))
	{
		if (!bIsBuilding)
		{
			bIsBuilding = true;
			CDebug::Print("Set bIsBuilding : ", bIsBuilding, FColor::Red);
		}
	}
	else
	{
		if (bIsBuilding)
		{
			bIsBuilding = false;
			CDebug::Print("Set bIsBuilding : ", bIsBuilding, FColor::Red);
		}
	}

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
		break;
	case EBuildStructureElement::None:
		break;
	default:
		break;
	}
}

void UCBuildComponent::SelectQ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectQ");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectW(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectW");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectE(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectE");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectA(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectA");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectS(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectS");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectD(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectD");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectZ(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectZ");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectX(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectX");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::SelectC(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	CDebug::Print("SelectC");

	SpawnBuildStructureElement(InClass, InElement);
}

void UCBuildComponent::BuildSpawnedStructure()
{
	if (!bIsBuildable)
	{
		CDebug::Print("You Can't Build There", FColor::Cyan);
		return;
	}

	if (IsValid(SpawnedStructure))
		SpawnedStructure->SetReplicates(true);

	if (Survivor->HasAuthority())
	{
		StructureTransform = SpawnedStructure->GetActorTransform();
		PerformBuild(StructureClass, StructureTransform);
	}
	else
	{
		CDebug::Print("Spawned Structure : ", SpawnedStructure);
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

void UCBuildComponent::SpawnBuildStructureElement(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
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
		break;
	case EBuildStructureElement::None:
		break;
	default:
		break;
	}

	if(IsValid(SpawnedStructure))
		SpawnedStructure->SetReplicates(false);
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
				EDrawDebugTrace::Persistent,
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
				// DownHit로 Foundation을 찾았지만 이미 다른구조물이나 무언가가있을때
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
				EDrawDebugTrace::Persistent,
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
			SpawnedCeiling->CheckRight();
			if (SpawnedCeiling->GetCeilingRightHit())
				bIsSnapped = SpawnedCeiling->GetCeilingRightHit();
			else
			{
				SpawnedCeiling->CheckLeft();
				if (SpawnedCeiling->GetCeilingLeftHit())
					bIsSnapped = SpawnedCeiling->GetCeilingLeftHit();
				else
				{
					SpawnedCeiling->CheckBackward();
					if (SpawnedCeiling->GetCeilingBackwardHit())
						bIsSnapped = SpawnedCeiling->GetCeilingBackwardHit();
					else
					{
						SpawnedCeiling->CheckForward();
						if (SpawnedCeiling->GetCeilingForwardHit())
							bIsSnapped = SpawnedCeiling->GetCeilingForwardHit();
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
				
				//structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				//structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				//structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				//SpawnedWall->SetActorLocation(structureLocation);
				//structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				//SpawnedWall->SetActorRotation(structureRotation);
				//bIsSnapped = false;
				//bIsBuildable = false;
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
			
				//structureLocation.X = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
				//structureLocation.Y = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
				//structureLocation.Z = Survivor->GetActorLocation().Z + 100.0f;
				//SpawnedWall->SetActorLocation(structureLocation);
				//structureRotation = Survivor->GetActorRotation() + FRotator(0, 90, 0);
				//SpawnedWall->SetActorRotation(structureRotation);
				//bIsSnapped = false;
				//bIsBuildable = false;
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
		}

		if (bIsSnapped)
		{
			SpawnedDoorFrame->CheckCenter();
			if (!SpawnedDoorFrame->GetDoorFrameCenterHit())
				bIsBuildable = (!SpawnedDoorFrame->GetDoorFrameCenterHit());
			else
			{
				// DownHit로 Foundation을 찾았지만 이미 다른구조물이나 무언가가있을때
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
				EDrawDebugTrace::Persistent,
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
				bIsBuildable = (!SpawnedDoor->GetDoorCenterHit());
			else
			{
				// DownHit로 Foundation을 찾았지만 이미 다른구조물이나 무언가가있을때
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
				EDrawDebugTrace::Persistent,
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

void UCBuildComponent::PerformBuild(TSubclassOf<ACStructure> InClass, FTransform InTransform)
{
	if (IsValid(InClass))
	{
		ACStructure* buildstructure = GetWorld()->SpawnActor<ACStructure>(InClass, InTransform);
		buildstructure->BroadcastDestroyPreviewBox();
		bIsSnapped = false;
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

void UCBuildComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCBuildComponent, SpawnedStructure);
}