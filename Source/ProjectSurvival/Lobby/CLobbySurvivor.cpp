#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CSurvivorName.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Lobby/CLobbyGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"

ACLobbySurvivor::ACLobbySurvivor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Accessory = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Accessory"));
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//class UDataTable* CustomizePantsData;
	//class UDataTable* CustomizeSingleData;

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeHeadDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizeHead.DT_CustomizeHead'"));
	if (customizeHeadDataFinder.Succeeded())
	{
		CustomizeHeadData = customizeHeadDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeHeadDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizePantsDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizePants.DT_CustomizePants'"));
	if (customizePantsDataFinder.Succeeded())
	{
		CustomizePantsData = customizePantsDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizePantsDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeBootsDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizeBoots.DT_CustomizeBoots'"));
	if (customizeBootsDataFinder.Succeeded())
	{
		CustomizeBootsData = customizeBootsDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeBootsDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeSingleDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizeSingle.DT_CustomizeSingle'"));
	if (customizeSingleDataFinder.Succeeded())
	{
		CustomizeSingleData = customizeSingleDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeSingleDataFinder Failed");
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> survivorNameClassFinder(TEXT("WidgetBlueprint'/Game/PirateIsland/Include/Blueprints/Widget/WBP_CSurvivorName.WBP_CSurvivorName_C'"));
	if (survivorNameClassFinder.Succeeded())
	{
		SurvivorNameClass = survivorNameClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("survivorNameClassFinder Failed - ACLobbySurvivorController"));
	}

	SurvivorNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SurvivorName"));
	SurvivorNameWidgetComponent->SetupAttachment(GetRootComponent());
	SurvivorNameWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	SurvivorNameWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));
	SurvivorNameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	SurvivorNameWidgetComponent->SetWidgetClass(SurvivorNameClass);
	SurvivorNameWidgetComponent->InitWidget();

	USkeletalMesh* skeletalHeadMesh = nullptr;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalHeadMeshFinder(TEXT("SkeletalMesh'/Game/PirateIsland/Include/Skeletal/Character/Survivor/SK_Survivor_Hair_01.SK_Survivor_Hair_01'"));
	if (skeletalHeadMeshFinder.Succeeded())
	{
		
		skeletalHeadMesh = skeletalHeadMeshFinder.Object;
		GetMesh()->SetSkeletalMesh(skeletalHeadMesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	else
	{
		CDebug::Log("skeletalHeadMeshFinder is not valid.");
	}

	//UPROPERTY()
	//	class USkeletalMeshComponent* AccessoryMesh;
	//UPROPERTY()
	//	class USkeletalMeshComponent* BodyMesh;
	//UPROPERTY()
	//	class USkeletalMeshComponent* HandMesh;
	//UPROPERTY()
	//	class USkeletalMeshComponent* PantsMesh;
	//UPROPERTY()
	//	class USkeletalMeshComponent* BootsMesh;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 450;

	SpringArm->TargetArmLength = -400;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bDoCollisionTest = false;
	Camera->SetRelativeRotation(FRotator(0, 180, 0));
	Camera->AddRelativeLocation(FVector(-100, -50, 0));
}

void ACLobbySurvivor::BeginPlay()
{
	Super::BeginPlay();

	InitCustomize();
}

void ACLobbySurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACLobbySurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACLobbySurvivor::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACLobbySurvivor::OnMoveRight);
}

void ACLobbySurvivor::InitCustomize()
{
	FSkeletalHeadMeshRow* headMeshRow = CustomizeHeadData->FindRow<FSkeletalHeadMeshRow>(FName("Head_01"), TEXT("HeadMeshRowFind"));
	USkeletalMesh* headMesh = headMeshRow->HeadMesh;
	GetMesh()->SetSkeletalMesh(headMesh);

	FSkeletalPantsMeshRow* pantsMeshRow = CustomizePantsData->FindRow<FSkeletalPantsMeshRow>(FName("Pants_01"), TEXT("PantsMeshRowFind"));
	USkeletalMesh* pantsMesh = pantsMeshRow->PantsMesh;
	Pants->SetSkeletalMesh(pantsMesh);
	Pants->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	FSkeletalBootsMeshRow* bootsMeshRow = CustomizeBootsData->FindRow<FSkeletalBootsMeshRow>(FName("Boots_01"), TEXT("BootsMeshRowFind"));
	USkeletalMesh* bootsMesh = bootsMeshRow->BootsMesh;
	Boots->SetSkeletalMesh(bootsMesh);
	Boots->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	FSkeletalSingleMeshRow* singleMeshRow = CustomizeSingleData->FindRow<FSkeletalSingleMeshRow>(FName("Single"), TEXT("SingleMeshRowFind"));
	USkeletalMesh* accessoryMesh = singleMeshRow->AccessoryMesh;
	Accessory->SetSkeletalMesh(accessoryMesh);
	Accessory->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	USkeletalMesh* bodyMesh = singleMeshRow->BodyMesh;
	Body->SetSkeletalMesh(bodyMesh);
	Body->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	USkeletalMesh* handsMesh = singleMeshRow->HandsMesh;
	Hands->SetSkeletalMesh(handsMesh);
	Hands->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	Pants->SetMasterPoseComponent(GetMesh());
	Boots->SetMasterPoseComponent(GetMesh());
	Accessory->SetMasterPoseComponent(GetMesh());
	Body->SetMasterPoseComponent(GetMesh());
	Hands->SetMasterPoseComponent(GetMesh());
}

void ACLobbySurvivor::SetLocalValue()
{
	Camera->AddRelativeLocation(FVector(100, 50, 50)); // 클라이언트가 로컬로만 사용하는 속성
}

void ACLobbySurvivor::SetSurvivorName(const FText& InText)
{
	if (HasAuthority())
	{
		PerformSetSurvivorName(InText);
	}
	else
	{
		RequestSetSurvivorName(InText);
	}
}

void ACLobbySurvivor::PerformSetSurvivorName(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerSetSurvivorName_Implementation Called"));
	ReplicatedSurvivorName = InText; // OnRep_ReplicatedSurvivorName() 트리거 (변수가 바뀌었으므로)
	
	int32 randomX = FMath::RandRange(10793, 11281);
	int32 randomY = FMath::RandRange(-26617, -25406);
	SetActorLocation(FVector(randomX, randomY, 557));
	
	// 위젯 업데이트
	// 서버에서 PerformSetSurvivorName()호출 시 서버의 이름이 업데이트 된다. (서버의 입장에서 서버 이름 설정)
	// 클라에서 RequestSetSurvivorName() (함수내용 PerformSetSurvivorName()) 호출 시 클라이언트의 이름이 업데이트 된다. (서버의 입장에서 클라 이름 설정)
	UpdateSurvivorNameWidget();
}

// 클라이언트에서 RequestSetSurvivorName() 로 호출, 서버에서 내용이 실행됨
void ACLobbySurvivor::RequestSetSurvivorName_Implementation(const FText& InText)
{
	PerformSetSurvivorName(InText);
}

bool ACLobbySurvivor::RequestSetSurvivorName_Validate(const FText& InText)
{
	return true;
}

void ACLobbySurvivor::UpdateSurvivorNameWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateSurvivorNameWidget Called"));
	if (SurvivorNameWidgetComponent)
	{
		if (SurvivorNameWidgetComponent->GetUserWidgetObject())
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(SurvivorNameWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("SurvivorName")));
			if (TextBlock)
			{
				TextBlock->SetText(ReplicatedSurvivorName);
				UE_LOG(LogTemp, Warning, TEXT("SurvivorName updated to: %s"), *ReplicatedSurvivorName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TextBlock FAIL"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SurvivorNameWidgetComponent->GetUserWidgetObject() FAIL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SurvivorNameWidgetComponent FAIL"));
	}
}

void ACLobbySurvivor::RequestReady_Implementation()
{
	ACLobbyGameMode* lobbyGameMode = Cast<ACLobbyGameMode>(GetWorld()->GetAuthGameMode());
	lobbyGameMode->ReadyPlayer();
}

bool ACLobbySurvivor::RequestReady_Validate()
{
	return true;
}

void ACLobbySurvivor::BroadcastSetText_Implementation(int InIndex)
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(lobbySurvivorController))
	{
		lobbySurvivorController->UpdateDifficultyWidget(InIndex);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("lobbySurvivorController is not valid - ACLobbySurvivor"));
	}
}

void ACLobbySurvivor::OnMoveForward(float InAxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();
	AddMovementInput(direction, -InAxisValue);
}

void ACLobbySurvivor::OnMoveRight(float InAxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();
	AddMovementInput(direction, -InAxisValue);
}

// 이 함수는 클라이언트가 서버로부터 변수 변경 사항을 수신한 후 호출되므로, 약간의 지연이 있을 수 있다.
void ACLobbySurvivor::OnRep_ReplicatedSurvivorName()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_ReplicatedSurvivorName Called"));

	UpdateSurvivorNameWidget(); // 리플리케이트 완료되었으니 클라 입장에서 서버의 값으로 위젯을 모두 업데이트 한다. 이 부분을 주석처리하면 클라 입장에선 모두 텍스트 기본값으로 보임
}

void ACLobbySurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedSurvivorName);
}
