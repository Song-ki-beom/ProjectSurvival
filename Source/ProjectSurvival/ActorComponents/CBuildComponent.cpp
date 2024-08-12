#include "ActorComponents/CBuildComponent.h"
#include "Build/CStructure_Foundation.h"
#include "Character/CSurvivor.h"
#include "Character/CSurvivorController.h"
#include "Widget/Build/CBuildWidget.h"
#include "Utility/CDebug.h"

UCBuildComponent::UCBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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
	ACSurvivorController* survivorController = Cast<ACSurvivorController>(GetWorld()->GetFirstPlayerController());
	BuildWidget = survivorController->GetBuildWidget();
	if (survivorController)
		CDebug::Print("Build Widget Valid");
	else
		CDebug::Print("Build Widget is not Vaild");

	Survivor = Cast<ACSurvivor>(this->GetOwner());
}

void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	//bIsBuildable = true;
	//SpawnedStructure->AttachToComponent(Survivor->GetCameraComponent(), FAttachmentTransformRules::KeepWorldTransform);
	//SpawnedStructure->GetStaticMesh()->SetMaterial(0, GreenMaterial);
	//SpawnedStructure->SaveOrigin

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

void UCBuildComponent::SpawnBuildStructureElement(TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	StructureElement = InElement;

	switch (StructureElement)
	{
	case EBuildStructureElement::Foundation:
	{
		FVector spawnLocation = Survivor->GetActorLocation();
		FRotator spawnRotation = Survivor->GetActorRotation();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = Survivor;
		spawnParams.Instigator = Survivor->GetInstigator();
		SpawnedFoundation = GetWorld()->SpawnActor<ACStructure_Foundation>(InClass, spawnLocation, spawnRotation, spawnParams);
		SpawnedFoundation->GetStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		float structureLocationX = Survivor->GetActorLocation().X + Survivor->GetControlRotation().Vector().X * 500.0f;
		float structureLocationY = Survivor->GetActorLocation().Y + Survivor->GetControlRotation().Vector().Y * 500.0f;
		float structureLocationZ;

		SpawnedFoundation->GetFloorHeight(structureLocationZ, bIsBuildable);

		SpawnedFoundation->SetActorLocation(FVector(structureLocationX, structureLocationY, structureLocationZ));
		SpawnedFoundation->SetActorRelativeRotation(Survivor->GetActorRotation());

		if (bIsBuildable && SpawnedFoundation->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			CDebug::Print("Change To GreenMaterial");
			SpawnedFoundation->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedFoundation->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			CDebug::Print("Change To RedMaterial");
			SpawnedFoundation->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
	}
}
