// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemyAIController.h"


#include "Enemy/CEnemyAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/CEnemy.h"
#include "Utility/CDebug.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACEnemyAIController::ACEnemyAIController()
{


    Blackboard =  CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

    {
        Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight"); //Sight , Hearing 중에 Sight 감지 모드로 설정 
        Sight->SightRadius = 1000;      
        Sight->LoseSightRadius = 1200;  
        Sight->PeripheralVisionAngleDegrees = 60; // 인식하는 시야 범위 
        Sight->SetMaxAge(5);  //인식한 상대를 기억하는 시간 
    }

   
    {
        Sight->DetectionByAffiliation.bDetectEnemies = false;        
        Sight->DetectionByAffiliation.bDetectNeutrals = true;   
        Sight->DetectionByAffiliation.bDetectFriendlies = false;    
    }
   
    Perception->ConfigureSense(*Sight);   
    Perception->SetDominantSense(*Sight->GetSenseImplementation());

}

void ACEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    Perception->OnPerceptionUpdated.AddDynamic(this, &ACEnemyAIController::OnPerceptionUpdated);
}

void ACEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    Enemy = Cast<ACEnemy>(InPawn);
    SetGenericTeamId(FGenericTeamId(Enemy->GetTeamID()));

  
    if(Enemy->GetBehaviorTree() == nullptr) 
        return;
    UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset,Blackboard); //CEnemy의 BehaviorTree 에서 설정한 BlackBoardAsset 를 가져와서 AIController의 참조 블랙보드로 설정 
    RunBehaviorTree(Enemy->GetBehaviorTree()); //BT 실행

    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    if(AIComponent == nullptr) return;
    AIComponent->SetBlackboard(Blackboard); //AIComponent에 여기서 설정된 BlackBoard를 AIComponent 가 사용하는 BlackBoard 로 설정(추후에 블랙보드 참조하기 위함)

}

void ACEnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ACEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    TArray<AActor*> actors;
    Perception->GetCurrentlyPerceivedActors(nullptr, actors); //이미 Sight 컨피그에 의해 플레이어를 적으로 인식하도록 함 

    if (actors.Num() > 0)
    {
        CDebug::Log(actors[0]->GetName()); 
        Blackboard->SetValueAsObject("Target", actors[0]); //처음 인식된 Actor을 타겟으로 설정 
        return;
    }
    Blackboard->SetValueAsObject("Target", nullptr);
}
