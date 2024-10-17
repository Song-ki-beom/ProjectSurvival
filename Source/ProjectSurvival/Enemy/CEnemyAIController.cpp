// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/CEnemyAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/CEnemy.h"
#include "Utility/CDebug.h"
#include "CoreMinimal.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

ACEnemyAIController::ACEnemyAIController()
{
    bReplicates = true;
    SetReplicates(true);
    SetGenericTeamId(FGenericTeamId(3));
   
    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));


    //Sight Config
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight"); //Sight , Hearing 중에 Sight 감지 모드로 설정 
    SetPerceptionComponent(*Perception);

    SightConfig->SightRadius = 1000;
    SightConfig->LoseSightRadius = 1100;
    SightConfig->PeripheralVisionAngleDegrees = 120; // 인식하는 시야 범위 
    SightConfig->SetMaxAge(3);  //인식한 상대를 기억하는 시간 

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

    // Damage Config
    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage"));
    DamageConfig->SetMaxAge(6.0f); 

   
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
    GetPerceptionComponent()->ConfigureSense(*DamageConfig);
    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACEnemyAIController::OnPerceptionUpdated);

  
}




void ACEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ACEnemyAIController::CustomTick, 0.1f, true);

}



void ACEnemyAIController::UpdatePerception()
{
    if (GetPerceptionComponent())
    {
      
        TArray<AActor*> AllActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

        for (AActor* Actor : AllActors)
        {
            APawn* OtherPawn = Cast<APawn>(Actor);
            if (OtherPawn) 
            {
                ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
                UE_LOG(LogTemp, Warning, TEXT("Updated attitude towards %s: %d"), *Actor->GetName(), (int32)Attitude);
            }
               
        }
    }
}

void ACEnemyAIController::TameEnemyToPlayer(APlayerController* PlayerController)
{
    if (PlayerController == nullptr) return;

        // 플레이어의 TeamID를 가져와서 설정
        IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(PlayerController);
        if (TeamAgentInterface)
        {
            if (TeamAgentInterface->GetGenericTeamId())
            {
             
                SetGenericTeamId(TeamAgentInterface->GetGenericTeamId());
                PerceptionComponent->RequestStimuliListenerUpdate();
                //PerceptionComponent->RegisterWithPerceptionSystem();
                //UpdatePerception();
            }
        }
}


void ACEnemyAIController::ChangeTargetLocation(FVector InTargetLocation)
{
    TargetLocation = InTargetLocation;
}

FVector ACEnemyAIController::GetCurrentTargetLocation()
{
     return TargetLocation; 
}

void ACEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    Enemy = Cast<ACEnemy>(InPawn);

  
    if(Enemy->GetBehaviorTree() == nullptr) 
        return;
    if (!Enemy->HasAuthority()) return;

    RunAI();
    if (Blackboard == NULL) return;
    AIComponent = Cast<UCEnemyAIComponent>(Enemy->GetComponentByClass(UCEnemyAIComponent::StaticClass()));
    if(AIComponent == NULL) return;
    AIComponent->SetBlackboard(Blackboard); //AIComponent에 여기서 설정된 BlackBoard를 AIComponent 가 사용하는 BlackBoard 로 설정(추후에 블랙보드 참조하기 위함)
   // CDebug::Print(TEXT("Enemy Got BlackBoard : "), Enemy->GetName());

}

void ACEnemyAIController::RunAI()
{
    if (bIsBehaviorTreeInitialized)
    {
        //CDebug::Print(TEXT("Behavior Tree and Blackboard are already initialized"));
        return;
    };

    Blackboard = NewObject<UBlackboardComponent>(this);
    if (Blackboard && UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard))
    {
        RunBehaviorTree(Enemy->GetBehaviorTree());
        bIsBehaviorTreeInitialized = true;
    }
    
        // Blackboard를 각 인스턴스마다 독립적으로 설정
    //Blackboard = NewObject<UBlackboardComponent>(this);
    //if (Blackboard && UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard))
    //{
    //    // 동적으로 새로운 Behavior Tree 생성 및 할당
    //    UBehaviorTreeComponent* NewBehaviorTreeComponent = NewObject<UBehaviorTreeComponent>(this);
    //    NewBehaviorTreeComponent->StartTree(*Enemy->GetBehaviorTree(), EBTExecutionMode::Looped);
    //    BehaviorTreeComp = NewBehaviorTreeComponent;
    //    bIsBehaviorTreeInitialized = true; // 초기화 완료로 설정
    //    UE_LOG(LogTemp, Log, TEXT("Behavior Tree and Blackboard successfully initialized."));
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to initialize Blackboard or run Behavior Tree."));
    //}
}

void ACEnemyAIController::StopAI()
{
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (nullptr == BehaviorTreeComponent) return;

    BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

ETeamAttitude::Type ACEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
    const APawn* OtherPawn = Cast<APawn>(&Other);
    if (OtherPawn == nullptr)
    {
        return ETeamAttitude::Neutral;
    }

    const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
    if (OtherTeamAgent)
    {
        FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
        FGenericTeamId MyTeamID = GetGenericTeamId();
        
        if (OtherTeamId == MyTeamID)
        {
            return ETeamAttitude::Friendly; //Friendly
        }
        else if (OtherTeamId == FGenericTeamId(2))
        {
            return ETeamAttitude::Neutral;
        }
        else
        {
            return ETeamAttitude::Hostile; //Hostile
        }
    }

    return ETeamAttitude::Neutral;
}

FGenericTeamId ACEnemyAIController::GetGenericTeamId() const
{
    return TeamId;
}

void ACEnemyAIController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
    TeamId = NewTeamId;
}



void ACEnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ACEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
   

    if (!bIsAggroCoolDown && TargetActor) return;

    Perception->GetCurrentlyPerceivedActors(nullptr, CandidateActors); //이미 Sight 컨피그에 의해 플레이어를 적으로 인식하도록 함 

    //거리상 제일 가까운 Target 탐색 
    if (CandidateActors.Num() > 0)
    {
        float shortestDistance = 99999.0f;
        AActor* NewTargetActor= NULL;
        for (AActor* CandidateActor : CandidateActors)
        {
            if (GetPawn() == nullptr) return;
            float distanceTo  = GetPawn()->GetDistanceTo(CandidateActor);
            if (distanceTo < shortestDistance)
            {
                shortestDistance = distanceTo;
                NewTargetActor = CandidateActor;
            }
        }
        ChangeTarget(NewTargetActor);
        GetWorld()->GetTimerManager().SetTimer(AggroTimerHandle, this, &ACEnemyAIController::EnableAggroCoolDown, CooldownTime, false);
        CandidateActors.Empty();
    }
    return;

}



void ACEnemyAIController::EnableAggroCoolDown()
{
    bIsAggroCoolDown = true; //어그로 풀림 설정 .. 다시 탐색 시작 
}

void ACEnemyAIController::CustomTick()
{
    if (TargetActor == nullptr) return;
    if (GetPawn() == nullptr) return;
        if (GetPawn()->GetDistanceTo(TargetActor) >= 5000.0f)
    {
        TargetActor = NULL;
        Blackboard->SetValueAsObject("Target", TargetActor);
        GetWorld()->GetTimerManager().ClearTimer(AggroTimerHandle);
        EnableAggroCoolDown();
    }
        if (FriendlyTargetActor) 
        {
            TArray<AActor*> PerceivedActors;
            Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

            for (AActor* Actor : PerceivedActors)
            {
                if (Actor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Perceived Actor: %s"), *Actor->GetName());
                }
            }
        }
}

void ACEnemyAIController::ChangeTarget(AActor* InTarget) 
{
    if(TargetActor == InTarget) return;
    if (InTarget == FriendlyTargetActor) return;
    TargetActor = InTarget;
    bIsAggroCoolDown = false;
    Blackboard->SetValueAsObject("Target", TargetActor); 
    

}

AActor* ACEnemyAIController::GetTarget()
{
    return TargetActor;
}

void ACEnemyAIController::ChangeFriendlyTarget(AActor* InTarget)
{
    if (InTarget == nullptr) return;
    if (FriendlyTargetActor == InTarget) return;

    FriendlyTargetActor = InTarget;
    ChangeTarget(nullptr);
    bIsAggroCoolDown = false;
    Blackboard->SetValueAsObject("FriendlyTarget", FriendlyTargetActor);
}

AActor* ACEnemyAIController::GetFriendlyTarget()
{
    return FriendlyTargetActor;
}

void ACEnemyAIController::ForceUpdatePerception()
{
    if (GetPerceptionComponent())
    {
        // 현재 감지된 모든 액터를 잊게 함으로써 Perception을 강제로 업데이트
        TArray<AActor*> AllActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

        for (AActor* Actor : AllActors)
        {
            APawn* OtherPawn = Cast<APawn>(Actor);
            if (OtherPawn)
            {
                GetPerceptionComponent()->ForgetActor(Actor);

            }
           
        }

    }
}