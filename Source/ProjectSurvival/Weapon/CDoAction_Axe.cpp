


#include "Weapon/CDoAction_Axe.h"
#include "Character/CSurvivor.h"
#include "Enemy/CEnemy.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CHarvestComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "DrawDebugHelpers.h"


void UCDoAction_Axe::DoAction()
{
	Super::DoAction();
	

}






void UCDoAction_Axe::Begin_DoAction()
{
	Super::Begin_DoAction();
	
}

void UCDoAction_Axe::End_DoAction()
{
	Super::End_DoAction();

}



void UCDoAction_Axe::Slash()
{
	if (!OwnerCharacter) return;
	ACSurvivor* survivor = Cast<ACSurvivor>(OwnerCharacter);
	
	if (survivor != nullptr && OwnerCharacter->HasAuthority()) //서버에서만 실행
	{

		//Trace 관련 세팅
		FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
		FVector Start = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z + 45.0f) + ForwardVector.GetSafeNormal() * TraceOffset;
		FVector End = Start + ForwardVector.GetSafeNormal() * TraceDistance;
		FQuat Rot = FQuat(OwnerCharacter->GetActorRotation());
		TArray<FHitResult> HitResults;
		FVector HalfSize = FVector(TraceDistance / 2.0f, TraceDistance / 2.0f, TraceDistance / 2.0f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerCharacter);

		FCollisionObjectQueryParams ObjectQueryParams; //다중 검출 PARAM
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
		World = OwnerCharacter->GetWorld();

		//BoxTrace 
		bool bHit = World->SweepMultiByObjectType(
			HitResults,
			Start,
			End,
			Rot,
			ObjectQueryParams,
			FCollisionShape::MakeBox(HalfSize),
			CollisionParams
		);

		DrawDebugBox(World, Start, HalfSize, Rot, FColor::Red, false, 1.0f);
		DrawDebugBox(World, End, HalfSize, Rot, FColor::Red, false, 1.0f);



		if (bHit)
		{
			for (const FHitResult& Hit : HitResults)
			{
				ACEnemy* HitEnemy;
				UPrimitiveComponent* HitComponent = Hit.GetComponent();
				if (HitComponent && HitComponent->IsA<USkeletalMeshComponent>()) //스켈레탈 메시가 검출되었는지
				{
					continue; // Skeletal Mesh는 무시
				}
				else if ((Hit.GetActor() != nullptr) && (HitEnemy = Cast<ACEnemy>(Hit.GetActor()))) //Enemy 로 형변환 가능하면 적에게 데미지
				{
					FActionDamageEvent e;
					e.HitID = DoActionDatas[ActionIdx].ActionID;
					HitEnemy->TakeDamage(0, e, OwnerCharacter->GetController(), OwnerCharacter);

				} 
				else //Static Mesh or Destructible Mesh면 
				{
					survivor->GetHarvestComponent()->HarvestBoxTrace(Hit, HarvestDamage);
				}
				
			}
		}
	}
		
	
}




