#include "CSurvivor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "DrawDebugHelpers.h"
#include "Utility/CDebug.h"

ACSurvivor::ACSurvivor()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = this->CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Skeletal Mesh
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

	
}

void ACSurvivor::BeginPlay()
{
	Super::BeginPlay();
	//SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
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
	//PlayerInputComponent->BindAction("Slash", this, &ACSurvivor::OnVerticalLook);

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

void ACSurvivor::SlashTree()
{
	int myint = 32;
	float myfloat = 10.0f;
	bool mybool = true;
	FVector myVector = GetActorLocation();
	FRotator myRotator = GetActorRotation();
	FQuat myQuat = GetActorQuat();
	UObject* myObject = GetWorld()->GetFirstPlayerController();
	UClass* myClass = GetWorld()->GetFirstPlayerController()->StaticClass();
	
	// 디버그 테스트 시작 //
	LogLine;
	PrintLine;
	PrintLine_Detail(FColor::Green, 5);

	CDebug::Log(myint);
	CDebug::Log(myint, "Test int");
	CDebug::Log("Test int", myint);
	CDebug::Print(myint);
	CDebug::Print(myint, "Test int");
	CDebug::Print("Test int", myint);

	CDebug::Log(myfloat);
	CDebug::Log(myfloat, "Test float");
	CDebug::Log("Test float", myfloat);
	CDebug::Print(myfloat);
	CDebug::Print(myfloat, "Test float");
	CDebug::Print("Test float", myfloat);

	CDebug::Log(mybool);
	CDebug::Log(mybool, "Test bool");
	CDebug::Log("Test bool", mybool);
	CDebug::Print(mybool);
	CDebug::Print(mybool, "Test bool");
	CDebug::Print("Test bool", mybool);

	CDebug::Log(myVector);
	CDebug::Log(myVector, "Test Vector");
	CDebug::Log("Test Vector", myVector);
	CDebug::Print(myVector);
	CDebug::Print(myVector, "Test Vector");
	CDebug::Print("Test Vector", myVector);

	CDebug::Log(myRotator);
	CDebug::Log(myRotator, "Test Rotator");
	CDebug::Log("Test Rotator", myRotator);
	CDebug::Print(myRotator);
	CDebug::Print(myRotator, "Test Rotator");
	CDebug::Print("Test Rotator", myRotator);

	CDebug::Log(myQuat);
	CDebug::Log(myQuat, "Test Quat");
	CDebug::Log("Test Quat", myQuat);
	CDebug::Print(myQuat);
	CDebug::Print(myQuat, "Test Quat");
	CDebug::Print("Test Quat", myQuat);

	CDebug::Log(myObject);
	CDebug::Log(myObject, "Test Object");
	CDebug::Log("Test Object", myObject);
	CDebug::Print(myObject);
	CDebug::Print(myObject, "Test Object");
	CDebug::Print("Test Object", myObject);

	CDebug::Log(myClass);
	CDebug::Log(myClass, "Test Class");
	CDebug::Log("Test Class", myClass);
	CDebug::Print(myClass);
	CDebug::Print(myClass, "Test Class");
	CDebug::Print("Test Class", myClass);
	// 디버그 테스트 끝//

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Character Slashing !"));
	
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndLocation = StartLocation + (ForwardVector * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()-> LineTraceSingleByChannel (
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
			);

	if (bHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		//if (HitComponent);
	}
	

}


