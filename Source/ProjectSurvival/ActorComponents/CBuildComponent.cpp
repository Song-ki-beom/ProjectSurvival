#include "ActorComponents/CBuildComponent.h"
#include "Character/CSurvivor_BuildTest.h"
#include "Character/CSurvivorController_BuildTest.h"
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
	ACSurvivorController_BuildTest* survivorController = Cast<ACSurvivorController_BuildTest>(GetWorld()->GetFirstPlayerController());
	BuildWidget = survivorController->GetBuildWidget();
	if (survivorController)
		CDebug::Print("Build Widget Valid");
	else
		CDebug::Print("Build Widget is not Vaild");

	Survivor = Cast<ACSurvivor_BuildTest>(this->GetOwner());
}

void UCBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsValid(SpawnedStructure))
	{
		FVector beforeLocation =SpawnedStructure->GetActorLocation();

		
		FVector targetLocation = Survivor->GetActorLocation() + Survivor->GetControlRotation().Vector() * 500.0f - FVector(0, 0, 50);
		//FRotator targetRotation = Survivor->GetActorRotation();


		FVector newLocation = FMath::VInterpTo(SpawnedStructure->GetActorLocation(), targetLocation, DeltaTime, 100.0f);
		//FRotator newRotation = FMath::RInterpTo(SpawnedStructure->GetActorRotation(), targetRotation, DeltaTime, 100.0f);

		SpawnedStructure->SetActorLocation(newLocation);
		SpawnedStructure->SetActorRelativeRotation(Survivor->GetActorRotation());

		if (bIsBuildable && SpawnedStructure->GetStaticMesh()->GetMaterial(0) != GreenMaterial)
		{
			CDebug::Print("Change To GreenMaterial");
			SpawnedStructure->GetStaticMesh()->SetMaterial(0, GreenMaterial);
		}
		if (!bIsBuildable && SpawnedStructure->GetStaticMesh()->GetMaterial(0) != RedMaterial)
		{
			SpawnedStructure->GetStaticMesh()->SetMaterial(0, RedMaterial);
		}
		CDebug::Print("Material : ", SpawnedStructure->GetOriginMaterial());
	}
}

void UCBuildComponent::SelectQ(TSubclassOf<ACStructure> InClass)
{
	CDebug::Print("SelectQ");
	FVector spawnLocation = Survivor->GetActorLocation();
	FRotator spawnRotation = Survivor->GetActorRotation();
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = Survivor;
	spawnParams.Instigator = Survivor->GetInstigator();
	
	SpawnedStructure = GetWorld()->SpawnActor<ACStructure>(InClass, spawnLocation, spawnRotation, spawnParams);
	bIsBuildable = true;
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
