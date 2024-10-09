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
#include "ActorComponents/CMovingComponent.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInventoryItemSlot.h"
#include "Widget/Map/CWorldMap.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"

void UCDoAction_Weapon::DoAction()
{
	bool bIsCanDoAction = true;

	UWorld* world = OwnerCharacter->GetWorld();

	if (!world)
	{
		CDebug::Print("world is not valid", FColor::Magenta);
	}

	UCGameInstance* gameInstance = Cast<UCGameInstance>(world->GetGameInstance());
	if (gameInstance)
	{
		ACSurvivor* personalSurvivor = gameInstance->WorldMap->GetPersonalSurvivor();

		if (personalSurvivor)
		{
			UCItemBase* usingItem = personalSurvivor->GetWeaponComponent()->GetUsingWeapon();
			UCInventoryItemSlot* usingItemSlot = personalSurvivor->GetWeaponComponent()->GetUsingWeaponSlot();
			if (usingItem && usingItemSlot)
			{
				if (usingItem->ItemStats.RemainDurability == 0)
				{
					bIsCanDoAction = false;
				}
			}
		}
	}

	if (!bIsCanDoAction)
		return;

	Super::DoAction();
}

void UCDoAction_Weapon::Begin_DoAction()
{
	bool bIsCanDoAction = true;

	UWorld* world = OwnerCharacter->GetWorld();

	if (!world)
	{
		CDebug::Print("world is not valid", FColor::Magenta);
	}

	UCGameInstance* gameInstance = Cast<UCGameInstance>(world->GetGameInstance());
	if (gameInstance)
	{
		ACSurvivor* personalSurvivor = gameInstance->WorldMap->GetPersonalSurvivor();

		if (personalSurvivor)
		{
			UCItemBase* usingItem = personalSurvivor->GetWeaponComponent()->GetUsingWeapon();
			UCInventoryItemSlot* usingItemSlot = personalSurvivor->GetWeaponComponent()->GetUsingWeaponSlot();
			if (usingItem && usingItemSlot)
			{
				if (usingItem->ItemStats.RemainDurability == 0)
				{
					bIsCanDoAction = false;
				}
			}
		}
	}

	if (!bIsCanDoAction)
		return;

	Super::Begin_DoAction();
}

void UCDoAction_Weapon::End_DoAction()
{
	Super::End_DoAction();

	UWorld* world = OwnerCharacter->GetWorld();

	if (!world)
	{
		CDebug::Print("world is not valid", FColor::Magenta);
	}

	UCGameInstance* gameInstance = Cast<UCGameInstance>(world->GetGameInstance());
	if (gameInstance)
	{
		ACSurvivor* personalSurvivor = gameInstance->WorldMap->GetPersonalSurvivor();

		if (personalSurvivor)
		{
			UCItemBase* usingItem = personalSurvivor->GetWeaponComponent()->GetUsingWeapon();
			UCInventoryItemSlot* usingItemSlot = personalSurvivor->GetWeaponComponent()->GetUsingWeaponSlot();
			if (usingItem && usingItemSlot)
			{
				if (usingItem->ItemStats.RemainDurability == 0)
				{
					personalSurvivor->GetWeaponComponent()->SetMode(usingItem->HuntData.WeaponType);
				}
			}
		}
	}
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
			FQuat rot = FQuat(HitBox->GetComponentRotation());
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
				bool bIsValidHit = false;

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
						bIsValidHit = true;
					}
					else //Static Mesh or Destructible Mesh면 
					{
						survivor->GetHarvestComponent()->HarvestBoxTrace(hit, HarvestDamage);
						bIsValidHit = true;
					}
				}

				if (bIsValidHit)
				{
					if (!OwnerCharacter) return;

					UWorld* world = OwnerCharacter->GetWorld();

					if (!world)
					{
						CDebug::Print("world is not valid", FColor::Magenta);
					}

					UCGameInstance* gameInstance = Cast<UCGameInstance>(world->GetGameInstance());
					if (gameInstance)
					{
						ACSurvivor* personalSurvivor = gameInstance->WorldMap->GetPersonalSurvivor();

						if (personalSurvivor)
						{
							UCItemBase* usingItem = personalSurvivor->GetWeaponComponent()->GetUsingWeapon();
							UCInventoryItemSlot* usingItemSlot = personalSurvivor->GetWeaponComponent()->GetUsingWeaponSlot();
							if (usingItem && usingItemSlot)
							{
								usingItem->ItemStats.RemainDurability--;
								usingItemSlot->SetRemainDurability(usingItem->ItemStats.RemainDurability);
								if (usingItem->ItemStats.RemainDurability == 0)
								{
									usingItemSlot->SetRedXVisibility(ESlateVisibility::Visible);
									//personalSurvivor->GetWeaponComponent()->SetMode(usingItem->HuntData.WeaponType);
									//End_DoAction();
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (survivor->IsLocallyControlled())
		{

			HitBox = survivor->GetWeaponComponent()->GetAttachment()->GetHitBox();

			//Trace 관련 세팅
			if (HitBox)
			{
				FVector start = HitBox->GetComponentLocation();
				FVector end = HitBox->GetComponentLocation();
				FQuat rot = FQuat(HitBox->GetComponentRotation());
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

				DrawDebugBox(World, start, halfSize, rot, FColor::Green, false, 1.0f);

				if (bHit)
				{
					bool bIsValidHit = false;

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
							bIsValidHit = true;
						}
						else //Static Mesh or Destructible Mesh면 
						{
							bIsValidHit = true;
						}
					}

					if (bIsValidHit)
					{
						if (!OwnerCharacter) return;

						UWorld* world = OwnerCharacter->GetWorld();

						if (!world)
						{
							CDebug::Print("world is not valid", FColor::Magenta);
						}

						UCGameInstance* gameInstance = Cast<UCGameInstance>(world->GetGameInstance());
						if (gameInstance)
						{
							ACSurvivor* personalSurvivor = gameInstance->WorldMap->GetPersonalSurvivor();

							if (personalSurvivor)
							{
								UCItemBase* usingItem = personalSurvivor->GetWeaponComponent()->GetUsingWeapon();
								UCInventoryItemSlot* usingItemSlot = personalSurvivor->GetWeaponComponent()->GetUsingWeaponSlot();
								if (usingItem && usingItemSlot)
								{
									int32 durability = usingItem->ItemStats.RemainDurability--;
									usingItemSlot->SetRemainDurability(durability);
									if (durability == 0)
									{
										usingItem->ItemStats.RemainDurability--;
										usingItemSlot->SetRemainDurability(usingItem->ItemStats.RemainDurability);
										if (usingItem->ItemStats.RemainDurability == 0)
										{
											usingItemSlot->SetRedXVisibility(ESlateVisibility::Visible);
											//personalSurvivor->GetWeaponComponent()->SetMode(usingItem->HuntData.WeaponType);
											//End_DoAction();
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
