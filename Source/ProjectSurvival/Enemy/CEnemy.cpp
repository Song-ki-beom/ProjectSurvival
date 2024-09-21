

#include "Enemy/CEnemy.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMontageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/CEnemyAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Enemy/CEnemyAIController.h"
#include "Net/UnrealNetwork.h"
#include "BehaviorTree/BlackboardData.h"
#include "Utility/CDebug.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; //리플리케이트 설정 



	//AI 세팅
	static ConstructorHelpers::FClassFinder<ACEnemyAIController> AIControllerFinder(TEXT("Blueprint'/Game/PirateIsland/Include/Blueprints/Character/Animal/Bear/BP_CEnemyAIController_Bear.BP_CEnemyAIController_Bear_C'"));
	if (AIControllerFinder.Class != nullptr)
	{
		AIControllerClass = AIControllerFinder.Class; //AIController 클래스 설정
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //이 캐릭터가 스폰될때 AIController 에 의해 빙의됨 
	}

	//Component Setting & Replicate
	StatusComponent = CreateDefaultSubobject<UCStatusComponent>(TEXT("StatusComponent"));
	StatusComponent->SetIsReplicated(true);

	MovingComponent = CreateDefaultSubobject<UCMovingComponent>(TEXT("MoveComponent"));
	MovingComponent->SetIsReplicated(true);
	

	AIComponent = CreateDefaultSubobject<UCEnemyAIComponent>(TEXT("AIComponent"));
	AIComponent->SetIsReplicated(true);

	StateComponent = CreateDefaultSubobject<UCStateComponent>(TEXT("StateComponent"));
	StateComponent->SetIsReplicated(true);

	MontageComponent = CreateDefaultSubobject<UCMontageComponent>(TEXT("MontageComponent"));
	MontageComponent->SetIsReplicated(true);
	

	//Mesh Setting
	{
			GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
			GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	
	

}

void ACEnemy::BeginPlay()
{

	Super::BeginPlay();

	//동적 리소스 로딩을 위한 에셋 로더 생성
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();


	//FSoftObjectPath 와 AssetLoader 를 이용한 지연 로딩 


	//SkeletalMesh 
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(AssetLoader.LoadSynchronous(FSoftObjectPath(SkeletalMeshPath)));

	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		
	}

	//AnimInstance 
	UClass* AnimInstance = Cast<UClass>(AssetLoader.LoadSynchronous(FSoftObjectPath(AnimInstancePath)));

	if (AnimInstance != nullptr)
	{
		GetMesh()->SetAnimClass(AnimInstance);
	}


	

	//BB
	UBlackboardData* BBAsset = Cast<UBlackboardData>(AssetLoader.LoadSynchronous(FSoftObjectPath(BBAssetPath)));
	if (BBAsset != nullptr)
	{
		GetBehaviorTree()->BlackboardAsset = BBAsset;
	}


	// AI 컨트롤러가 제대로 빙의되었는지 확인
	AIController = Cast<ACEnemyAIController>(GetController());
	if (AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController has successfully possessed the character"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AIController failed to possess the character"));
	}

	MovingComponent->SetSpeed(ESpeedType::Walk);
	MovingComponent->DisableControlRotation();
	// 이동 방향에 따라 회전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemy::DoAction()
{
	if (HasAuthority())
	{
		int32 AttackIdx = FMath::RandRange(0, DoActionDatas.Num() - 1);
		BroadcastDoAction(AttackIdx);
	}
	
}

void ACEnemy::BroadcastDoAction_Implementation(int32 InAttackIdx)
{
	PerformDoAction(InAttackIdx);
}


void ACEnemy::RequestDoAction_Implementation()
{
	DoAction();
}

void ACEnemy::PerformDoAction(int32 InAttackIdx)
{
	MovingComponent->EnableControlRotation();
	StateComponent->ChangeType(EStateType::Action);
	if (this->HasAuthority()) 
	{
		CDebug::Print(TEXT(" On Server Attack Index has Changed To :") + FString::FromInt(InAttackIdx));

	}
	else
	{
		CDebug::Print(TEXT("On Client Attack Index has Changed To :")+ FString::FromInt(InAttackIdx));
	}
	MontageComponent->Montage_Play(DoActionDatas[InAttackIdx].Montage, 1.0f);
}

void ACEnemy::EndDoAction()
{

}



void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACEnemy::Damage(ACharacter* Attacker, AActor* Causer, FHitData HitData)
{

	StatusComponent->TakeDamage(HitData.Power);

}



//void ACEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ACEnemy, AttackIdx);
//	
//}

