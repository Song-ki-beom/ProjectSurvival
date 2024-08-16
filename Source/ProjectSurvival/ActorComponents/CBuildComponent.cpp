#include "ActorComponents/CBuildComponent.h"
#include "Build/CStructure_Foundation.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Widget/Build/CBuildWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		break;
	case EBuildStructureElement::WindowWall:
		break;
	case EBuildStructureElement::TriLeftWall:
		break;
	case EBuildStructureElement::TriRightWall:
		break;
	case EBuildStructureElement::TriTopWall:
		break;
	case EBuildStructureElement::Ceiling:
		break;
	case EBuildStructureElement::TriCeiling:
		break;
	case EBuildStructureElement::Roof:
		break;
	case EBuildStructureElement::HalfRoof:
		break;
	case EBuildStructureElement::DoorFrame:
		break;
	case EBuildStructureElement::Door:
		break;
	case EBuildStructureElement::Fence:
		break;
	case EBuildStructureElement::Ramp:
		break;
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

void UCBuildComponent::SelectW()
{
	CDebug::Print("SelectW");
}

void UCBuildComponent::SelectE()
{
	CDebug::Print("SelectE");
}

void UCBuildComponent::SelectA()
{
	CDebug::Print("SelectA");
}

void UCBuildComponent::SelectS()
{
	CDebug::Print("SelectS");
}

void UCBuildComponent::SelectD()
{
	CDebug::Print("SelectD");
}

void UCBuildComponent::SelectZ()
{
	CDebug::Print("SelectZ");
}

void UCBuildComponent::SelectX()
{
	CDebug::Print("SelectX");
}

void UCBuildComponent::SelectC()
{
	CDebug::Print("SelectC");
}

void UCBuildComponent::BuildSpawnedStructure()
{
	if (!bIsBuildable)
	{
		CDebug::Print("You Can't Build There", FColor::Cyan);
		return;
	}
	ACStructure* buildstructure = GetWorld()->SpawnActor<ACStructure>(SpawnedStructure->GetClass(), SpawnedStructure->GetActorLocation(), SpawnedStructure->GetActorRotation());
	DestroyChildComponent(buildstructure, StructureElement);
	bIsSnapped = false;
}

void UCBuildComponent::ClearSpawnedStructure()
{
	if (SpawnedStructure)
		SpawnedStructure->Destroy();
	bIsSnapped = false;
}

void UCBuildComponent::SpawnBuildStructureElement(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	StructureElement = InElement;

	switch (StructureElement)
	{
	case EBuildStructureElement::Foundation:
	{
		if (IsValid(SpawnedFoundation))
			SpawnedFoundation->Destroy();
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedFoundation = GetWorld()->SpawnActor<ACStructure_Foundation>(InClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedFoundation->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnedStructure = Cast<ACStructure>(SpawnedFoundation);
		break;
	}
	case EBuildStructureElement::TriFoundation:
		break;
	case EBuildStructureElement::Wall:
		break;
	case EBuildStructureElement::WindowWall:
		break;
	case EBuildStructureElement::TriLeftWall:
		break;
	case EBuildStructureElement::TriRightWall:
		break;
	case EBuildStructureElement::TriTopWall:
		break;
	case EBuildStructureElement::Ceiling:
		break;
	case EBuildStructureElement::TriCeiling:
		break;
	case EBuildStructureElement::Roof:
		break;
	case EBuildStructureElement::HalfRoof:
		break;
	case EBuildStructureElement::DoorFrame:
		break;
	case EBuildStructureElement::Door:
		break;
	case EBuildStructureElement::Fence:
		break;
	case EBuildStructureElement::Ramp:
		break;
	case EBuildStructureElement::Stair:
		break;
	case EBuildStructureElement::None:
		break;
	default:
		break;
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
			bIsSnapped = SpawnedFoundation->GetFoundationRightHit();
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

void UCBuildComponent::DestroyChildComponent(ACStructure* InStructure, EBuildStructureElement InElement)
{
	switch (InElement)
	{
	case EBuildStructureElement::Foundation:
	{
		ACStructure_Foundation* structure_Foundation = Cast<ACStructure_Foundation>(InStructure);
		structure_Foundation->DestroyPreviewBox();
	}
	case EBuildStructureElement::TriFoundation:
		break;
	case EBuildStructureElement::Wall:
		break;
	case EBuildStructureElement::WindowWall:
		break;
	case EBuildStructureElement::TriLeftWall:
		break;
	case EBuildStructureElement::TriRightWall:
		break;
	case EBuildStructureElement::TriTopWall:
		break;
	case EBuildStructureElement::Ceiling:
		break;
	case EBuildStructureElement::TriCeiling:
		break;
	case EBuildStructureElement::Roof:
		break;
	case EBuildStructureElement::HalfRoof:
		break;
	case EBuildStructureElement::DoorFrame:
		break;
	case EBuildStructureElement::Door:
		break;
	case EBuildStructureElement::Fence:
		break;
	case EBuildStructureElement::Ramp:
		break;
	case EBuildStructureElement::Stair:
		break;
	case EBuildStructureElement::None:
		break;
	default:
		break;
	}
}
