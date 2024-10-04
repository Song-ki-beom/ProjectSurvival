
#include "CSurvivor.h"
#include "ActorComponents/Disposable/CCustomizeComponent.h"
#include "ActorComponents/CWeaponComponent.h"
#include "ActorComponents/CHarvestComponent.h"
#include "ActorComponents/CBuildComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CInteractionComponent.h"
#include "ActorComponents/CInventoryComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "ActorComponents/CMontageComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h" //리플리케이트를 위한 헤더 파일 
#include "Engine/NetSerialization.h" 
#include "Engine/NetDriver.h"    // UNetDriver 및 FNetGUIDCache가 포함된 헤더
#include "Misc/NetworkGuid.h" //FNetworkGUID 정의가 포함된 헤더 파일
#include "CGameInstance.h"
#include "Utility/CDebug.h"
#include "Widget/CMainHUD.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Widget/Map/CWorldMap.h"
#include "Widget/Map/CMiniMap.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/CSurvivorController.h"
#include "GameFramework/PlayerState.h"
#include "Widget/Map/CWorldMap.h"
#include "EngineUtils.h"
#include "Engine/PackageMapClient.h"


ACSurvivor::ACSurvivor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicates(true);
	Tags.Add(FName("Player"));
	//인터렉션 세팅
	BaseEyeHeight = 67.0f; //Pawn의 Default 눈 높이 세팅

	GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(this));

	//컴포넌트 세팅
	WeaponComponent = CreateDefaultSubobject<UCWeaponComponent>(TEXT("Weapon"));
	WeaponComponent->SetIsReplicated(true);
	InventoryComponent = CreateDefaultSubobject<UCInventoryComponent>(TEXT("Inventory"));
	InventoryComponent->SetSlotsCapacity(20);
	InventoryComponent->SetWeightCapacity(200.0f);
	InventoryComponent->SetIsReplicated(true);
	HarvestComponent = CreateDefaultSubobject<UCHarvestComponent>(TEXT("Harvest"));
	HarvestComponent->SetIsReplicated(true);
	InteractionComponent = CreateDefaultSubobject<UCInteractionComponent>(TEXT("Interaction"));
	CustomizeComponent = CreateDefaultSubobject<UCCustomizeComponent>(TEXT("Customize"));
	BuildComponent = CreateDefaultSubobject<UCBuildComponent>(TEXT("Build"));
	MovingComponent = CreateDefaultSubobject<UCMovingComponent>(TEXT("Moving"));
	CustomizeComponent->SetIsReplicated(true);
	BuildComponent->SetIsReplicated(true);
	StatusComponent = CreateDefaultSubobject<UCStatusComponent>(TEXT("Status"));
	StatusComponent->SetIsReplicated(true);
	StateComponent = CreateDefaultSubobject<UCStateComponent>(TEXT("State"));
	StateComponent->SetIsReplicated(true);
	MontageComponent = CreateDefaultSubobject<UCMontageComponent>(TEXT("Montage"));
	MontageComponent->SetIsReplicated(true);
	//커스터마이즈 메쉬 세팅 
	Head = GetMesh();
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Accessory = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Accessory"));
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));

	

	//카메라 세팅 
	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = this->CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	//ReplicateComponent = this->CreateDefaultSubobject<UCReplicateComponent>(TEXT("Replicate"));

	//스켈레탈 메시
	USkeletalMesh* skeletalMesh = nullptr;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshFinder(TEXT("SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Character/Survivor/SK_Survivor.SK_Survivor'"));
	if (skeletalMeshFinder.Succeeded())
	{
		skeletalMesh = skeletalMeshFinder.Object;
		GetMesh()->SetSkeletalMesh(skeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("skeletalMeshFinder Failed - ACSurvivor"));
	}

	

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 450;

	SpringArm->SetRelativeLocation(FVector(0, 0, 100));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	
	static ConstructorHelpers::FClassFinder<UUserWidget> survivorNameClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/WBP_CSurvivorName.WBP_CSurvivorName_C'"));
	if (survivorNameClassFinder.Succeeded())
	{
		SurvivorNameClass = survivorNameClassFinder.Class;
	}
	else
	{
		CDebug::Print("survivorNameClassFinder Failed");
	}

	SurvivorNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SurvivorName"));
	SurvivorNameWidgetComponent->SetupAttachment(GetRootComponent());
	SurvivorNameWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 25.0f));
	SurvivorNameWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));
	SurvivorNameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	SurvivorNameWidgetComponent->SetWidgetClass(SurvivorNameClass);
	SurvivorNameWidgetComponent->InitWidget();

	MovingComponent->SetSpeed(ESpeedType::Run);

}

void ACSurvivor::BeginPlay()
{
	Super::BeginPlay();

	NetDriver = GetWorld()->GetNetDriver();

	Pants->SetMasterPoseComponent(GetMesh());
	Boots->SetMasterPoseComponent(GetMesh());
	Accessory->SetMasterPoseComponent(GetMesh());
	Body->SetMasterPoseComponent(GetMesh());
	Hands->SetMasterPoseComponent(GetMesh());

	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	FText tempName = gameInstance->GetLobbySurvivorName();
	FString stringName = tempName.ToString();
	CDebug::Print(stringName);

	if (HasAuthority())
	{
		PerformSetSurvivorName(gameInstance->GetLobbySurvivorName());
	}
	else
	{
		RequestSetSurvivorName(gameInstance->GetLobbySurvivorName());
	}

	FTimerHandle nameTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(nameTimerHandle, this, &ACSurvivor::SetSurvivorNameVisibility, 0.5f, true);
}

void ACSurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACSurvivor::DoAction()
{
	if (WeaponComponent->IsUnarmedMode()) return;
	WeaponComponent->DoAction();
}

void ACSurvivor::SubAction()
{

}

void ACSurvivor::HoldAxe()
{
	WeaponComponent->SetAxeMode();
}


void ACSurvivor::StartInteract()
{
	InteractionComponent->DoInteract();
}

void ACSurvivor::FinishInteract()
{
	InteractionComponent->FinishInteract();
}

void ACSurvivor::Build()
{
	BuildComponent->BuildSpawnedStructure();
}

void ACSurvivor::CancleBuild()
{
	if (IsValid(BuildComponent))
		BuildComponent->ClearSpawnedStructure();
}

void ACSurvivor::HandleMouseWheelUp()
{
	InteractionComponent->ExtraOptionButtonUp();
}

void ACSurvivor::HandleMouseWheelDown()
{
	InteractionComponent->ExtraOptionButtonDown();
}

void ACSurvivor::SelectStructure(ESelectedStructure InKey, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement, FName InItemID)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		BuildComponent->SelectQ(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::W:
	{
		BuildComponent->SelectW(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::E:
	{
		BuildComponent->SelectE(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::A:
	{
		BuildComponent->SelectA(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::S:
	{
		BuildComponent->SelectS(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::D:
	{
		BuildComponent->SelectD(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::Z:
	{
		BuildComponent->SelectZ(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::X:
	{
		BuildComponent->SelectX(InClass, InElement, InItemID);
		break;
	}

	case ESelectedStructure::C:
	{
		BuildComponent->SelectC(InClass, InElement, InItemID);
		break;
	}
	}
}

float ACSurvivor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
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

void ACSurvivor::BroadCastApplyHitData_Implementation(FDamageData InDamageData)
{
	if (!HasAuthority())
	{
		DamageData = InDamageData;
	}
	ApplyHitData();
}

void ACSurvivor::ApplyHitData()
{
	UDataTable* HitDataTable = nullptr;
	if (GameInstance)
	{
		HitDataTable = GameInstance->HitDataTable;
	}

	if (HitDataTable != nullptr)
	{
		FString HitActorName = FString("_Survivor");
		FName CompleteHitID = FName(*(DamageData.HitID.ToString()) + HitActorName);
			HitData = HitDataTable->FindRow<FHitData>(CompleteHitID, FString("Hit_Survivor"));
		if (HitData && HitData->Montage)
		{
			MontageComponent->Montage_Play(HitData->Montage, HitData->PlayRate);
			if (StatusComponent != nullptr && this->HasAuthority())
			{
				StatusComponent->ApplyDamage(HitData->DamageAmount);
			}
			HitData->PlaySoundWave(this);
			HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			HitData->PlayCameraShake(PlayerController, 1.0f);
		}
		

		if (!StatusComponent->IsDead())
		{
			FVector start = GetActorLocation();
			UObject* FoundObject = NetDriver->GuidCache->GetObjectFromNetGUID(DamageData.CharacterID, true);  
			AActor* targetActor = HitData->FindActorByNetGUID(DamageData.CharacterID, GetWorld());
			FVector target = targetActor->GetActorLocation();
			FVector direction = target - start;
			direction = direction.GetSafeNormal();
			LaunchCharacter(-direction*HitData->Launch, false, false);
		}
		if (StatusComponent->IsDead())
		{
			StateComponent->ChangeType(EStateType::Dead);
			Die();
			return;
		}

		DamageData.CharacterID = 0;
		DamageData.CauserID = 0;
		DamageData.HitID = "";

	}
	
}

void ACSurvivor::Die()
{
	GameInstance->WorldMap->GetPersonalSurvivorController()->SetInputMode(FInputModeUIOnly());
	//GetCharacterMovement()->DisableMovement();
	//GetCharacterMovement()->StopMovementImmediately();
	BroadcastDoSpecialAction(ESpecialState::Dead);
	//BroadcastDisableCollision();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACSurvivor::RemoveCharacter, 2.0f, false);
}

void ACSurvivor::RemoveCharacter()
{
	if (this->HasAuthority())
		BroadcastDestroySurvivor();
	else
		RequestDestroySurvivor();
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//PlayerController->UnPossess();

}

void ACSurvivor::BroadcastDoSpecialAction_Implementation(ESpecialState SpecialState)
{
	PerformDoSpecialAction(SpecialState);
}

void ACSurvivor::PerformDoSpecialAction(ESpecialState SpecialState)
{
	MontageComponent->Montage_Play(DoSpecialActionDatas[(int32)SpecialState].Montage, DoSpecialActionDatas[(int32)SpecialState].PlayRate);
}


void ACSurvivor::BroadcastDisableCollision_Implementation()
{
	//입력 무효화
	//DisableInput(GetWorld()->GetFirstPlayerController());
	GameInstance->WorldMap->GetPersonalSurvivorController()->SetInputMode(FInputModeUIOnly());
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false);
	// 모든 채널에 대해 충돌 무시
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ACSurvivor::SetSurvivorNameVisibility()
{
	// 게임 인스턴스 기반으로 고유한 서바이버 찾을때까지 반복

	if (!PersonalSurvivor)
	{
		UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
		if (gameInstance)
		{
			if (gameInstance->WorldMap && NetDriver && NetDriver->GuidCache)
			{
				if (NetDriver && NetDriver->GuidCache)
				{
					//UObject* foundObject = NetDriver->GuidCache->GetObjectFromNetGUID(gameInstance->WorldMap->GetPersonalGUID(), true);
					//if (foundObject)
					//{
					//	ACSurvivor* survivor = Cast<ACSurvivor>(foundObject);
					//	if (survivor)
					//		PersonalSurvivor = survivor;
					//}

					PersonalSurvivor = gameInstance->WorldMap->GetPersonalSurvivor();
				}
			}
		}
	}
	else
	{
		// 다른 서바이버 위치 검사
		for (TActorIterator<ACSurvivor> It(GetWorld()); It; ++It)
		{
			ACSurvivor* otherSurvivor = *It;

			// 자신이 아닌 다른 survivor에 대해서만 처리
			if (otherSurvivor && otherSurvivor != PersonalSurvivor)
			{
				float distance = FVector::Dist(PersonalSurvivor->GetActorLocation(), otherSurvivor->GetActorLocation());

				// 거리 조건을 만족할 때 가시성 변경이 한번만 호출됨
				if (distance > MaxDistanceForNameVisibility)
				{
					if (otherSurvivor->SurvivorNameWidgetComponent)
					{
						if (otherSurvivor->SurvivorNameWidgetComponent->IsVisible())
						{
							otherSurvivor->SurvivorNameWidgetComponent->SetVisibility(false);
						}
					}
				}
				else
				{
					if (otherSurvivor->SurvivorNameWidgetComponent)
					{
						if (!(otherSurvivor->SurvivorNameWidgetComponent->IsVisible()))
						{
							otherSurvivor->SurvivorNameWidgetComponent->SetVisibility(true);
						}
					}
				}
			}
		}
	}
}

void ACSurvivor::BroadcastDestroySurvivor_Implementation()
{
	this->Destroy();
}

void ACSurvivor::RequestDestroySurvivor_Implementation()
{
	BroadcastDestroySurvivor();
}

void ACSurvivor::PerformSetSurvivorName(const FText& InText)
{
	ReplicatedSurvivorName = InText; // OnRep_ReplicatedSurvivorName() 트리거 (변수가 바뀌었으므로)
	UpdateSurvivorNameWidget();
}

void ACSurvivor::RequestSetSurvivorName_Implementation(const FText& InText)
{
	PerformSetSurvivorName(InText);
}

bool ACSurvivor::RequestSetSurvivorName_Validate(const FText& InText)
{
	return true;
}

void ACSurvivor::UpdateSurvivorNameWidget()
{
	CDebug::Print("Update Called");
	if (SurvivorNameWidgetComponent)
	{
		if (SurvivorNameWidgetComponent->GetUserWidgetObject())
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(SurvivorNameWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("SurvivorName")));
			if (TextBlock)
			{
				TextBlock->SetText(ReplicatedSurvivorName);
				CDebug::Print("SetText Called");
			}
			else
			{
				CDebug::Print("TextBlock is not valid");
			}
		}
		else
		{
			CDebug::Print("UserWidgetObject is not valid");
		}
	}
	else
	{
		CDebug::Print("SurvivorNameWidgetComponent is not valid");
	}

}

void ACSurvivor::OnRep_ReplicatedSurvivorName()
{
	UpdateSurvivorNameWidget();
}

void ACSurvivor::RequestMessage_Implementation(const FText& InSurvivorNameText, const FText& InMessageText)
{
	BroadcastMessage(InSurvivorNameText, InMessageText);
}

void ACSurvivor::BroadcastMessage_Implementation(const FText& InSurvivorNameText, const FText& InMessageText)
{
	PerformAddMessage(InSurvivorNameText, InMessageText);
}

void ACSurvivor::ReceiveMessage(const FText& InSurvivorNameText, const FText& InMessageText)
{
	if (HasAuthority())
		BroadcastMessage(InSurvivorNameText, InMessageText);
	else
		RequestMessage(InSurvivorNameText, InMessageText);
}

void ACSurvivor::PerformAddMessage(const FText& InSurvivorNameText, const FText& InMessageText)
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		CDebug::Print("CGameInstance is Valid", gameInstance);
		if (gameInstance->ChattingBox)
		{
			CDebug::Print(TEXT("gameInstance->ChattingBox is Valid"), gameInstance->ChattingBox);
			gameInstance->ChattingBox->AddMessageToMessageBox(InSurvivorNameText, InMessageText);
		}
		else
			CDebug::Print(TEXT("gameInstance->ChattingBox is is Not Valid"), FColor::Red);
	}
	else
		CDebug::Print(("CGameInstance is Not Valid"), FColor::Red);
}

void ACSurvivor::BroadcastSetName_Implementation(const FText& InText, uint32 NetGUIDValue)
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		gameInstance->WorldMap->CreateSurvivorLocationOnWorldMap(InText, NetGUIDValue);
	}
	else
	{
		CDebug::Print("gameInstance is not valid");
	}
}

void ACSurvivor::RequestSetName_Implementation(const FText& InText, uint32 NetGUIDValue)
{
	BroadcastSetName(InText, NetGUIDValue);
}

void ACSurvivor::BroadcastSetLocation_Implementation(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue)
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance)
	{
		gameInstance->WorldMap->RefreshSurvivorLocationOnWorldMap(LocationX, LocationY, RotationZ, NetGUIDValue);
	}
	else
	{
		CDebug::Print("gameInstance is not valid");
	}
}

void ACSurvivor::RequestSetLocation_Implementation(float LocationX, float LocationY, float RotationZ, uint32 NetGUIDValue)
{
	BroadcastSetLocation(LocationX, LocationY, RotationZ, NetGUIDValue);
}

void ACSurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACSurvivor, ReplicatedSurvivorName);
}