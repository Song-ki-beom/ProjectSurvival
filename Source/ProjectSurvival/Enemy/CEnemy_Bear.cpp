// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemy_Bear.h"

ACEnemy_Bear::ACEnemy_Bear()
{
	//CEnemy 에서 지연 메쉬 호출을 위해 필요한 경로 설정 
	SkeletalMeshPath = "SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Animal/Bear/SK_Bear.SK_Bear'";
	AnimInstancePath = "AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/Animal/Bear/ABP_Bear.ABP_Bear_C'";
	BBAssetPath = "BlackboardData'/Game/PirateIsland/Include/Blueprints/Character/Animal/Bear/BB_Bear.BB_Bear'";
	//Drop
	 DropItemNum= 3;
	 DropItemID = FName("Consumable_1");
	 DropOffsetRange = 50.0f;
}

void ACEnemy_Bear::DoAction()
{
	Super::DoAction();
	//Attack 몽타주 실행부분 


}

void ACEnemy_Bear::Begin_DoAction()
{
	Super::Begin_DoAction();

}

void ACEnemy_Bear::End_DoAction()
{
	Super::End_DoAction();

	//Attack 몽타주 중단부분
}

void ACEnemy_Bear::BeginPlay()
{
	Super::BeginPlay();

}
