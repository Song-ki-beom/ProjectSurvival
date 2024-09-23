

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
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "BehaviorTree/BlackboardData.h"
#include "Utility/CDebug.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; //리플리케이트 설정 

	GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(this));



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
		int32 NewAttackIdx = FMath::RandRange(0, DoActionDatas.Num() - 1); //플레이할 몽타주 랜덤 넘버 설정
		BroadcastDoAction(NewAttackIdx);
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
	AttackIdx = InAttackIdx;
	MovingComponent->EnableControlRotation();
	MontageComponent->Montage_Play(DoActionDatas[InAttackIdx].Montage, 1.0f);
}

void ACEnemy::AttackTraceHit()
{

	//Trace 관련 세팅
	FVector ForwardVector = GetActorForwardVector();
	FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y,GetActorLocation().Z + 45.0f) + ForwardVector.GetSafeNormal() * TraceOffset;
	FVector End = Start + ForwardVector.GetSafeNormal() * TraceDistance;
	FQuat Rot = FQuat(GetActorRotation());

	FVector HalfSize = FVector(TraceDistance / 2.0f, TraceDistance / 1.0f, TraceDistance / 1.0f);
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);


	//BoxTrace 
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		Rot,
		ECC_Visibility,
		FCollisionShape::MakeBox(HalfSize),
		CollisionParams
	);

	DrawDebugBox(GetWorld(), Start, HalfSize, Rot, FColor::Red, false, 1.0f);
	DrawDebugBox(GetWorld(), End, HalfSize, Rot, FColor::Red, false, 1.0f);


	TArray<ACharacter*> HitPlayers;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			ACharacter* HitPlayer;
			if ((Hit.GetActor() != nullptr) && (HitPlayer = Cast<ACharacter>(Hit.GetActor())))
			{
				FActionDamageEvent e;
				e.HitID = DoActionDatas[AttackIdx].ActionID;
				HitPlayer->TakeDamage(0, e, this->GetController(),this);

			}
		}
	}
}

void ACEnemy::Begin_DoAction()
{
	StateComponent->ChangeType(EStateType::Action);
	if (!DoActionDatas[AttackIdx].bCanMove)
		MovingComponent->Stop();
}

void ACEnemy::End_DoAction()
{
	StateComponent->ChangeType(EStateType::Idle);
	if (!DoActionDatas[AttackIdx].bCanMove)
		MovingComponent->Move();
}




void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) 
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.HitID = ((FActionDamageEvent*)&DamageEvent)->HitID;

	
	if (!DamageData.HitID.IsNone())
	{
		ApplyHitData();
	}

	return damage;

}

void ACEnemy::ApplyHitData()
{
	UDataTable* HitDataTable = nullptr;
	if (GameInstance)
	{
		HitDataTable = GameInstance->HitDataTable;
	}

	if (HitDataTable != nullptr)
	{
		FHitData* Row = HitDataTable->FindRow<FHitData>(DamageData.HitID, FString(""));
		if (Row && Row->Montage)
		{
			MontageComponent->Montage_Play(Row->Montage, Row->PlayRate);
		}
	}
	//StatusComponent->ApplyDamage(Damage.power);
	//Damage.power = 0;

	//// ���󺯰�
	//{
	//	Change_Color(this, FLinearColor::Red);
	//	FTimerDelegate  timerDelegate;
	//	timerDelegate.BindUFunction(this, "RestoreColor");
	//	GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);
	//}

	//// Montage->HitStop->Sound->Effect

	//if (!!Damage.Event && !!Damage.Event->HitData)
	//{
	//	// Montage
	//	Damage.Event->HitData->PlayMontage(this);
	//	// HitStop
	//	Damage.Event->HitData->PlayHitStop(GetWorld());
	//	// Sound 
	//	Damage.Event->HitData->PlaySoundWave(this);
	//	// Effect 
	//	Damage.Event->HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

	//	if (!Status->IsDead())
	//	{
	//		FVector start = GetActorLocation();
	//		FVector target = Damage.Character->GetActorLocation();
	//		FVector direction = target - start; direction = direction.GetSafeNormal();

	//		LaunchCharacter(-direction * Damage.Event->HitData->Launch, false, false);
	//		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	//	}
	//	if (Status->IsDead())
	//	{
	//		State->SetDeadMode();
	//		return;
	//	}
	//}
	////	Damage.Character = nullptr;
	//Damage.Causer = nullptr;
	//Damage.Event = nullptr;


}

void ACEnemy::Die()
{


}





//리플리케이트 변수 선언부분 
//void ACEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ACEnemy, AttackIdx);
//	
//}

