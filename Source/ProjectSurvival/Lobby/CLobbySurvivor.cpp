#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CLobbySurvivorController.h"
#include "Lobby/CSurvivorName.h"
#include "Lobby/CLobbyGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"

ACLobbySurvivor::ACLobbySurvivor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Head = GetMesh();
	Head->SetIsReplicated(true);
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	Pants->SetIsReplicated(true);
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Boots->SetIsReplicated(true);
	Accessory = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Accessory"));
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeHeadDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Customize/DT_CustomizeHead.DT_CustomizeHead'"));
	if (customizeHeadDataFinder.Succeeded())
	{
		CustomizeHeadData = customizeHeadDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeHeadDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeHeadColorDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Customize/DT_CustomizeHeadColor.DT_CustomizeHeadColor'"));
	if (customizeHeadColorDataFinder.Succeeded())
	{
		CustomizeHeadColorData = customizeHeadColorDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeHeadColorDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizePantsDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Customize/DT_CustomizePants.DT_CustomizePants'"));
	if (customizePantsDataFinder.Succeeded())
	{
		CustomizePantsData = customizePantsDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizePantsDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeBootsDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Customize/DT_CustomizeBoots.DT_CustomizeBoots'"));
	if (customizeBootsDataFinder.Succeeded())
	{
		CustomizeBootsData = customizeBootsDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeBootsDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeSkinColorDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Customize/DT_CustomizeSkinColor.DT_CustomizeSkinColor'"));
	if (customizeSkinColorDataFinder.Succeeded())
	{
		CustomizeSkinColorData = customizeSkinColorDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeSkinColorDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeSingleDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/Customize/DT_CustomizeSingle.DT_CustomizeSingle'"));
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
		CDebug::Log("skeletalHeadMeshFinder is not valid.");
	}

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
	if (!HasAuthority())
	{
		if (!ReplicatedHeadName.IsNone())
			UpdateSkeletalHeadMesh();
		if (!ReplicatedHeadColorName.IsNone())
			UpdateSkeletalHeadMeshColor();
		if (!ReplicatedPantsName.IsNone())
			UpdateSkeletalPantsMesh();
		if (!ReplicatedBootsName.IsNone())
			UpdateSkeletalBootsMesh();
		if (!ReplicatedSkinColorName.IsNone())
			UpdateSkinColor();

		RequestDifficultyUpdate();
	}
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
	Head->SetSkeletalMesh(headMesh);

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

	TArray<FName> hairRowNames = CustomizeHeadData->GetRowNames();
	ValidHairRowNumber = hairRowNames.Num();
	TArray<FName> hairColorRowNames = CustomizeHeadColorData->GetRowNames();
	ValidHairColorRowNumber = hairColorRowNames.Num();
	TArray<FName> pantsRowNames = CustomizePantsData->GetRowNames();
	ValidPantsRowNumber = pantsRowNames.Num();
	TArray<FName> bootsRowNames = CustomizeBootsData->GetRowNames();
	ValidBootsRowNumber = bootsRowNames.Num();
	TArray<FName> skinColorRowNames = CustomizeSkinColorData->GetRowNames();
	ValidSkinColorRowNumber = skinColorRowNames.Num();

	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	if (lobbySurvivorController)
	{
		lobbySurvivorController->GetCustomizeInfo();
	}
	else
		CDebug::Print("Failed to Cast Controller");
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
	if (SurvivorNameWidgetComponent)
	{
		if (SurvivorNameWidgetComponent->GetUserWidgetObject())
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(SurvivorNameWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("SurvivorName")));
			if (TextBlock)
			{
				TextBlock->SetText(ReplicatedSurvivorName);
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

void ACLobbySurvivor::SetHeadMesh(int32 InIndex)
{
	if (HasAuthority())
	{
		PerformSetHeadMesh(InIndex);
	}
	else
	{
		RequestSetHeadMesh(InIndex);
	}
}

void ACLobbySurvivor::SetHeadMeshColor(int32 InIndex)
{
	if (HasAuthority())
	{
		PerformSetHeadMeshColor(InIndex);
	}
	else
	{
		RequestSetHeadMeshColor(InIndex);
	}
}

void ACLobbySurvivor::SetPantsMesh(int32 InIndex)
{
	if (HasAuthority())
	{
		PerformSetPantsMesh(InIndex);
	}
	else
	{
		RequestSetPantsMesh(InIndex);
	}
}

void ACLobbySurvivor::SetBootsMesh(int32 InIndex)
{
	if (HasAuthority())
	{
		PerformSetBootsMesh(InIndex);
	}
	else
	{
		RequestSetBootsMesh(InIndex);
	}
}

void ACLobbySurvivor::SetSkinColor(int32 InIndex)
{
	if (HasAuthority())
	{
		PerformSetSkinColor(InIndex);
	}
	else
	{
		RequestSetSkinColor(InIndex);
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

TArray<FName> ACLobbySurvivor::GetCustomizeRowNames()
{
	if (ReplicatedHeadName.IsNone())
		CustomizeRowNames.Add("Head_01");
	else
		CustomizeRowNames.Add(ReplicatedHeadName);

	if (ReplicatedHeadColorName.IsNone())
		CustomizeRowNames.Add("HeadColor_01");
	else
		CustomizeRowNames.Add(ReplicatedHeadColorName);

	if (ReplicatedPantsName.IsNone())
		CustomizeRowNames.Add("Pants_01");
	else
		CustomizeRowNames.Add(ReplicatedPantsName);

	if (ReplicatedBootsName.IsNone())
		CustomizeRowNames.Add("Boots_01");
	else
		CustomizeRowNames.Add(ReplicatedBootsName);

	if (ReplicatedSkinColorName.IsNone())
		CustomizeRowNames.Add("SkinColor_01");
	else
		CustomizeRowNames.Add(ReplicatedSkinColorName);

	return CustomizeRowNames;
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

void ACLobbySurvivor::PerformSetHeadMesh(int32 InIndex)
{
	FString indexString = FString::Printf(TEXT("%02d"), InIndex);
	FString headString = "Head_";
	FString combinedString = headString.Append(indexString);
	FName index(*combinedString);
	ReplicatedHeadName = index;
	FSkeletalHeadMeshRow* headMeshRow = CustomizeHeadData->FindRow<FSkeletalHeadMeshRow>(ReplicatedHeadName, TEXT("HeadMeshRowFind"));
	Head->SetSkeletalMesh(headMeshRow->HeadMesh);
}

void ACLobbySurvivor::RequestSetHeadMesh_Implementation(int32 InIndex)
{
	PerformSetHeadMesh(InIndex);
}

bool ACLobbySurvivor::RequestSetHeadMesh_Validate(int32 InIndex)
{
	return true;
}

void ACLobbySurvivor::UpdateSkeletalHeadMesh()
{
	FSkeletalHeadMeshRow* headMeshRow = CustomizeHeadData->FindRow<FSkeletalHeadMeshRow>(ReplicatedHeadName, TEXT("HeadMeshRowFind"));
	if (headMeshRow)
		Head->SetSkeletalMesh(headMeshRow->HeadMesh);
}

void ACLobbySurvivor::OnRep_ReplicatedHeadName()
{
	UpdateSkeletalHeadMesh();
}

void ACLobbySurvivor::PerformSetHeadMeshColor(int32 InIndex)
{
	FString indexString = FString::Printf(TEXT("%02d"), InIndex);
	FString headColorString = "HeadColor_";
	FString combinedString = headColorString.Append(indexString);
	FName index(*combinedString);
	ReplicatedHeadColorName = index;
	FSkeletalHeadMeshColorRow* headMeshColorRow = CustomizeHeadColorData->FindRow<FSkeletalHeadMeshColorRow>(ReplicatedHeadColorName, TEXT("HeadMeshColorRowFind"));
	MeshColorMaterial = Head->GetMaterial(0);
	if (IsValid(DynamicHeadMeshColorMaterial))
	{
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Hair Tint", headMeshColorRow->HeadMeshColor);
		Head->SetMaterial(0, DynamicHeadMeshColorMaterial);
	}
	else
	{
		DynamicHeadMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Hair Tint", headMeshColorRow->HeadMeshColor);
		Head->SetMaterial(0, DynamicHeadMeshColorMaterial);
	}
}

void ACLobbySurvivor::RequestSetHeadMeshColor_Implementation(int32 InIndex)
{
	PerformSetHeadMeshColor(InIndex);
}

bool ACLobbySurvivor::RequestSetHeadMeshColor_Validate(int32 InIndex)
{
	return true;
}

void ACLobbySurvivor::UpdateSkeletalHeadMeshColor()
{
	FSkeletalHeadMeshColorRow* headMeshColorRow = CustomizeHeadColorData->FindRow<FSkeletalHeadMeshColorRow>(ReplicatedHeadColorName, TEXT("HeadMeshColorRowFind"));
	
	if (IsValid(DynamicHeadMeshColorMaterial))
	{
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Hair Tint", headMeshColorRow->HeadMeshColor);
	}
	else
	{
		MeshColorMaterial = Head->GetMaterial(0);
		DynamicHeadMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Hair Tint", headMeshColorRow->HeadMeshColor);
		Head->SetMaterial(0, DynamicHeadMeshColorMaterial);
	}

}

void ACLobbySurvivor::OnRep_ReplicatedHeadColorName()
{
	UpdateSkeletalHeadMeshColor();
}

void ACLobbySurvivor::PerformSetPantsMesh(int32 InIndex)
{
	FString indexString = FString::Printf(TEXT("%02d"), InIndex);
	FString headString = "Pants_";
	FString combinedString = headString.Append(indexString);
	FName index(*combinedString);
	ReplicatedPantsName = index;
	FSkeletalPantsMeshRow* pantsMeshRow = CustomizePantsData->FindRow<FSkeletalPantsMeshRow>(ReplicatedPantsName, TEXT("PantsMeshRowFind"));
	Pants->SetSkeletalMesh(pantsMeshRow->PantsMesh);
}

void ACLobbySurvivor::RequestSetPantsMesh_Implementation(int32 InIndex)
{
	PerformSetPantsMesh(InIndex);
}

bool ACLobbySurvivor::RequestSetPantsMesh_Validate(int32 InIndex)
{
	return true;
}

void ACLobbySurvivor::UpdateSkeletalPantsMesh()
{
	FSkeletalPantsMeshRow* pantsMeshRow = CustomizePantsData->FindRow<FSkeletalPantsMeshRow>(ReplicatedPantsName, TEXT("PantsMeshRowFind"));
	Pants->SetSkeletalMesh(pantsMeshRow->PantsMesh);
}

void ACLobbySurvivor::OnRep_ReplicatedPantsName()
{
	UpdateSkeletalPantsMesh();
}

void ACLobbySurvivor::PerformSetBootsMesh(int32 InIndex)
{
	FString indexString = FString::Printf(TEXT("%02d"), InIndex);
	FString headString = "Boots_";
	FString combinedString = headString.Append(indexString);
	FName index(*combinedString);
	ReplicatedBootsName = index;
	FSkeletalBootsMeshRow* bootsMeshRow = CustomizeBootsData->FindRow<FSkeletalBootsMeshRow>(ReplicatedBootsName, TEXT("BootsMeshRowFind"));
	Boots->SetSkeletalMesh(bootsMeshRow->BootsMesh);
}

void ACLobbySurvivor::RequestSetBootsMesh_Implementation(int32 InIndex)
{
	PerformSetBootsMesh(InIndex);
}

bool ACLobbySurvivor::RequestSetBootsMesh_Validate(int32 InIndex)
{
	return true;
}

void ACLobbySurvivor::UpdateSkeletalBootsMesh()
{
	FSkeletalBootsMeshRow* bootsMeshRow = CustomizeBootsData->FindRow<FSkeletalBootsMeshRow>(ReplicatedBootsName, TEXT("BootsMeshRowFind"));
	Boots->SetSkeletalMesh(bootsMeshRow->BootsMesh);
}

void ACLobbySurvivor::OnRep_ReplicatedBootsName()
{
	UpdateSkeletalBootsMesh();
}

void ACLobbySurvivor::PerformSetSkinColor(int32 InIndex)
{
	FString indexString = FString::Printf(TEXT("%02d"), InIndex);
	FString headColorString = "SkinColor_";
	FString combinedString = headColorString.Append(indexString);
	FName index(*combinedString);

	ReplicatedSkinColorName = index;

	FSkeletalSkinMeshColorRow* skinColorRow = CustomizeSkinColorData->FindRow<FSkeletalSkinMeshColorRow>(ReplicatedSkinColorName, TEXT("SkinMeshColorRowFind"));
	
	UMaterialInstanceDynamic* dynamicHeadMaterial = Cast<UMaterialInstanceDynamic>(Head->GetMaterial(0));
	if (IsValid(dynamicHeadMaterial))
	{
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		MeshColorMaterial = Head->GetMaterial(0);
		DynamicHeadMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Head->SetMaterial(0, DynamicHeadMeshColorMaterial);
	}

	UMaterialInstanceDynamic* dynamicPantsMaterial = Cast<UMaterialInstanceDynamic>(Pants->GetMaterial(0));
	if (IsValid(dynamicPantsMaterial))
	{
		DynamicPantsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		MeshColorMaterial = Pants->GetMaterial(0);
		DynamicPantsMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicPantsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Pants->SetMaterial(0, DynamicPantsMeshColorMaterial);
	}

	UMaterialInstanceDynamic* dynamicBootsMaterial = Cast<UMaterialInstanceDynamic>(Boots->GetMaterial(0));
	if (IsValid(dynamicBootsMaterial))
	{
		DynamicBootsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		MeshColorMaterial = Boots->GetMaterial(0);
		DynamicBootsMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicBootsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Boots->SetMaterial(0, DynamicBootsMeshColorMaterial);
	}

	UMaterialInstanceDynamic* dynamicBodyMaterial = Cast<UMaterialInstanceDynamic>(Body->GetMaterial(0));
	if (IsValid(dynamicBodyMaterial))
	{
		DynamicBodyMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		MeshColorMaterial = Body->GetMaterial(0);
		DynamicBodyMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicBodyMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Body->SetMaterial(0, DynamicBodyMeshColorMaterial);
	}

	if (IsValid(DynamicHandsMeshColorMaterial))
	{
		DynamicHandsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		MeshColorMaterial = Hands->GetMaterial(0);
		DynamicHandsMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicHandsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Hands->SetMaterial(0, DynamicHandsMeshColorMaterial);
	}
}

void ACLobbySurvivor::RequestSetSkinColor_Implementation(int32 InIndex)
{
	PerformSetSkinColor(InIndex);
}

bool ACLobbySurvivor::RequestSetSkinColor_Validate(int32 InIndex)
{
	return true;
}

void ACLobbySurvivor::UpdateSkinColor()
{
	FSkeletalSkinMeshColorRow* skinColorRow = CustomizeSkinColorData->FindRow<FSkeletalSkinMeshColorRow>(ReplicatedSkinColorName, TEXT("SkinMeshColorRowFind"));

	UMaterialInstanceDynamic* dynamicHeadMaterial = Cast<UMaterialInstanceDynamic>(Head->GetMaterial(0));
	if (IsValid(dynamicHeadMaterial))
	{
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		MeshColorMaterial = Head->GetMaterial(0);
		DynamicHeadMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicHeadMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Head->SetMaterial(0, DynamicHeadMeshColorMaterial);
	}

	UMaterialInstanceDynamic* dynamicPantsMaterial = Cast<UMaterialInstanceDynamic>(Pants->GetMaterial(0));
	if (IsValid(dynamicPantsMaterial))
	{
		DynamicPantsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		//FSkeletalPantsMeshRow* singleMeshRow = CustomizePantsData->FindRow<FSkeletalPantsMeshRow>(("Pants_01"), TEXT("PantsMeshColorRowFind"));
		//Pants->SetSkeletalMesh(singleMeshRow->PantsMesh);
		MeshColorMaterial = Pants->GetMaterial(0);
		DynamicPantsMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicPantsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Pants->SetMaterial(0, DynamicPantsMeshColorMaterial);
	}

	UMaterialInstanceDynamic* dynamicBootsMaterial = Cast<UMaterialInstanceDynamic>(Boots->GetMaterial(0));
	if (IsValid(dynamicBootsMaterial))
	{
		DynamicBootsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		//FSkeletalBootsMeshRow* singleMeshRow = CustomizeBootsData->FindRow<FSkeletalBootsMeshRow>(("Boots_01"), TEXT("BootsMeshColorRowFind"));
		//Boots->SetSkeletalMesh(singleMeshRow->BootsMesh);
		MeshColorMaterial = Boots->GetMaterial(0);
		DynamicBootsMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		DynamicBootsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		Boots->SetMaterial(0, DynamicBootsMeshColorMaterial);
	}

	if(IsValid(DynamicBodyMeshColorMaterial))
	{
		DynamicBodyMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
		DynamicHandsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
	else
	{
		FSkeletalSingleMeshRow* singleMeshRow = CustomizeSingleData->FindRow<FSkeletalSingleMeshRow>(("Single"), TEXT("SkinMeshColorRowFind"));
		Body->SetSkeletalMesh(singleMeshRow->BodyMesh);
		MeshColorMaterial = Body->GetMaterial(0);
		DynamicBodyMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		Body->SetMaterial(0, DynamicBodyMeshColorMaterial);
		DynamicBodyMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);

		Hands->SetSkeletalMesh(singleMeshRow->HandsMesh);
		MeshColorMaterial = Hands->GetMaterial(0);
		DynamicHandsMeshColorMaterial = UMaterialInstanceDynamic::Create(MeshColorMaterial, this);
		Hands->SetMaterial(0, DynamicHandsMeshColorMaterial);
		DynamicHandsMeshColorMaterial->SetVectorParameterValue("Skin Tint", skinColorRow->SkinMeshColor);
	}
}

void ACLobbySurvivor::OnRep_ReplicatedSkinColorName()
{
	UpdateSkinColor();
}

// 이 함수는 클라이언트가 서버로부터 변수 변경 사항을 수신한 후 호출되므로, 약간의 지연이 있을 수 있다.
void ACLobbySurvivor::OnRep_ReplicatedSurvivorName()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_ReplicatedSurvivorName Called"));
	UpdateSurvivorNameWidget(); // 리플리케이트 완료되었으니 클라 입장에서 서버의 값으로 위젯을 모두 업데이트 한다. 이 부분을 주석처리하면 클라 입장에선 모두 텍스트 기본값으로 보임
}

void ACLobbySurvivor::RequestDifficultyUpdate_Implementation()
{
	ClientDifficultyUpdate();
}

bool ACLobbySurvivor::RequestDifficultyUpdate_Validate()
{
	return true;
}

void ACLobbySurvivor::ClientDifficultyUpdate()
{
	ACLobbySurvivorController* lobbySurvivorController = Cast<ACLobbySurvivorController>(GetWorld()->GetFirstPlayerController());
	lobbySurvivorController->UpdateClientDifficulty();
}

void ACLobbySurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedSurvivorName); 
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedHeadName); 
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedHeadColorName); 
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedPantsName); 
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedBootsName);
	DOREPLIFETIME(ACLobbySurvivor, ReplicatedSkinColorName);
}
