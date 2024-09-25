


#include "Weapon/CDoAction_Axe.h"
#include "Character/CSurvivor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CHarvestComponent.h"
#include "ActorComponents/CStateComponent.h"


void UCDoAction_Axe::DoAction()
{
	Super::DoAction();
	if(StateComponent->IsActionMode()) return;

	if (DoActionDatas.Num() > 0)
		DoActionDatas[ActionIdx].DoAction(OwnerCharacter);
	
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
	if(survivor!= nullptr&& OwnerCharacter->HasAuthority())
		survivor->GetHarvestComponent()->HarvestBoxTrace(HarvestDamage);
	
}




