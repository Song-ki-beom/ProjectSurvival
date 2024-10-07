#include "Weapon/CDoAction_Weapon.h"
#include "Weapon/CAttachment.h"
#include "Components/BoxComponent.h"
#include "Character/CSurvivor.h"
#include "Enemy/CEnemy.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CHarvestComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/CDebug.h"

void UCDoAction_Weapon::DoAction()
{
	Super::DoAction();
}

void UCDoAction_Weapon::Begin_DoAction()
{
	Super::Begin_DoAction();
}

void UCDoAction_Weapon::End_DoAction()
{
	Super::End_DoAction();
}

void UCDoAction_Weapon::WeaponHitTrace()
{
	if (!OwnerCharacter) return;
	ACSurvivor* survivor = Cast<ACSurvivor>(OwnerCharacter);

	if (survivor != nullptr && OwnerCharacter->HasAuthority()) //서버에서만 실행
	{
		HitBox = survivor->GetWeaponComponent()->GetAttachment()->GetHitBox();

		//Trace 관련 세팅
		if (HitBox)
		{
			FVector start = HitBox->GetComponentLocation();
			FVector end = HitBox->GetComponentLocation();
			FQuat rot = FQuat(OwnerCharacter->GetActorRotation());
			TArray<FHitResult> hitResults;
			FVector halfSize = FVector(HitBox->GetScaledBoxExtent());
			FCollisionQueryParams collisionParams;
			collisionParams.AddIgnoredActor(OwnerCharacter);

			FCollisionObjectQueryParams objectQueryParams; //다중 검출 PARAM
			objectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
			objectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
			objectQueryParams.AddObjectTypesToQuery(ECC_Destructible);

			World = OwnerCharacter->GetWorld();

			//BoxTrace 
			bool bHit = World->SweepMultiByObjectType(
				hitResults,
				start,
				end,
				rot,
				objectQueryParams,
				FCollisionShape::MakeBox(halfSize),
				collisionParams
			);

			DrawDebugBox(World, start, halfSize, rot, FColor::Red, false, 1.0f);

			if (bHit)
			{
				for (const FHitResult& hit : hitResults)
				{
					ACEnemy* hitEnemy;
					UPrimitiveComponent* hitComponent = hit.GetComponent();
					if (hitComponent && hitComponent->IsA<USkeletalMeshComponent>()) //스켈레탈 메시가 검출되었는지
					{
						continue; // Skeletal Mesh는 무시
					}
					else if ((hit.GetActor() != nullptr) && (hitEnemy = Cast<ACEnemy>(hit.GetActor()))) //Enemy 로 형변환 가능하면 적에게 데미지
					{
						FActionDamageEvent e;
						e.HitID = DoActionDatas[ActionIdx].ActionID;
						hitEnemy->TakeDamage(0, e, OwnerCharacter->GetController(), OwnerCharacter);
					}
					else //Static Mesh or Destructible Mesh면 
					{
						survivor->GetHarvestComponent()->HarvestBoxTrace(hit, HarvestDamage);
					}

				}
			}
		}
	}
}
