#include "Build/CStructure_Placeable.h"
#include "Build/CStructure_Foundation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/CDebug.h"

ACStructure_Placeable::ACStructure_Placeable()
{
	DownBox = CreateDefaultSubobject<UBoxComponent>("DownBox");
	DownBox->SetupAttachment(PickupMesh);
}
void ACStructure_Placeable::CheckCenter()
{

}

void ACStructure_Placeable::CheckDown_FoundationActor()
{
	// 토대 액터 체크
	FHitResult foundationActorHitResult;
	FVector startLocation = DownBox->GetComponentLocation();
	FVector endLocation = startLocation - FVector(0, 0, 300);
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypeQuery;
	objectTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<AActor*> ignores;
	FCollisionQueryParams collisionParams;
	bDown_FoundationActorHit = UKismetSystemLibrary::LineTraceSingleForObjects
	(
		GetWorld(),
		startLocation,
		endLocation,
		objectTypeQuery,
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		foundationActorHitResult,
		true,
		FLinearColor::Yellow,
		FLinearColor::Red
	);

	if (bDown_FoundationActorHit) // 라인트레이스가 어떤 액터에 히트했는지 확인
	{
		AActor* HitActor = foundationActorHitResult.GetActor();
		if (HitActor && HitActor->IsA(ACStructure_Foundation::StaticClass())) // 히트된 액터가 AСStructure_Foundation의 하위 클래스인지 확인
		{
			// AСStructure_Foundation 하위 클래스 액터에 히트했을 때 실행할 로직
			CDebug::Print("Foundation Found", FColor::Cyan);
		}
	}
}

void ACStructure_Placeable::CheckDown_CeilingActor()
{
}
