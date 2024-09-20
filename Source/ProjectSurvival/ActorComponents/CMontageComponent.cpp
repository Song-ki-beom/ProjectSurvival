#include "ActorComponents/CMontageComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UCMontageComponent::UCMontageComponent()
{

}
void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (DataTable == nullptr)
	{
		return;
	}

	TArray<FMontagesData*>  OutArray;
	DataTable->GetAllRows<FMontagesData>("", OutArray);

	
	
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontagesData* data : OutArray)
		{
			if ((EStateType)i == data->Type)
			{
				MontageDatas[i] = data;
			}
		}
	}

	// 몽타주 제거할때 
	//Mesh->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UMCombatComponent::OnMontageNotifyBegin);

	if (OwnerCharacter && OwnerCharacter->GetMesh() && OwnerCharacter->GetMesh()->GetAnimInstance()) 
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UCMontageComponent::OnMontageNotifyBegin);
		OwnerCharacter->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.AddDynamic(this, &UCMontageComponent::OnMontageNotifyEnd);
		OwnerCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCMontageComponent::OnMontageEnded);
	}

}





void UCMontageComponent::PlayDeadMontage()
{
	PlayAnimMontage(EStateType::Dead);
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

void UCMontageComponent::PlayAnimMontage(EStateType InType)
{
	if(OwnerCharacter == nullptr) return;

	FMontagesData* data = MontageDatas[(int32)InType];
	if (data == nullptr)
	{
		return;
	}
	//	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(data->Montage, data->PlayRate);
}
void UCMontageComponent::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}
void UCMontageComponent::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{

}

void UCMontageComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
}