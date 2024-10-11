// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemy_Boar.h"
#include "Enemy/CEnemyAIController.h"
#include "ActorComponents/CEnemyAIComponent.h"


ACEnemy_Boar::ACEnemy_Boar() : ACEnemy()
{

	//CEnemy 에서 지연 메쉬 호출을 위해 필요한 경로 설정 
	SkeletalMeshPath = "SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Animal/Boar/SK_Boar_Fangs001.SK_Boar_Fangs001'";
	AnimInstancePath = "AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/Animal/Boar/ABP_Boar.ABP_Boar_C'";
	//Drop
	DropItemNum = 3;
	DropItemID = FName("Consumable_1");
	DropOffsetRange = 50.0f;


}

float ACEnemy_Boar::DoAction()
{
	return Super::DoAction();
	//Attack 몽타주 실행부분 


}

void ACEnemy_Boar::Begin_DoAction()
{
	Super::Begin_DoAction();


}

void ACEnemy_Boar::End_DoAction()
{
	Super::End_DoAction();

	//Attack 몽타주 중단부분
}

void ACEnemy_Boar::BeginPlay()
{
	Super::BeginPlay();

	
}
