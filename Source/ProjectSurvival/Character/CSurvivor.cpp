
#include "CSurvivor.h"
#include "ActorComponents/Disposable/CCustomizeComponent.h"
#include "ActorComponents/CWeaponComponent.h"
#include "ActorComponents/CHarvestComponent.h"
#include "ActorComponents/CBuildComponent.h"
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


ACSurvivor::ACSurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;


	//컴포넌트 세팅
	WeaponComponent = CreateDefaultSubobject<UCWeaponComponent>(TEXT("Weapon"));
	HarvestComponent = CreateDefaultSubobject<UCHarvestComponent>(TEXT("Harvest"));
	CustomizeComponent = CreateDefaultSubobject<UCCustomizeComponent>(TEXT("Customize"));
	BuildComponent = CreateDefaultSubobject<UCBuildComponent>("BuildComponent");
	CustomizeComponent->SetIsReplicated(true);



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
	//CDebug::Print(gameInstance->GetLobbySurvivorName().ToString());

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

	PlayerInputComponent->BindAxis("MoveForward", this, &ACSurvivor::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACSurvivor::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACSurvivor::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACSurvivor::OnVerticalLook);
}

void ACSurvivor::OnMoveForward(float InAxisValue)
{
	FRotator rotator = FRotator(0, this->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	this->AddMovementInput(direction, InAxisValue);
}

void ACSurvivor::OnMoveRight(float InAxisValue)
{
	FRotator rotator = FRotator(0, this->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	this->AddMovementInput(direction, InAxisValue);
}

void ACSurvivor::OnHorizontalLook(float InAxisValue)
{
	this->AddControllerYawInput(InAxisValue * 0.75f);
}

void ACSurvivor::OnVerticalLook(float InAxisValue)
{
	this->AddControllerPitchInput(InAxisValue * 0.75f);
}

void ACSurvivor::HoldAxe()
{
	WeaponComponent->SetAxeMode();
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
		BuildComponent->SelectW();
		break;
	}

	case ESelectedStructure::E:
	{
		BuildComponent->SelectE();
		break;
	}

	case ESelectedStructure::A:
	{
		BuildComponent->SelectA();
		break;
	}

	case ESelectedStructure::S:
	{
		BuildComponent->SelectS();
		break;
	}

	case ESelectedStructure::D:
	{
		BuildComponent->SelectD();
		break;
	}

	case ESelectedStructure::Z:
	{
		BuildComponent->SelectZ();
		break;
	}

	case ESelectedStructure::X:
	{
		BuildComponent->SelectX();
		break;
	}

	case ESelectedStructure::C:
	{
		BuildComponent->SelectC();
		break;
	}
	}
}

void ACSurvivor::DoAction()
{
	if (WeaponComponent->IsUnarmedMode()) return;
		WeaponComponent->DoAction();
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

void ACSurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACSurvivor, ReplicatedSurvivorName);
}



//int myint = 32;
	//float myfloat = 10.0f;
	//bool mybool = true;
	//FVector myVector = GetActorLocation();
	//FRotator myRotator = GetActorRotation();
	//FQuat myQuat = GetActorQuat();
	//UObject* myObject = GetWorld()->GetFirstPlayerController();
	//UClass* myClass = GetWorld()->GetFirstPlayerController()->StaticClass();

	//// 디버그 테스트 시작 //
	//LogLine;
	//PrintLine;
	//PrintLine_Detail(FColor::Green, 5);

	//CDebug::Log(myint);
	//CDebug::Log(myint, "Test int");
	//CDebug::Log("Test int", myint);
	//CDebug::Print(myint);
	//CDebug::Print(myint, "Test int");
	//CDebug::Print("Test int", myint);

	//CDebug::Log(myfloat);
	//CDebug::Log(myfloat, "Test float");
	//CDebug::Log("Test float", myfloat);
	//CDebug::Print(myfloat);
	//CDebug::Print(myfloat, "Test float");
	//CDebug::Print("Test float", myfloat);

	////CDebug::Log(mybool); 쓰지않음
	//CDebug::Log(mybool, "Test bool");
	//CDebug::Log("Test bool", mybool);
	////CDebug::Print(mybool); 쓰지않음
	//CDebug::Print(mybool, "Test bool");
	//CDebug::Print("Test bool", mybool);

	//CDebug::Log(myVector);
	//CDebug::Log(myVector, "Test Vector");
	//CDebug::Log("Test Vector", myVector);
	//CDebug::Print(myVector);
	//CDebug::Print(myVector, "Test Vector");
	//CDebug::Print("Test Vector", myVector);

	//CDebug::Log(myRotator);
	//CDebug::Log(myRotator, "Test Rotator");
	//CDebug::Log("Test Rotator", myRotator);
	//CDebug::Print(myRotator);
	//CDebug::Print(myRotator, "Test Rotator");
	//CDebug::Print("Test Rotator", myRotator);

	//CDebug::Log(myQuat);
	//CDebug::Log(myQuat, "Test Quat");
	//CDebug::Log("Test Quat", myQuat);
	//CDebug::Print(myQuat);
	//CDebug::Print(myQuat, "Test Quat");
	//CDebug::Print("Test Quat", myQuat);

	//CDebug::Log(myObject);
	//CDebug::Log(myObject, "Test Object");
	//CDebug::Log("Test Object", myObject);
	//CDebug::Print(myObject);
	//CDebug::Print(myObject, "Test Object");
	//CDebug::Print("Test Object", myObject);

	//CDebug::Log(myClass);
	//CDebug::Log(myClass, "Test Class");
	//CDebug::Log("Test Class", myClass);
	//CDebug::Print(myClass);
	//CDebug::Print(myClass, "Test Class");
	//CDebug::Print("Test Class", myClass);
	//// 디버그 테스트 끝//