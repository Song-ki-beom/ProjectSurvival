// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CDoAction_Bow.h"
#include "Weapon/CEquipment.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/CAttachment_Bow.h"
#include "Weapon/CArrow.h"
#include "Animation/CBowAnimInstance.h"

void  UCDoAction_Bow::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionData)
{
    Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionData);

    StateComponent = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
    SkeletalMesh = Cast<USkeletalMeshComponent>(InOwner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    //PoseableMesh = Cast<UPoseableMeshComponent>(InOwner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
   
    AnimInstance_Bow = Cast<UCBowAnimInstance>(SkeletalMesh->GetAnimInstance());
    
    ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

    if (!!bow)
        Bend = bow->GetBend();

    //OriginLocation = PoseableMesh->GetBoneLocationByName("Bow_String", EBoneSpaces::ComponentSpace);
    bEquipped = InEquipment->GetEquipped();

}


void UCDoAction_Bow::DoAction()
{
    if(!StateComponent->IsIdleMode()) return;
    if(!StateComponent->IsSubActionMode()) return;

    Super::DoAction();
    DoActionDatas[0].DoAction(OwnerCharacter);

}
void UCDoAction_Bow::Tick(float InDeltaTime)
{
    Super::Tick(InDeltaTime);

    bool bCheck = true;
    bCheck &= (*bEquipped == true);   
    bCheck &= (bBeginAction == true);  
    bCheck &= (bAttachedString == true);  

    if(!bCheck) return;

    //PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
    FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("socket_weapon_r");
    //PoseableMesh->SetBoneLocationByName("Bow_String", handLocation, EBoneSpaces::WorldSpace);


}

void UCDoAction_Bow::Begin_DoAction()
{
    if(OwnerCharacter == nullptr) return;
    if(bBeginAction) return ;

    bAttachedString = false;
    *Bend = 0;

    //PoseableMesh->SetBoneLocationByName("Bow_String", OriginLocation, EBoneSpaces::ComponentSpace);


    Super::Begin_DoAction();


    int32 index = GetAttachedArrow();
    if (index == -1) return;

    ACArrow* arrow = Arrows[index].Arrow;
    if(arrow == nullptr) return;
    arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
    arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
    arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay); // LifeSpan

    FVector foward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
    arrow->Shoot(foward);
    Arrows[index].bShooting = true;
}

void UCDoAction_Bow::End_DoAction()
{
    Super::End_DoAction();
    CreateArrow();
}

void UCDoAction_Bow::OnBeginEquip()
{
    Super::OnBeginEquip();
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CreateArrow();
}

void UCDoAction_Bow::OnUnEquip()
{

    Super::OnUnEquip();
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    *Bend = 0.0f;
    //PoseableMesh->SetBoneLocationByName("Bow_String", OriginLocation, EBoneSpaces::ComponentSpace);

    for (int32 i = 0; i < Arrows.Num(); i++)
    {
        if (Arrows[i].bShooting) continue;

        Arrows[i].Arrow->Destroy();
        Arrows.RemoveAt(i);
    }


  


}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
    /*CheckFalse(HitDatas.Num() > 0);
    HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);*/
}


void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
   
    for (int32 index = Arrows.Num() - 1; index >= 0; index--)
    {
        if (Arrows[index].Arrow == InDestroyer)
            Arrows.RemoveAt(index);
    }
}

void UCDoAction_Bow::ShowAttachedArrow()
{
    int32 curArrowIdx = GetAttachedArrow();
    if (curArrowIdx<0 || curArrowIdx>Arrows.Num()) return;
    if (Arrows[curArrowIdx].bShooting) return;
    Arrows[curArrowIdx].Arrow->SetActorHiddenInGame(false);
}

void UCDoAction_Bow::HideAttachedArrow()
{
    int32 curArrowIdx = GetAttachedArrow();
    if (curArrowIdx<0 || curArrowIdx>Arrows.Num()) return;
    if (Arrows[curArrowIdx].bShooting) return;
    Arrows[curArrowIdx].Arrow->SetActorHiddenInGame(true);
    //*Bend = 0;
}

void UCDoAction_Bow::CreateArrow()
{
    if (World->bIsTearingDown == true)
        return;
    FArrow tempArrow;
    FTransform transform;
    tempArrow.Arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform,
        nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if(tempArrow.Arrow == nullptr) return;

    {
        tempArrow.Arrow->AddIgnoreActor(OwnerCharacter);
        FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
        tempArrow.Arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");
        if (!StateComponent->IsSubActionMode())
        tempArrow.Arrow->SetActorHiddenInGame(true);
    }

    UGameplayStatics::FinishSpawningActor(tempArrow.Arrow, transform);
    Arrows.Add(tempArrow);

}

int32 UCDoAction_Bow::GetAttachedArrow()
{
    for (int32 index = Arrows.Num() - 1; index >= 0; index--)
    {
        if (Arrows[index].bShooting) continue;
        return index;
    }
    return -1;

}

void UCDoAction_Bow::End_BowString()
{
    if(StateComponent->IsSubActionMode())
        *Bend = 100;
    bAttachedString = true;
}

void UCDoAction_Bow::DestroyWeapon()
{
    for (int32 i = 0; i < Arrows.Num(); i++)
    {
        if (Arrows[i].bShooting) continue;

        Arrows[i].Arrow->Destroy();
        Arrows.RemoveAt(i);
    }

}
