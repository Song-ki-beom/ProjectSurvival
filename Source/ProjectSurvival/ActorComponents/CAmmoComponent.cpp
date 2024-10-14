// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/CAmmoComponent.h"
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
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/CDebug.h"
#include "Net/UnrealNetwork.h"
#include "Animation/CBowAnimInstance.h"


UCAmmoComponent::UCAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    bArrowFullyCreated = false;
    
}


void UCAmmoComponent::BeginPlay()
{
	Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
        StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}



void UCAmmoComponent::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter)
{
    if (OwnerCharacter->HasAuthority())
    {
        FActionDamageEvent e;
        e.HitID = FName("Survivor_Bow");
        InOtherCharacter->TakeDamage(0, e, OwnerCharacter->GetController(), OwnerCharacter);

    }
}


void UCAmmoComponent::OnArrowEndPlay(ACArrow* InDestroyer)
{
    if (!OwnerCharacter->HasAuthority()) return;
    for (int32 index = Arrows.Num() - 1; index >= 0; index--)
    {
        if (Arrows[index].Arrow == InDestroyer)
            BroadcastRemoveArrow(index);

    }
}

void UCAmmoComponent::ShowAttachedArrow()
{
    if (OwnerCharacter->HasAuthority())
    {
        BroadcastShowAttachedArrow();
    }
    else
    {
        RequestShowAttachedArrow();
    }
   
}

void UCAmmoComponent::BroadcastShowAttachedArrow_Implementation()
{
    int32 curArrowIdx = GetAttachedArrow();
    if (curArrowIdx<0 || curArrowIdx>Arrows.Num()) return;
    if (Arrows[curArrowIdx].bShooting) return;
    Arrows[curArrowIdx].Arrow->SetActorHiddenInGame(false);
}

void UCAmmoComponent::RequestShowAttachedArrow_Implementation()
{
    BroadcastShowAttachedArrow();
}



void UCAmmoComponent::HideAttachedArrow()
{
    if (OwnerCharacter->HasAuthority())
    {
        BroadcastHideAttachedArrow();
    }
    else
    {
        RequestHideAttachedArrow();
    }
}


void UCAmmoComponent::BroadcastHideAttachedArrow_Implementation()
{
    int32 curArrowIdx = GetAttachedArrow();
    if (curArrowIdx<0 || curArrowIdx>Arrows.Num()) return;
    if (Arrows[curArrowIdx].bShooting) return;
    Arrows[curArrowIdx].Arrow->SetActorHiddenInGame(true);
}

void UCAmmoComponent::RequestHideAttachedArrow_Implementation()
{
    BroadcastHideAttachedArrow();
}


void UCAmmoComponent::CreateArrow()
{
    if (!OwnerCharacter->HasAuthority()) return;
    
    if (GetWorld()->bIsTearingDown == true)
        return;
    bArrowFullyCreated = false;
    //새로 만들 화살 초기화
    TempArrow.Arrow = NULL;
    TempArrow.bShooting = false;
    FTransform transform;
    TempArrow.Arrow = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACArrow>(ArrowClass, transform,
        nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (TempArrow.Arrow == nullptr) return;
    
    UGameplayStatics::FinishSpawningActor(TempArrow.Arrow, transform);
    BroadcastInitializeArrow(TempArrow.Arrow);
    Arrows.Add(TempArrow);

}

void UCAmmoComponent::ShootArrow(FVector Start,FVector End)
{
   
    

    if (OwnerCharacter->HasAuthority())
    {
        PerformShootArrow(Start, End);
    }
    else
    {
        RequestShootArrow(Start, End);

    }

  
}

void UCAmmoComponent::PerformShootArrow(FVector Start, FVector End)
{
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter); // 자신의 캐릭터는 트레이스에서 제외

    DrawDebugLine(OwnerCharacter->GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
    // 라인 트레이스 실행
    if (OwnerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        CDebug::Print("Arrow Hit Actor:", HitResult.Component->GetName());
        // 라인 트레이스가 맞은 지점을 목표로 설정
        FVector TargetLocation = HitResult.ImpactPoint;
        int32 AttachedIdx = GetAttachedArrow();
        if (AttachedIdx<0 || AttachedIdx >= Arrows.Num()) return;
        Arrows[AttachedIdx].bShooting = true;
        ACArrow* arrow = Arrows[AttachedIdx].Arrow;
        if (arrow == nullptr) return;
        FVector FireDirection = (TargetLocation - arrow->GetActorLocation()).GetSafeNormal();

        // 화살의 방향을 회전하여 맞추기
        //arrow->SetActorRotation(FireDirection.Rotation());


        arrow->OnHit.AddDynamic(this, &UCAmmoComponent::OnArrowHit);
        arrow->OnEndPlay.AddDynamic(this, &UCAmmoComponent::OnArrowEndPlay); // LifeSpan
        BroadcastShootArrow(arrow, FireDirection);
    }
}

void UCAmmoComponent::BroadcastShootArrow_Implementation(ACArrow* TargetArrow, FVector Direction)
{
    if (TargetArrow == nullptr) return;
    AActor* ParentActor = TargetArrow->GetAttachParentActor();
    if (!IsValid(ParentActor)) return;
    if (ParentActor && IsValid(ParentActor) && !ParentActor->IsPendingKill())
        TargetArrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
    TargetArrow->Shoot(Direction);
}
void UCAmmoComponent::RequestShootArrow_Implementation(FVector Start, FVector End)
{
    PerformShootArrow(Start, End);
}




void UCAmmoComponent::BroadcastInitializeArrow_Implementation(class ACArrow* TargetArrow)
{
    if (TargetArrow == NULL) return;
        TargetArrow->AddIgnoreActor(OwnerCharacter);
        FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
        if(OwnerCharacter->GetMesh())
            TargetArrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");
        if (StateComponent &&!StateComponent->IsSubActionMode())
            TargetArrow->SetActorHiddenInGame(true);
        bArrowFullyCreated = true;
       
}

void UCAmmoComponent::BroadcastAddArrow_Implementation(class ACArrow* TargetArrow)
{
    FArrow newArrrowData = {false, TargetArrow };
        Arrows.Add(newArrrowData);
}


int32 UCAmmoComponent::GetAttachedArrow()
{

    for (int32 index = Arrows.Num() - 1; index >= 0; index--)
    {
        if (Arrows[index].bShooting) continue;
        return index;
    }

    return -1;

}

ACArrow* UCAmmoComponent::GetArrow(int32 ArrowIndex)
{
    if (ArrowIndex < 0 || ArrowIndex >= Arrows.Num()) return NULL;
    return Arrows[ArrowIndex].Arrow;

}




void UCAmmoComponent::DestroyArrows()
{
    if (!OwnerCharacter->HasAuthority()) return;

    for (int32 i = 0; i < Arrows.Num(); i++)
    {
        if (Arrows[i].bShooting) continue;

        Arrows[i].Arrow->Destroy();
        BroadcastRemoveArrow(i);
    }

}

void UCAmmoComponent::BroadcastRemoveArrow_Implementation(int32 Arrowidx)
{
    if (Arrowidx < 0 || Arrowidx >= Arrows.Num()) return;
    Arrows.RemoveAt(Arrowidx);

}