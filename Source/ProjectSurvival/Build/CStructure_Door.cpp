#include "Build/CStructure_Door.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/CSurvivorController.h"
#include "Components/ArrowComponent.h"
#include "Widget/Map/CWorldMap.h"
#include "Widget/Inventory/CItemBase.h"
#include "CGameInstance.h"
#include "Engine/PackageMapClient.h"
#include "ActorComponents/CInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CDebug.h"

ACStructure_Door::ACStructure_Door()
{
	TempRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TempRoot"));
	SetRootComponent(TempRoot);

	PivotArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PivotArrow"));
	PivotArrow->SetupAttachment(TempRoot);
	PivotArrow->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	PickupMesh->SetupAttachment(PivotArrow);
	PickupMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	//DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	//DoorMesh->SetupAttachment(PivotArrow);
	//DoorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//DoorMesh->SetVisibility(false);
}

void ACStructure_Door::BeginPlay()
{
	Super::BeginPlay();

	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		if (GetWorld()->GetNetDriver() && GetWorld()->GetNetDriver()->GuidCache)
		{
			UObject* foundObject = GetWorld()->GetNetDriver()->GuidCache->GetObjectFromNetGUID(gameInstance->WorldMap->GetPersonalGUID(), true);
			if (foundObject)
			{
				APawn* personalPawn = Cast<APawn>(foundObject);
				if (personalPawn)
				{
					PersonalSurvivorController = Cast<ACSurvivorController>(personalPawn->GetController());
				}
			}
		}
	}

	//ItemReference->BuildData.bIsDoorOpened = false;
}
void ACStructure_Door::Tick(float DeltaSeconds)
{
	//
	//// 액터의 현재 위치
	//FVector ActorLocation = GetActorLocation();
	//
	//FVector PivotPoint = GetActorLocation() + FVector(50, 0, 0);
	//
	//// 피벗과 액터 사이의 오프셋 계산
	//FVector Offset = ActorLocation - PivotPoint;
	//
	//// 새 회전 각도 계산
	//FRotator NewRotation = FRotator(0.f, Angle, 0.f); // 예시로 Yaw 축을 기준으로 회전
	//
	//// 회전된 오프셋 적용 (피벗을 기준으로 회전)
	//FVector RotatedOffset = NewRotation.RotateVector(Offset);
	//
	//// 액터의 새 위치 설정
	////FVector NewLocation = PivotPoint + RotatedOffset;
	//
	//// 액터 이동 및 회전
	////SetActorLocation(NewLocation);
	//SetActorRotation(NewRotation);
}

void ACStructure_Door::CheckCenter()
{
	FHitResult centerBoxHitResult;
	FVector centerBoxLocation = this->GetActorLocation() - FVector(0, 0, 50);
	FVector centerBoxSize = FVector(50, 5, 75);
	FRotator centerBoxOrientation;
	if (!bUp_DoorFrameHit)
		centerBoxOrientation = GetOwner()->GetActorRotation();
	else
		centerBoxOrientation = CenterRotation;
	ETraceTypeQuery centerBoxTraceTypeQuery = ETraceTypeQuery::TraceTypeQuery3;
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
		EDrawDebugTrace::None,
		centerBoxHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);
}

void ACStructure_Door::CheckUp_DoorFrame()
{
	FHitResult upHitResult;
	FVector upStartLocation = this->GetActorLocation();
	FVector upEndLocation = this->GetActorLocation() + this->GetActorUpVector() * 300.0f;
	TArray<TEnumAsByte<EObjectTypeQuery>> upObjectTypeQuery;
	upObjectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));

	TArray<AActor*> upBoxActorsToIgnore;
	upBoxActorsToIgnore.Add(this);

	bUp_DoorFrameHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		upStartLocation,
		upEndLocation,
		upObjectTypeQuery,
		false,
		upBoxActorsToIgnore,
		EDrawDebugTrace::None,
		upHitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	if (bUp_DoorFrameHit)
	{
		this->SetActorLocation(upHitResult.GetComponent()->GetComponentLocation() + upHitResult.GetComponent()->GetForwardVector() * 15.0f + upHitResult.ImpactNormal * 120.0f);
		CenterRotation = upHitResult.GetComponent()->GetComponentRotation() + FRotator(0, -90, 0);
		this->SetActorRotation(CenterRotation);
	}
}

void ACStructure_Door::DoBuildTypeInteract()
{
	//CDebug::Print("DoBuildTypeInteract Called");

	if (this->HasAuthority())
	{
		if (bIsOpened)
			BroadcastCloseDoor();
		else
			BroadcastOpenDoor();
	}
	else
	{
		if (bIsOpened)
		{
			if (PersonalSurvivorController)
				PersonalSurvivorController->RequestCloseDoor(this);
		}
		else
		{
			if (PersonalSurvivorController)
				PersonalSurvivorController->RequestOpenDoor(this);
		}
	}
}

void ACStructure_Door::BroadcastOpenDoor_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(OpenTimerHandle, this, &ACStructure_Door::PerformOpenDoor, 0.025f, true);

	if (OpenSound && DoorSoundAttenuation)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			OpenSound,
			this->GetActorLocation(),
			this->GetActorRotation(),
			0.5f,
			1.0f,
			0.0f,
			DoorSoundAttenuation
		);
	}
}

void ACStructure_Door::BroadcastCloseDoor_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(CloseTimerHandle, this, &ACStructure_Door::PerformCloseDoor, 0.025f, true);

	if (CloseSound && DoorSoundAttenuation)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			CloseSound,
			this->GetActorLocation(),
			this->GetActorRotation(),
			0.5f,
			1.0f,
			0.0f,
			DoorSoundAttenuation
		);
	}
}

void ACStructure_Door::PerformOpenDoor()
{
	if (!bIsOpened)
	{
		PivotArrow->AddLocalRotation(FQuat(0.0f, 0.0f, +0.05f, 1.0f));
		Angle += 0.025f;

		if (FMath::IsNearlyEqual(Angle, 0.4f, 0.01f))
		{
			//CDebug::Print("PerformOpenDoor Ended");
			GetWorld()->GetTimerManager().ClearTimer(OpenTimerHandle);
			Angle = 0.4f;
			bIsOpened = true;
			ItemReference->BuildData.bIsDoorOpened = true;
			UpdateInteractableData();
			if (PersonalSurvivorController)
			{
				ACSurvivor* survivor = Cast<ACSurvivor>(PersonalSurvivorController->GetPawn());
				if (survivor)
					survivor->GetInteractionComponent()->UpdateInteractionWidget();
			}
		}
	}
}

void ACStructure_Door::PerformCloseDoor()
{
	if (bIsOpened)
	{
		PivotArrow->AddLocalRotation(FQuat(0.0f, 0.0f, -0.05f, 1.0f));
		Angle -= 0.025f;

		if (FMath::IsNearlyEqual(Angle, 0.0f, 0.01f))
		{
			//CDebug::Print("PerformCloseDoor Ended");
			GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
			Angle = 0.0f;
			bIsOpened = false;
			ItemReference->BuildData.bIsDoorOpened = false;
			UpdateInteractableData();
			if (PersonalSurvivorController)
			{
				ACSurvivor* survivor = Cast<ACSurvivor>(PersonalSurvivorController->GetPawn());
				if (survivor)
					survivor->GetInteractionComponent()->UpdateInteractionWidget();
			}
		}

	}
}
