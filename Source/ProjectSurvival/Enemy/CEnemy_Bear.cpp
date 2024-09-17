// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemy_Bear.h"

ACEnemy_Bear::ACEnemy_Bear()
{
	//CEnemy 에서 지연 메쉬 호출을 위해 필요한 경로 설정 
	SkeletalMeshPath = "SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Animal/Bear/SK_Bear.SK_Bear'";
	AnimInstancePath = "AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/Animal/Bear/ABP_Bear.ABP_Bear_C'";
	BBAssetPath = "BlackboardData'/Game/PirateIsland/Include/Blueprints/Character/Animal/Bear/BB_Bear.BB_Bear'";

}

void ACEnemy_Bear::BeginPlay()
{
	Super::BeginPlay();

}
