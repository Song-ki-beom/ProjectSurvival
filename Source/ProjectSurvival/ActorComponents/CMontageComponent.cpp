#include "ActorComponents/CMontageComponent.h"
#include "GameFramework/Character.h"
#include "Utility/CDebug.h"
#include "Components/SkeletalMeshComponent.h"

UCMontageComponent::UCMontageComponent()
{

}
void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	

	// 몽타주 제거할때 
	//Mesh->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMCombatComponent::OnMontageNotifyBegin);

	if (OwnerCharacter && OwnerCharacter->GetMesh() && OwnerCharacter->GetMesh()->GetAnimInstance()) 
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UCMontageComponent::OnMontageNotifyBegin);
		OwnerCharacter->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.AddDynamic(this, &UCMontageComponent::OnMontageNotifyEnd);
		OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCMontageComponent::OnMontageEnded);

	}

}


void UCMontageComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	OnPlayMontageNotifyBegin.Broadcast();
}
void UCMontageComponent::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	OnPlayMontageNotifyEnd.Broadcast();
}

void UCMontageComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) //중간에 중단 
	{
		
		CDebug::Print("OnMontageInterrupted: ", Montage->GetFName().ToString());
			OnMontageInterrupted.Broadcast();
		
	}
	else
	{// 성공적 마무리 
		
		CDebug::Print("OnMontageFinalEnded", Montage->GetFName().ToString());
			OnMontageFinalEnded.Broadcast();
	
		
	}
}

void UCMontageComponent::BindFoodMontageEnded()
{
	OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &UCMontageComponent::OnMontageEnded);

	OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCMontageComponent::OnFoodMontageEnded);

}

void UCMontageComponent::OnFoodMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	OnFoodMontageNotifyEnd.Broadcast();

	OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &UCMontageComponent::OnFoodMontageEnded);
	//원상태의 델리게이트로 복귀 
	OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCMontageComponent::OnMontageEnded);

}



// session
void UCMontageComponent::Montage_Play(UAnimMontage* InMontage, float InPlayRate)
{
	MontageDelay= OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(InMontage, InPlayRate);
	CDebug::Print("Montage Delay:", MontageDelay);
}

void UCMontageComponent::Montage_Play_Section(UAnimMontage* InMontage, FName SectionName)
{
	 OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, InMontage);
}


