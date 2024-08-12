


#include "Weapon/CDoAction_Axe.h"
#include "Character/CSurvivor.h"
#include "GameFramework/Character.h"
#include "ActorComponents/CHarvestComponent.h"


void UCDoAction_Axe::DoAction()
{
	Super::DoAction();
	if (DoActionDatas.Num() > 0)
		DoActionDatas[0].DoAction(OwnerCharacter);
}


void UCDoAction_Axe::Begin_DoAction()
{
	Super::Begin_DoAction();
	Slash();
}

void UCDoAction_Axe::Slash()
{
	if (!OwnerCharacter) return;
	ACSurvivor* survivor = Cast<ACSurvivor>(OwnerCharacter);
	if(survivor!= nullptr) 
		survivor->GetHarvestComponent()->HarvestBoxTrace(HarvestDamage);
	
}


