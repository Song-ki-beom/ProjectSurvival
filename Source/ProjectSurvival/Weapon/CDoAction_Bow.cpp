// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CDoAction_Bow.h"
#include "Weapon/CEquipment.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ActorComponents/CAmmoComponent.h"
#include "Weapon/CAttachment_Bow.h"
#include "Weapon/CArrow.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/CDebug.h"
#include "Net/UnrealNetwork.h"
#include "Animation/CBowAnimInstance.h"

void  UCDoAction_Bow::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionData)
{
    Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionData);

    StateComponent = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
    SkeletalMesh = Cast<USkeletalMeshComponent>(InOwner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    //PoseableMesh = Cast<UPoseableMeshComponent>(InOwner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    AmmoComponent = Cast<UCAmmoComponent>(InOwner->GetComponentByClass(UCAmmoComponent::StaticClass()));
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
    //if (!AmmoComponent->GetIsArrowFullyCreated()) return;
    Super::DoAction();
    //DoActionDatas[0].DoAction(OwnerCharacter);

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
    if (AmmoComponent == nullptr) return;
    if(bBeginAction) return ;

    bAttachedString = false;
    *Bend = 0;

    Super::Begin_DoAction();


    int32 ArrowIdx = AmmoComponent->GetAttachedArrow();

    APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (PlayerController)
    {
         //화면 중앙에 있는 크로스헤어의 좌표 계산
        //int32 ViewportSizeX, ViewportSizeY;
        //PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
        //FVector2D CrosshairPosition(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f); // 화면의 중앙 지점 좌표

        FVector2D CrosshairPosition;
        if (OwnerCharacter->GetWorld()->GetGameViewport())
            OwnerCharacter->GetWorld()->GetGameViewport()->GetViewportSize(CrosshairPosition); //위에꺼 안되면 이걸로
        CrosshairPosition = FVector2D(CrosshairPosition.X * 0.5f, CrosshairPosition.Y * 0.5f);
        
        // 크로스헤어 좌표에서 월드 방향으로 라인 트레이스
        FVector CameraLocation;
        FRotator CameraRotation;
        PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
        

        FVector WorldDirection;
        if (PlayerController->DeprojectScreenPositionToWorld(CrosshairPosition.X, CrosshairPosition.Y, CameraLocation, WorldDirection))
        {
            // 라인 트레이스를 이용하여 크로스헤어가 가리키는 월드 지점 찾기
            FVector Start = CameraLocation;
            FVector End = Start + (WorldDirection * 200000.0f); 
            
        
            AmmoComponent->ShootArrow(Start, End);

           
        }
    }
}







void UCDoAction_Bow::End_DoAction()
{
    Super::End_DoAction();
    if(OwnerCharacter->HasAuthority())
        AmmoComponent->CreateArrow();
}

void UCDoAction_Bow::OnBeginEquip()
{
    Super::OnBeginEquip();
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (OwnerCharacter->HasAuthority())
        AmmoComponent->CreateArrow();
 

}

void UCDoAction_Bow::OnUnEquip()
{

    Super::OnUnEquip();
    OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    *Bend = 0.0f;
    //PoseableMesh->SetBoneLocationByName("Bow_String", OriginLocation, EBoneSpaces::ComponentSpace);
    AmmoComponent->DestroyArrows();
}


void UCDoAction_Bow::End_BowString()
{
    if(StateComponent->IsSubActionMode())
        *Bend = 100;
    bAttachedString = true;
}

void UCDoAction_Bow::DestroyWeapon()
{
    AmmoComponent->DestroyArrows();

}
