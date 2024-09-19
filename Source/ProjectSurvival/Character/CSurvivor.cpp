
#include "CSurvivor.h"
#include "ActorComponents/Disposable/CCustomizeComponent.h"
#include "ActorComponents/CWeaponComponent.h"
#include "ActorComponents/CHarvestComponent.h"
#include "ActorComponents/CBuildComponent.h"
#include "ActorComponents/CMovingComponent.h"
#include "ActorComponents/CInteractionComponent.h"
#include "ActorComponents/CInventoryComponent.h"
#include "ActorComponents/CStatusComponent.h"
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
#include "Net/UnrealNetwork.h"
#include "CGameInstance.h"
#include "Utility/CDebug.h"
#include "Widget/CMainHUD.h"
#include "Widget/Chatting/CChattingBox.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CSurvivorController.h"
#include "GameFramework/PlayerState.h"


ACSurvivor::ACSurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	//인터렉션 세팅
	BaseEyeHeight = 67.0f; //Pawn의 Default 눈 높이 세팅



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

	//커스터마이즈 메쉬 세팅 
	Head = GetMesh();
	Head->SetIsReplicated(true);
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Pants->SetIsReplicated(true);
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Boots->SetIsReplicated(true);
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

	// AnimInstance
	//static ConstructorHelpers::FClassFinder<UAnimInstance> animInstanceFinder(TEXT("AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/ABP_CSurvivor.ABP_CSurvivor_C'"));
	//if (animInstanceFinder.Succeeded())
	//{
	//	GetMesh()->SetAnimClass(animInstanceFinder.Class);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("animInstanceFinder Failed - ACSurvivor"));
	//}

	bUseControllerRotationYaw = true;
	// true일 경우 컨트롤러의 회전 방향으로 캐릭터가 회전한다.
	// false일 경우 캐릭터의 이동 방향으로 캐릭터가 회전한다.
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

void ACSurvivor::SelectStructure(ESelectedStructure InKey, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		BuildComponent->SelectQ(InClass, InElement);
		break;
	}

	case ESelectedStructure::W:
	{
		BuildComponent->SelectW(InClass, InElement);
		break;
	}

	case ESelectedStructure::E:
	{
		BuildComponent->SelectE(InClass, InElement);
		break;
	}

	case ESelectedStructure::A:
	{
		BuildComponent->SelectA(InClass, InElement);
		break;
	}

	case ESelectedStructure::S:
	{
		BuildComponent->SelectS(InClass, InElement);
		break;
	}

	case ESelectedStructure::D:
	{
		BuildComponent->SelectD(InClass, InElement);
		break;
	}

	case ESelectedStructure::Z:
	{
		BuildComponent->SelectZ(InClass, InElement);
		break;
	}

	case ESelectedStructure::X:
	{
		BuildComponent->SelectX(InClass, InElement);
		break;
	}

	case ESelectedStructure::C:
	{
		BuildComponent->SelectC(InClass, InElement);
		break;
	}
	}
}

void ACSurvivor::Damage(ACharacter* Attacker, AActor* Causer, FHitData HitData)
{

	if (StatusComponent != nullptr)
	{
		StatusComponent->TakeDamage(HitData.Power);
	}

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

void ACSurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACSurvivor, ReplicatedSurvivorName);
}