

#include "Enemy/CEnemy.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CMontageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Widget/Status/CEnemyStatusBar.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Enemy/CEnemyAIController.h"
#include "Animation/CEnemyAnimInstance.h"
#include "World/CPickUp.h"
#include "Widget/Inventory/CItemBase.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Net/UnrealNetwork.h"
#include "BehaviorTree/BlackboardData.h"
#include "Engine/PackageMapClient.h"
#include "Utility/CDebug.h"
#include "Camera/PlayerCameraManager.h"
#include "Widget/Map/CWorldMap.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; 
	SetReplicates(true);
	bUseControllerRotationYaw = true;
	Tags.Add(FName("Enemy"));
	SetGenericTeamId(FGenericTeamId(3));
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Inventory/DT_Items.DT_Items'"));
	if (DataTableAsset.Succeeded())
	{
		ItemDataTable = DataTableAsset.Object;
	}

	//AI 세팅
	/*static ConstructorHelpers::FClassFinder<ACEnemyAIController> AIControllerFinder(TEXT("Blueprint'/Game/PirateIsland/Include/Blueprints/Character/Animal/Bear/BP_CEnemyAIController_Bear.BP_CEnemyAIController_Bear_C'"));*/
	/*if (AIControllerFinder.Class != nullptr)
	{*/
		//AIControllerClass = AIControllerFinder.Class; //AIController 클래스 설정..이 클래스를 기반으로 게임 시작 시 자동설정
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //이 캐릭터가 스폰될때 AIController 에 의해 빙의됨 
	//}

	//Component Setting & Replicate
	StatusComponent = CreateDefaultSubobject<UCStatusComponent>(TEXT("StatusComponent"));
	StatusComponent->SetIsReplicated(true);
	StatusComponent->OnBecameFriendly.AddDynamic(this, &ACEnemy::OnBecameFriendlyHandler);

	MovingComponent = CreateDefaultSubobject<UCMovingComponent>(TEXT("MoveComponent"));
	MovingComponent->SetIsReplicated(true);
	
	EnemyAIComponent = CreateDefaultSubobject<UCEnemyAIComponent>(TEXT("EnemyAIComponent"));
	EnemyAIComponent->SetIsReplicated(true);
	

	StateComponent = CreateDefaultSubobject<UCStateComponent>(TEXT("StateComponent"));
	StateComponent->SetIsReplicated(true);
	StateComponent->OnStateTypeChanged.AddDynamic(this,&ACEnemy::OnStateTypeChangedHandler);

	MontageComponent = CreateDefaultSubobject<UCMontageComponent>(TEXT("MontageComponent"));
	MontageComponent->SetIsReplicated(true);
	

	//Mesh Setting
	{
			GetMesh()->SetRelativeLocation(FVector(-45, 0, -90));
			GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			GetMesh()->bMultiBodyOverlap = false; //본마다 Object Trace 되는 현상 방지 
	}

	
	SlopeCheckArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SlopeCheckArrow")); //경사 확인을 위한 Arrow Component
	SlopeCheckArrow->SetupAttachment(RootComponent);
	SlopeCheckArrow->ArrowSize = 2.0f;
	SlopeCheckArrow->ArrowColor = FColor::Red;
	SlopeCheckArrow->bHiddenInGame = false;
	SlopeCheckArrow->SetVisibility(true);


	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	// 박스 크기 설정
	BoxCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Pawn 제외 모두 블록 처리
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
	
	
	BoxCollision->bHiddenInGame = false;
	BoxCollision->SetVisibility(true);

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/Status/WBP_EnemyStatusBar.WBP_EnemyStatusBar_C'"));
	if (HPBarClassFinder.Succeeded())
	{
		HPBarWidgetClass = HPBarClassFinder.Class;
	}

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHPBar"));
	HPBarWidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 25.0f));
	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HPBarWidgetComponent->SetDrawAtDesiredSize(true);
	//HPBarWidgetComponent->SetDrawSize(FVector2D(150.f, 30.f));
	HPBarWidgetComponent->SetWidgetClass(HPBarWidgetClass);
	HPBarWidgetComponent->InitWidget();
	HPBarWidgetComponent->SetVisibility(true);
	//HPBarWidgetComponent->PrimaryComponentTick.bCanEverTick = true;
	//HPBarWidgetComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;

		//AI 피아식별 세팅
	  // AIPerceptionStimuliSourceComponent 생성
	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));

	// 시각 또는 청각 같은 인식 가능한 자극을 등록
	PerceptionStimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	PerceptionStimuliSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());

	// AI에 의한 인식 자극을 활성화
	PerceptionStimuliSource->bAutoRegister = true;

	GetCharacterMovement()->bUseRVOAvoidance = true;  // RVO 회피 활성화
	GetCharacterMovement()->AvoidanceConsiderationRadius = 2000.0f;  // 회피 반경 설정
	GetCharacterMovement()->AvoidanceWeight = 2.0f;  // 회피 우선순위 설정
	
}

void ACEnemy::BeginPlay()
{
	HPBarWidgetComponent->SetCastShadow(false); //명암 효과 x 

	Super::BeginPlay();
	NetDriver = GetWorld()->GetNetDriver();
	NetUpdateFrequency = 144.0f;
	NetPriority = 4.0f;
	GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(this));
	//동적 리소스 로딩을 위한 에셋 로더 생성
	FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();


	//FSoftObjectPath 와 AssetLoader 를 이용한 지연 로딩 


	//SkeletalMesh 
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(AssetLoader.LoadSynchronous(FSoftObjectPath(SkeletalMeshPath)));

	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
	}
	CreateDynamicMaterial(); //Mesh 에서 원본 Mesh 색 추출, 인스턴스 머터리얼 생성 

	//AnimInstance 
	UClass* AnimInstance = Cast<UClass>(AssetLoader.LoadSynchronous(FSoftObjectPath(AnimInstancePath)));

	if (AnimInstance != nullptr)
	{
		GetMesh()->SetAnimClass(AnimInstance);
	}


	if (GetMesh()->GetAnimInstance())
	{
		AfterABPBindDynamic();
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

	//StateComponent->ChangeType(EStateType::Idle);
	MovingComponent->SetSpeed(ESpeedType::Walk);
	MovingComponent->DisableControlRotation();
	// 이동 방향에 따라 회전하도록 설정
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
		//몽타주 끝나는 시점 관련 바인딩 
	UCEnemyStatusBar* EnemyStatusBar = Cast<UCEnemyStatusBar>(HPBarWidgetComponent->GetWidget());
	if (EnemyStatusBar)
	{
		// OwnerEnemyCharacter로 현재 적을 설정 
		EnemyStatusBar->InitializeEnemyCharacter(this);
	}

	if (MovingComponent == nullptr)
	{
		CDebug::Print(TEXT("MovingComponent is missing in BeginPlay"));
	}
	else
	{
		CDebug::Print(TEXT("MovingComponent is valid in BeginPlay"));
	}


	if (EnemyAIComponent == nullptr)
	{
		CDebug::Print(TEXT("EnemyAIComponent is missing in BeginPlay"));
	}
	else
	{
		CDebug::Print(TEXT("EnemyAIComponent is valid in BeginPlay"));
	}
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//RotateMeshToSlope(DeltaTime);

	if (IsValid(GetWorld()) && GetWorld()->IsClient())  // 클라이언트에서만 실행
	{
		if (IsValid(HPBarWidgetComponent) && HPBarWidgetComponent->IsVisible())
		{
			if (GameInstance->WorldMap)
			{
				AActor* PlayerActor = HitData->FindActorByNetGUID(GameInstance->WorldMap->GetPersonalGUID(), GetWorld());
				PlayerCharacter = Cast<ACharacter>(PlayerActor);
			}
			else
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); //로컬 플레이어(플레이어)
				PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
			}

		



			if (PlayerCharacter == nullptr) return;

			UCameraComponent* CameraComponent = PlayerCharacter->FindComponentByClass<UCameraComponent>();
			if (CameraComponent == nullptr)
			{
				CDebug::Print("CameraComponent Null");
				return;
			}
			FVector CameraLocation = CameraComponent->GetComponentLocation();
			//FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetCameraLocation();
			FVector VectorTowardPlayer = CameraLocation - HPBarWidgetComponent->GetComponentLocation();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(VectorTowardPlayer).Rotator();
			//CDebug::Print("CameraLocation", CameraLocation);

			// 위젯이 카메라를 향하도록 회전
			HPBarWidgetComponent->SetWorldRotation(LookAtRotation);


			

		}
	}
	if (EnemyAIComponent == nullptr)
		CDebug::Print(TEXT("EnemyAIComponent Missing"));

	if (bChangeMesh)
	{
		// 스켈레탈 메쉬 로드 (LoadObject를 통해 FString 경로를 사용)
		USkeletalMesh* NewMesh = LoadObject<USkeletalMesh>(nullptr, *FriendlyMeshPath);

		if (NewMesh)
		{
			// 애니메이션이 실행 중이면 중지
			if (GetMesh()->IsPlayingRootMotion())
			{
				GetMesh()->Stop();
				return;
			}
			if (!GetMesh()->IsPlayingRootMotion())
			{
				GetMesh()->SetSkeletalMesh(NewMesh);
				bChangeMesh = false;
				//GetMesh()->Play(true);

				CreateDynamicMaterial(); //Mesh 에서 원본 Mesh 색 추출, 인스턴스 머터리얼 생성 
			}
				
		}
	}
}

float ACEnemy::DoAction()
{
	
	if (HasAuthority())
	{
		int32 NewAttackIdx = FMath::RandRange(0, DoActionDatas.Num() - 1); //플레이할 몽타주 랜덤 넘버 설정
		 BroadcastDoAction(NewAttackIdx);
		 return MontageComponent->GetMontageDelay();
	}
	return -1;
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
	Begin_DoAction();
	MontageComponent->Montage_Play(DoActionDatas[InAttackIdx].Montage, 1.0f);
}


void ACEnemy::AttackTraceHit()
{

	//Trace 관련 세팅
	FVector ForwardVector = GetActorForwardVector();
	FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y,GetActorLocation().Z + 10.0f) + ForwardVector.GetSafeNormal() * TraceOffset;
	FVector End = Start + ForwardVector.GetSafeNormal() * TraceDistance;
	FQuat Rot = FQuat(GetActorRotation());

	FVector HalfSize = FVector(TraceDistance / 0.7f, TraceDistance / 1.0f, TraceDistance / 0.5f);
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams objectQueryParams; //다중 검출 PARAM
	objectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	//BoxTrace 
	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		Start,
		End,
		Rot,
		objectQueryParams,
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
			ACharacter* HitCharacter;
			UPrimitiveComponent* HitComponent = Hit.GetComponent();
			if (HitComponent && HitComponent->IsA<USkeletalMeshComponent>()) //스켈레탈 메시가 검출되었는지
			{
				FActionDamageEvent e;
				continue; // Skeletal Mesh는 무시
			}
			else if ((Hit.GetActor() != nullptr) && (HitCharacter = Cast<ACharacter>(Hit.GetActor()))) //Character 로 형변환 가능하면 적에게 데미지
			{
				
				FActionDamageEvent e;
				e.HitID = DoActionDatas[AttackIdx].ActionID;
				HitCharacter->TakeDamage(0, e, this->GetController(), this);

			}
			

		}
	
	
	
	}
}

void ACEnemy::Begin_DoAction()
{
	StateComponent->ChangeType(EStateType::Action);
	if (!DoActionDatas[AttackIdx].bCanMove)
	{
		if (HasAuthority())
		AIController->StopMovement();
	}
	MovingComponent->EnableControlRotation();
}

void ACEnemy::End_DoAction()
{
	StateComponent->ChangeType(EStateType::Combat);
	//if (!DoActionDatas[AttackIdx].bCanMove)
		//GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	MovingComponent->DisableControlRotation();
}

float ACEnemy::DoEncounter()
{
	if (HasAuthority())
	{
		BroadcastDoSpecialAction(ESpecialState::Encounter);
		return MontageComponent->GetMontageDelay();
	}
	return -1;
}

void ACEnemy::PerformDoSpecialAction(ESpecialState SpecialState)
{
	MontageComponent->Montage_Play(DoSpecialActionDatas[(int32)SpecialState].Montage, DoSpecialActionDatas[(int32)SpecialState].PlayRate);
}

void ACEnemy::BroadcastDoSpecialAction_Implementation(ESpecialState SpecialState)
{
	PerformDoSpecialAction(SpecialState);
}

void ACEnemy::BroadcastDisableCollision_Implementation()
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACEnemy::BroadcastDisableMesh_Implementation()
{
	GetMesh()->SetVisibility(false);
}


void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACEnemy::BroadcastChangeMesh_Implementation()
{
		bChangeMesh = true;
}

float ACEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) 
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (StatusComponent->IsDead()) return -1;
	IGenericTeamAgentInterface* TeamAgentInterface = Cast<IGenericTeamAgentInterface>(EventInstigator);
	if (TeamAgentInterface == nullptr) return -1;
	if (TeamAgentInterface->GetGenericTeamId() == GetGenericTeamId()) return -1;
	if (this->HasAuthority())
	{
		if (NetDriver && NetDriver->GuidCache)
		{
			//네트워크에서 액터 ID 식별하는 과정
			FNetworkGUID InstigatorNetGUID = NetDriver->GuidCache->GetOrAssignNetGUID(EventInstigator->GetPawn());
			if (!InstigatorNetGUID.IsValid()) return -1;
			DamageData.CharacterID = InstigatorNetGUID.Value;
			FNetworkGUID CauserNetGUID = NetDriver->GuidCache->GetOrAssignNetGUID(DamageCauser);
			if (!CauserNetGUID.IsValid()) return -1;
			DamageData.CauserID = CauserNetGUID.Value;
			DamageData.HitID = ((FActionDamageEvent*)&DamageEvent)->HitID;
		}


	}




	if (!DamageData.HitID.IsNone())
	{
		BroadCastApplyHitData(DamageData);
	}

	return damage;

}

void ACEnemy::AfterABPBindDynamic()
{
	if (MontageComponent && (GetMesh()->GetAnimInstance()))
	{
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(MontageComponent, &UCMontageComponent::OnMontageNotifyBegin);
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyEnd.AddDynamic(MontageComponent, &UCMontageComponent::OnMontageNotifyEnd);
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(MontageComponent, &UCMontageComponent::OnMontageEnded);
		MontageComponent->OnMontageFinalEnded.AddDynamic(this, &ACEnemy::OnMontageFinalEnded);
		MontageComponent->OnMontageInterrupted.AddDynamic(this, &ACEnemy::OnMontageInterrupted);
		MontageComponent->OnPlayMontageNotifyBegin.AddDynamic(this, &ACEnemy::OnPlayMontageNotifyBegin);
		MontageComponent->OnPlayMontageNotifyEnd.AddDynamic(this, &ACEnemy::OnPlayMontageNotifyEnd);

	}
}


void ACEnemy::BroadCastApplyHitData_Implementation(FDamageData InDamageData)
{
	if (!HasAuthority())
	{
		DamageData = InDamageData;
	}
	ApplyHitData();
}



void ACEnemy::ApplyHitData()
{
	if (!HPBarWidgetComponent->IsVisible())
		HPBarWidgetComponent->SetVisibility(true);
	UDataTable* HitDataTable = nullptr;
	if (GameInstance)
	{
		HitDataTable = GameInstance->HitDataTable;
	}


	if (HitDataTable != nullptr)
	{
		CDebug::Print(TEXT("Hit Start"));
		FName CompleteHitID = FName(*(DamageData.HitID.ToString()) + HitActorName);
		HitData = HitDataTable->FindRow<FHitData>(CompleteHitID, FString("Hit_Bear"));
		if (HitData && HitData->Montage)
		{
			if (StatusComponent != nullptr && this->HasAuthority())
			{
				StatusComponent->ApplyDamage(HitData->DamageAmount);
			}
			
			HitData->PlaySoundWave(this);
			HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());
			hitCnt++;
			if (!StatusComponent->IsDead())
			{
				ChangeMeshColor(FLinearColor::Red); //Material Change 
				GetWorld()->GetTimerManager().SetTimer(ResetColorTimerHandle, this, &ACEnemy::ResetColor, 0.7f, false);
				FVector start = GetActorLocation();
				UObject* FoundObject = NetDriver->GuidCache->GetObjectFromNetGUID(DamageData.CharacterID, true);
				AActor* targetActor = HitData->FindActorByNetGUID(DamageData.CharacterID, GetWorld());
				

				FVector target = targetActor->GetActorLocation();
				FVector direction = target - start;
				direction = direction.GetSafeNormal();

				if (hitCnt >= MaxhitCnt)
				{
					MontageComponent->Montage_Play(HitData->Montage, HitData->PlayRate); //몽타주 재생 
					if (this->HasAuthority()) AIController->ChangeTarget(targetActor); //타겟 변경 
					//Look At
					FRotator LookAtRotation = FRotationMatrix::MakeFromX(direction).Rotator();
					LookAtRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
					SetActorRotation(LookAtRotation);
					LaunchCharacter(-direction * HitData->Launch, false, false);
					hitCnt = 0;
				}
				else
				{
					LaunchCharacter(-direction * HitData->Launch*(0.75f), false, false);
				}
		

			}

			if (StatusComponent->IsDead())
			{
				
				StateComponent->ChangeType(EStateType::Dead);
				if(this->HasAuthority()) Die();
				return;
			}

		}
		DamageData.CharacterID = 0;
		DamageData.CauserID = 0;
		DamageData.HitID = "";
	}
	

}

void ACEnemy::Die()
{
	
	if (AIController)
	{
		AIController->StopMovement(); // AI가 현재 수행 중인 움직임 멈춤
		AIController->BrainComponent->StopLogic("Enemy Dead"); // AI의 행동 로직 중지
	}
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	BroadcastDoSpecialAction(ESpecialState::Dead);
	BroadcastDisableCollision();
	
}

void ACEnemy::RemoveCharacter()
{
	BroadcastDisableMesh();
	CreateDropItem();
	GetWorld()->GetTimerManager().SetTimer(RemoveTimerHandle, this, &ACEnemy::DestroyEnemy, 1.0f, false);

}

void ACEnemy::DestroyEnemy()
{
	Destroy();
}

void ACEnemy::OnStateTypeChangedHandler(EStateType PrevType, EStateType NewType)
{

}

void ACEnemy::OnBecameFriendlyHandler()
{
	if (EnemyAIComponent && HasAuthority())
	{
		BroadcastChangeMesh();
		BroadcastUpdateHealthBar(FLinearColor::Green);
		StatusComponent->BroadcastCancelExhausted();
		EnemyAIComponent->ChangeAIStateType(EAIStateType::Wait);
		EnemyAIComponent->ChangeAIReputationType(EAIReputationType::Friendly);

		FVector TraceStart = GetActorLocation();

		TArray<FHitResult> HitResults;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueryParams; //다중 검출 PARAM
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		float SearchSphereRadius = 1000.0f;
		// 멀티 스피어 트레이스 실행
		bool bHit = GetWorld()->SweepMultiByObjectType(
			HitResults,
			TraceStart,
			TraceStart,
			FQuat::Identity,
			ObjectQueryParams,
			FCollisionShape::MakeSphere(SearchSphereRadius),
			QueryParams
		);
		DrawDebugSphere(GetWorld(), TraceStart, SearchSphereRadius, 50, FColor::Green, false, 0.1f);


		if (bHit)
		{
			AActor* NewTargetActor = NULL;
			float shortestDistance = 99999.0f;
			for (const FHitResult& Hit : HitResults)
			{

				if (IsValid(Hit.GetActor()))
				{
					if (ACSurvivor* survivor = Cast<ACSurvivor>(Hit.GetActor()))
					{
						if (!Hit.GetActor()->ActorHasTag("Player")) continue;
						float distanceTo = GetDistanceTo(Hit.GetActor());
						if (distanceTo < shortestDistance)
						{
							shortestDistance = distanceTo;
							NewTargetActor = Hit.GetActor();
						}
					}
				}
			}

			if (NewTargetActor)
			{
				AIController->ChangeFriendlyTarget(NewTargetActor);
				APawn* TargetPawn = Cast<APawn>(NewTargetActor);
				if (TargetPawn)
				{
					APlayerController* PlayerController = Cast<APlayerController>(TargetPawn->GetController());
					if (PlayerController)
					{
						AIController->ChangeTarget(NULL);
						SetGenericTeamId(FGenericTeamId(1));
						AIController->ForceUpdatePerception();
						AIController->TameEnemyToPlayer(PlayerController);
						AIController->EnableAggroCoolDown();

					}
				}
			}
		}
	}
}

void ACEnemy::BroadcastUpdateHealthBar_Implementation(FLinearColor InColor)
{
	UCEnemyStatusBar* EnemyStatusBar = Cast<UCEnemyStatusBar>(HPBarWidgetComponent->GetWidget());
	if (EnemyStatusBar)
	{
		EnemyStatusBar->UpdateHealthColor(FLinearColor::Green);
	}
}

void ACEnemy::RotateMeshToSlope(float InDeltaTime)
{

	//FVector Start = SlopeCheckArrow->GetComponentLocation();
	//FVector End = Start + (SlopeCheckArrow->GetUpVector() * (-500.0f));  //아래로 라인트레이스
	//FHitResult HitResult;
	////FCollisionQueryParams TraceParams;
	////TraceParams.

	//FCollisionQueryParams TraceParams;
	//TraceParams.bTraceComplex = true;
	//TraceParams.AddIgnoredActor(this);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);

	//if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
	//{
	//	FVector ImpactNormal = HitResult.ImpactNormal;
	//	FVector ImpactStart = HitResult.ImpactPoint;
	//	FVector ImpactEnd = ImpactStart + (ImpactNormal * 150.0f);
	//	DrawDebugLine(GetWorld(), ImpactStart, ImpactEnd, FColor::Green, false, 1.0f, 0, 2.0f);


	//	//FRotator TargetRotator = FRotationMatrix::MakeFromZ(ImpactNormal).Rotator();

	//	FRotator MeshRotator = GetMesh()->GetComponentRotation();
	//	
	//	FMatrix MeshRightMatrix = FRotationMatrix::MakeFromYZ(GetMesh()->GetRightVector(),ImpactNormal);
	//	FRotator MeshRightRotator = MeshRightMatrix.Rotator();

	//	FMatrix MeshForwardMatrix = FRotationMatrix::MakeFromYZ(GetMesh()->GetForwardVector(), ImpactNormal);
	//	FRotator MeshForwardRotator = MeshForwardMatrix.Rotator();


	//	FRotator TargetRotator = FRotator(MeshRightRotator.Pitch, MeshRotator.Yaw,MeshForwardRotator.Roll);
	//	//BoxCollision
	//	TargetRotator = FMath::RInterpTo(MeshRotator, TargetRotator, InDeltaTime, 5.0f);
	//	GetMesh()->SetWorldRotation(TargetRotator);
	//	
	//}

}

void ACEnemy::OnMontageFinalEnded()
{
	End_DoAction();

}

void ACEnemy::OnMontageInterrupted()
{
	End_DoAction();
}

void ACEnemy::OnPlayMontageNotifyBegin()
{
	if (this->HasAuthority() && StatusComponent->IsDead())
	{
		RemoveCharacter();
	}
}

void ACEnemy::OnPlayMontageNotifyEnd()
{
	
}


void ACEnemy::CreateDynamicMaterial()
{
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		
		UMaterialInterface* material = GetMesh()->GetMaterials()[i];
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(material);
		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(material, this);
			GetMesh()->SetMaterial(i, DynamicMaterial);
			if (i == 0)
			{
				FMaterialParameterInfo ParameterInfo(TEXT("Color")); 
				DynamicMaterial->GetVectorParameterValue(ParameterInfo, OriginalMeshColor);

			}

		}
		
	}

	
}

void ACEnemy::ChangeMeshColor(FLinearColor InColor)
{
	for (UMaterialInterface* material : GetMesh()->GetMaterials())
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);
		if (!!material)
			instance->SetVectorParameterValue("Color", InColor);
	}
}

void ACEnemy::ResetColor()
{
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(i));
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("Color",OriginalMeshColor);
		}
	}
}


void ACEnemy::CreateDropItem()
{

	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DropItemID, DropItemID.ToString());
	if (ItemData)
	{
		UCItemBase* ItemToDrop = NewObject<UCItemBase>(UCItemBase::StaticClass());
		ItemToDrop->CopyFromItemData(*ItemData);
	}


	// 디스트럭티블 메쉬의  절반 높이 가져오기 (Z 축 기준)
	FVector MeshBounds = GetMesh()->Bounds.BoxExtent;
	float HalfHeight = MeshBounds.Z;
	// 스폰 위치 설정 시 절반 높이를 고려하여 Y축에만 오프셋을 적용
	FVector SpawnLocation = this->GetActorLocation() + FVector(0.0f, 0.0f, HalfHeight * 0.7f);

	for (int i = 0; i < DropItemNum; i++)
	{
		// 겹치지 않도록 랜덤 오프셋 추가
		FVector RandomOffset = FVector(
			FMath::RandRange(-DropOffsetRange, DropOffsetRange),
			FMath::RandRange(-DropOffsetRange, DropOffsetRange),
			0.0f //높이는 그대로 
		);
		FVector FinalSpawnLocation = SpawnLocation + RandomOffset;
		FTransform SpawnTransform(this->GetActorRotation(), FinalSpawnLocation); //최종 스폰 위치 

		//스폰 파라미터 설정 
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 벽에 끼이는 등  spawn 에 실패하는 상황이 생기면 위치를 Adjust해서 성공시킴 

		ACPickUp* Pickup = GetWorld()->SpawnActor<ACPickUp>(ACPickUp::StaticClass(), SpawnTransform, SpawnParams);
		Pickup->InitializeDrop(DropItemID, 1);

	}



}

void ACEnemy::EatFood(ACPickUp* TargetPickUp)
{
	StatusComponent->RecoverHunger(TargetPickUp->ItemReference->ItemStats.DamageValue);
	StatusComponent->StackFriendShip(TargetPickUp->ItemReference->ItemStats.DamageValue*3);
	TargetPickUp->Destroy();
}

void ACEnemy::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	TeamId = NewTeamId;
}


//리플리케이트 변수 선언부분 
//void ACEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(ACEnemy, AttackIdx);
//	
//}

