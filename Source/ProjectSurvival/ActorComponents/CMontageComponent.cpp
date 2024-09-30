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

}
void UCMontageComponent::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}

void UCMontageComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bInterrupted;
		// 몽타주가 중단된 경우 처리할 로직
	}
	else
	{
		// 몽타주가 정상적으로 끝난 경우 처리할 로직
	}
}



// session
void UCMontageComponent::Montage_Play(UAnimMontage* InMontage, float InPlayRate)
{
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(InMontage, InPlayRate);
}

void UCMontageComponent::Montage_Play_Section(UAnimMontage* InMontage, FName SectionName)
{
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, InMontage);
}


