#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CSurvivorName.h"
#include "Lobby/CLobbySurvivorController.h"
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
#include "EngineUtils.h"

ACLobbySurvivor::ACLobbySurvivor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

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
		UE_LOG(LogTemp, Warning, TEXT("skeletalMeshFinder Failed - ACLobbySurvivor"));
	}

//	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstanceFinder(TEXT("AnimBlueprint'/Game/PirateIsland/Include/Animation/AnimationBlueprint/ABP_CSurvivor.ABP_CSurvivor_C'"));
//	if (animInstanceFinder.Succeeded())
//	{
//		GetMesh()->SetAnimClass(animInstanceFinder.Class);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("animInstanceFinder Failed - ACSurvivor"));
//	}

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
}

void ACLobbySurvivor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWidgetVisibility();
}

void ACLobbySurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACLobbySurvivor::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACLobbySurvivor::OnMoveRight);
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
	ReplicatedSurvivorName = InText; // OnRep_ReplicatedSurvivorName() Ʈ����
	
	int32 randomX = FMath::RandRange(10793, 11281);
	int32 randomY = FMath::RandRange(-26617, -25406);
	SetActorLocation(FVector(randomX, randomY, 557));
	
	// ���� ������Ʈ
	// �������� PerformSetSurvivorName()ȣ�� �� ������ �̸��� ������Ʈ �ȴ�. (������ ���忡�� ���� �̸� ����)
	// Ŭ�󿡼� RequestSetSurvivorName() (�Լ����� PerformSetSurvivorName()) ȣ�� �� Ŭ���̾�Ʈ�� �̸��� ������Ʈ �ȴ�. (������ ���忡�� Ŭ�� �̸� ����)
	UpdateSurvivorNameWidget();
}

bool ACLobbySurvivor::RequestSetSurvivorName_Validate(const FText& InText)
{
	return true;
}

// Ŭ���̾�Ʈ���� RequestSetSurvivorName() �� ȣ��, �������� ������ �����
void ACLobbySurvivor::RequestSetSurvivorName_Implementation(const FText& InText)
{
	PerformSetSurvivorName(InText);
}

void ACLobbySurvivor::SetLocalValue()
{
	Camera->AddRelativeLocation(FVector(100, 50, 50)); // Ŭ���̾�Ʈ�� ���÷θ� ����ϴ� �Ӽ�
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

// �� �Լ��� Ŭ���̾�Ʈ�� �����κ��� ���� ���� ������ ������ �� ȣ��ǹǷ�, �ణ�� ������ ���� �� �ִ�.
void ACLobbySurvivor::OnRep_ReplicatedSurvivorName()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_ReplicatedSurvivorName Called"));
	
	UpdateSurvivorNameWidget(); // ���ø�����Ʈ �Ϸ�Ǿ����� Ŭ�� ���忡�� ������ ������ ������ ��� ������Ʈ �Ѵ�. �� �κ��� �ּ�ó���ϸ� Ŭ�� ���忡�� ��� �ؽ�Ʈ �⺻������ ����
}

void ACLobbySurvivor::UpdateWidgetVisibility()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());

	if (Distance > HideDistance)
	{
		SurvivorNameWidgetComponent->SetVisibility(false);
	}
	else
	{
		SurvivorNameWidgetComponent->SetVisibility(true);
	}
}

void ACLobbySurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedSurvivorName);
}
