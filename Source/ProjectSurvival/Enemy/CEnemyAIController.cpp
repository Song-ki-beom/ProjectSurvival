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
        Sight->PeripheralVisionAngleDegrees = 90; // 인식하는 시야 범위 
        Sight->SetMaxAge(15);  //인식한 상대를 기억하는 시간 
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
   

    if (!bIsAggroCoolDown) return;

    Perception->GetCurrentlyPerceivedActors(nullptr, CandidateActors); //이미 Sight 컨피그에 의해 플레이어를 적으로 인식하도록 함 



    //거리상 제일 가까운 Target 탐색 
    if (CandidateActors.Num() > 0)
    {
        float shortestDistance = 99999.0f;
        for (AActor* CandidateActor : CandidateActors)
        {
            if (GetPawn() == nullptr) return;
            float distanceTo  = GetPawn()->GetDistanceTo(CandidateActor);
            if (distanceTo < shortestDistance)
            {
                shortestDistance = distanceTo;
                TargetActor = CandidateActor;
            }
        }
        if (TargetActor != nullptr)
        {
            bIsAggroCoolDown = false;
            GetWorld()->GetTimerManager().SetTimer(AggroTimerHandle, this, &ACEnemyAIController::EnableAggroCoolDown, CooldownTime, false);
            //CDebug::Log(actors[0]->GetName()); 
            Blackboard->SetValueAsObject("Target", TargetActor); //처음 인식된 Actor을 타겟으로 설정 
        }
        CandidateActors.Empty();
    }
    return;

}

void ACEnemyAIController::EnableAggroCoolDown()
{
    bIsAggroCoolDown = true; //어그로 풀림 설정 .. 다시 탐색 시작 

    
}
