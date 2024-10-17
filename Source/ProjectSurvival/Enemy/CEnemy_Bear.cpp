// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemy_Bear.h"
#include "Enemy/CEnemyAIController.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
ACEnemy_Bear::ACEnemy_Bear() : ACEnemy()
{

	//CEnemy 에서 지연 메쉬 호출을 위해 필요한 경로 설정 
	SkeletalMeshPath = "SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Animal/Bear/SK_Bear.SK_Bear'";
	AnimInstancePath = "AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/Animal/Bear/ABP_Bear.ABP_Bear_C'";
	FriendlyMeshPath = "SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Animal/Bear/SK_BearMagic.SK_BearMagic'";
	//Drop
	 DropItemNum= 3;
	 DropItemID = FName("Consumable_1");
	 DropOffsetRange = 50.0f;


}

float ACEnemy_Bear::DoAction()
{
	if (HasAuthority() && GetGenericTeamId() != 1) // teamid 가 아군이 아닐때만 reduce
	{
		StatusComponent->ReduceHunger(20.0f);
	}
	return Super::DoAction();
	//Attack 몽타주 실행부분 


}

void ACEnemy_Bear::Begin_DoAction()
{
	Super::Begin_DoAction();
	

}

void ACEnemy_Bear::End_DoAction()
{
	Super::End_DoAction();

	
}

void ACEnemy_Bear::ApplyHitData()
{
	if (HasAuthority()&& GetGenericTeamId() != 1) // teamid 가 아군이 아닐때만 reduce
	{
		StatusComponent->ReduceHunger(10.0f);
	}
	Super::ApplyHitData();
}

void ACEnemy_Bear::BeginPlay()
{
	Super::BeginPlay();



}
