// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/Disposable/CCustomizeComponent.h"
#include "Character/CSurvivor.h"
#include "CGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"



UCCustomizeComponent::UCCustomizeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	//	CustomizeIndex = -1;

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeHeadDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizeHead.DT_CustomizeHead'"));
	if (customizeHeadDataFinder.Succeeded())
	{
		CustomizeHeadData = customizeHeadDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeHeadDataFinder Failed");
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeHeadColorDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizeHeadColor.DT_CustomizeHeadColor'"));
	if (customizeHeadColorDataFinder.Succeeded())
	{
		CustomizeHeadColorData = customizeHeadColorDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeHeadColorDataFinder Failed");
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

	static ConstructorHelpers::FObjectFinder<UDataTable> customizeSkinColorDataFinder(TEXT("DataTable'/Game/PirateIsland/Include/Datas/Widget/DT_CustomizeSkinColor.DT_CustomizeSkinColor'"));
	if (customizeSkinColorDataFinder.Succeeded())
	{
		CustomizeSkinColorData = customizeSkinColorDataFinder.Object;
	}
	else
	{
		CDebug::Log("customizeSkinColorDataFinder Failed");
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
}

void UCCustomizeComponent::BeginPlay()
{
	Super::BeginPlay();
	ComponentOwner = this->GetOwner();
	Survivor = Cast<ACSurvivor>(ComponentOwner);
	
//	ReplicatedHeadColorName = CustomizeRowNames[1];
//	ReplicatedPantsName = CustomizeRowNames[2];
//	ReplicatedBootsName = CustomizeRowNames[3];
//	ReplicatedSkinColorName = CustomizeRowNames[4];
	FTimerHandle handle;

	// 커스터마이징 시작, GameInstance의 변수값이 Null 일 경우 실행되지 않음
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UCCustomizeComponent::DoSkeletalMeshReplicate, 0.1f, false); 
}

void UCCustomizeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCCustomizeComponent::DoSkeletalMeshReplicate()
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	CustomizeRowNames = gameInstance->GetCustomizeRowNames();
	if (CustomizeRowNames.Num() == 5)
	{
		if (Survivor->HasAuthority())
		{
			PerformSetHeadMesh(CustomizeRowNames[0], CustomizeRowNames[1], CustomizeRowNames[2], CustomizeRowNames[3], "Single", CustomizeRowNames[4]);
		}
		else
		{
			RequestSetHeadMesh(CustomizeRowNames[0], CustomizeRowNames[1], CustomizeRowNames[2], CustomizeRowNames[3], "Single", CustomizeRowNames[4]);
		}
	}
	else
	{
		CDebug::Print(TEXT("Can't Find Customize Info. Check GameInstance"));
	}
}

void UCCustomizeComponent::PerformSetHeadMesh(FName InHeadName, FName InHeadColorName, FName InPantsName, FName InBootsName, FName InSingleName, FName InSkinColorName)
{
	ReplicatedHeadName = InHeadName;
	FSkeletalHeadMeshRow* headMeshRow = CustomizeHeadData->FindRow<FSkeletalHeadMeshRow>(ReplicatedHeadName, TEXT("Head"));
	USkeletalMesh* headMesh = headMeshRow->HeadMesh;
	Survivor->GetHeadMeshComponent()->SetSkeletalMesh(headMesh);
	CDebug::Print(ReplicatedHeadName.ToString(), FColor::Green);

	ReplicatedHeadColorName = InHeadColorName;
	FSkeletalHeadMeshColorRow* headMeshColorRow = CustomizeHeadColorData->FindRow<FSkeletalHeadMeshColorRow>(ReplicatedHeadColorName, TEXT("HeadColor"));
	FLinearColor headMeshColor = headMeshColorRow->HeadMeshColor;
	UMaterialInterface* headColorMaterial = Survivor->GetHeadMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempHeadDynamicMaterial = Cast<UMaterialInstanceDynamic>(headColorMaterial);
	if (IsValid(tempHeadDynamicMaterial))
	{
		tempHeadDynamicMaterial->SetVectorParameterValue("Hair Tint", headMeshColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicHeadColorMaterial = UMaterialInstanceDynamic::Create(headColorMaterial, this);
		dynamicHeadColorMaterial->SetVectorParameterValue("Hair Tint", headMeshColor);
		Survivor->GetHeadMeshComponent()->SetMaterial(0, dynamicHeadColorMaterial);
	}

	ReplicatedPantsName = InPantsName;
	FSkeletalPantsMeshRow* pantsMeshRow = CustomizePantsData->FindRow<FSkeletalPantsMeshRow>(ReplicatedPantsName, TEXT("Pants"));
	USkeletalMesh* pantsMesh = pantsMeshRow->PantsMesh;
	Survivor->GetPantsMeshComponent()->SetSkeletalMesh(pantsMesh);
	Survivor->GetPantsMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	ReplicatedBootsName = InBootsName;
	FSkeletalBootsMeshRow* bootsMeshRow = CustomizeBootsData->FindRow<FSkeletalBootsMeshRow>(ReplicatedBootsName, TEXT("Boots"));
	USkeletalMesh* bootsMesh = bootsMeshRow->BootsMesh;
	Survivor->GetBootsMeshComponent()->SetSkeletalMesh(bootsMesh);
	Survivor->GetBootsMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	ReplicatedSingleName = InSingleName;
	FSkeletalSingleMeshRow* singleMeshRow = CustomizeSingleData->FindRow<FSkeletalSingleMeshRow>(ReplicatedSingleName, TEXT("Single"));

	USkeletalMesh* bodyMesh = singleMeshRow->BodyMesh;
	Survivor->GetBodyMeshComponent()->SetSkeletalMesh(bodyMesh);
	Survivor->GetBodyMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	USkeletalMesh* handsMesh = singleMeshRow->HandsMesh;
	Survivor->GetHandsMeshComponent()->SetSkeletalMesh(handsMesh);
	Survivor->GetHandsMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	USkeletalMesh* accessoryMesh = singleMeshRow->AccessoryMesh;
	Survivor->GetAccessoryMeshComponent()->SetSkeletalMesh(accessoryMesh);
	Survivor->GetAccessoryMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	ReplicatedSkinColorName = InSkinColorName;
	FSkeletalSkinMeshColorRow* skinColorRow = CustomizeSkinColorData->FindRow<FSkeletalSkinMeshColorRow>(ReplicatedSkinColorName, TEXT("SkinColor"));
	FLinearColor skinColor = skinColorRow->SkinMeshColor;

	UMaterialInterface* headSkinMaterial = Survivor->GetHeadMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempHeadSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(headSkinMaterial);
	if (IsValid(tempHeadSkinDynamicMaterial))
	{
		tempHeadSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicHeadSkinColorMaterial = UMaterialInstanceDynamic::Create(headSkinMaterial, this);
		dynamicHeadSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetHeadMeshComponent()->SetMaterial(0, dynamicHeadSkinColorMaterial);
	}

	UMaterialInterface* bodySkinMaterial = Survivor->GetBodyMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempBodySkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(bodySkinMaterial);
	if (IsValid(tempBodySkinDynamicMaterial))
	{
		tempBodySkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicBodySkinColorMaterial = UMaterialInstanceDynamic::Create(bodySkinMaterial, this);
		dynamicBodySkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetBodyMeshComponent()->SetMaterial(0, dynamicBodySkinColorMaterial);
	}

	UMaterialInterface* handsSkinMaterial = Survivor->GetHandsMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempHandsSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(handsSkinMaterial);
	if (IsValid(tempHandsSkinDynamicMaterial))
	{
		tempHandsSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicHandsSkinColorMaterial = UMaterialInstanceDynamic::Create(handsSkinMaterial, this);
		dynamicHandsSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetHandsMeshComponent()->SetMaterial(0, dynamicHandsSkinColorMaterial);
	}

	UMaterialInterface* pantsSkinMaterial = Survivor->GetPantsMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempPantsSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(pantsSkinMaterial);
	if (IsValid(tempPantsSkinDynamicMaterial))
	{
		tempPantsSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicPantsSkinColorMaterial = UMaterialInstanceDynamic::Create(pantsSkinMaterial, this);
		dynamicPantsSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetPantsMeshComponent()->SetMaterial(0, dynamicPantsSkinColorMaterial);
	}

	UMaterialInterface* bootsSkinMaterial = Survivor->GetBootsMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempBootsSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(bootsSkinMaterial);
	if (IsValid(tempBootsSkinDynamicMaterial))
	{
		tempBootsSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicBootsSkinColorMaterial = UMaterialInstanceDynamic::Create(bootsSkinMaterial, this);
		dynamicBootsSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetBootsMeshComponent()->SetMaterial(0, dynamicBootsSkinColorMaterial);
	}
}

void UCCustomizeComponent::RequestSetHeadMesh_Implementation(FName InHeadName, FName InHeadColorName, FName InPantsName, FName InBootsName, FName InSingleName, FName InSkinColorName)
{
	PerformSetHeadMesh(InHeadName, InHeadColorName, InPantsName, InBootsName, InSingleName, InSkinColorName);
}

bool UCCustomizeComponent::RequestSetHeadMesh_Validate(FName InHeadName, FName InHeadColorName, FName InPantsName, FName InBootsName, FName InSingleName, FName InSkinColorName)
{
	return true;
}

void UCCustomizeComponent::OnRep_ReplicatedHeadName()
{
	UpdateHeadMesh();
}

void UCCustomizeComponent::UpdateHeadMesh()
{
	FSkeletalHeadMeshRow* headMeshRow = CustomizeHeadData->FindRow<FSkeletalHeadMeshRow>(ReplicatedHeadName, TEXT("Head"));
	USkeletalMesh* headMesh = headMeshRow->HeadMesh;
	Survivor->GetHeadMeshComponent()->SetSkeletalMesh(headMesh);
	CDebug::Print(ReplicatedHeadName.ToString(), FColor::Green);
}

void UCCustomizeComponent::OnRep_ReplicatedHeadColorName()
{
	UpdateHeadColor();
}

void UCCustomizeComponent::UpdateHeadColor()
{
	UMaterialInstanceDynamic* temp = Cast<UMaterialInstanceDynamic>(Survivor->GetHeadMeshComponent()->GetMaterial(0));
	if (IsValid(temp))
	{
		FSkeletalHeadMeshColorRow* headMeshColorRow = CustomizeHeadColorData->FindRow<FSkeletalHeadMeshColorRow>(ReplicatedHeadColorName, TEXT("HeadColor"));
		FLinearColor headMeshColor = headMeshColorRow->HeadMeshColor;
		temp->SetVectorParameterValue("Hair Tint", headMeshColor);
	}
	else
	{
		FSkeletalHeadMeshColorRow* headMeshColorRow = CustomizeHeadColorData->FindRow<FSkeletalHeadMeshColorRow>(ReplicatedHeadColorName, TEXT("HeadColor"));
		FLinearColor headMeshColor = headMeshColorRow->HeadMeshColor;
		UMaterialInterface* headColorMaterial = Survivor->GetHeadMeshComponent()->GetMaterial(0);
		UMaterialInstanceDynamic* dynamicHeadColorMaterial = UMaterialInstanceDynamic::Create(headColorMaterial, this);
		dynamicHeadColorMaterial->SetVectorParameterValue("Hair Tint", headMeshColor);
		Survivor->GetHeadMeshComponent()->SetMaterial(0, dynamicHeadColorMaterial);
	}
}

void UCCustomizeComponent::OnRep_ReplicatedPantsName()
{
	UpdatePantsMesh();
}

void UCCustomizeComponent::UpdatePantsMesh()
{
	FSkeletalPantsMeshRow* pantsMeshRow = CustomizePantsData->FindRow<FSkeletalPantsMeshRow>(ReplicatedPantsName, TEXT("Pants"));
	USkeletalMesh* pantsMesh = pantsMeshRow->PantsMesh;
	Survivor->GetPantsMeshComponent()->SetSkeletalMesh(pantsMesh);
	Survivor->GetPantsMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void UCCustomizeComponent::OnRep_ReplicatedBootsName()
{
	UpdateBootsMesh();
}

void UCCustomizeComponent::UpdateBootsMesh()
{
	FSkeletalBootsMeshRow* bootsMeshRow = CustomizeBootsData->FindRow<FSkeletalBootsMeshRow>(ReplicatedBootsName, TEXT("Boots"));
	USkeletalMesh* bootsMesh = bootsMeshRow->BootsMesh;
	Survivor->GetBootsMeshComponent()->SetSkeletalMesh(bootsMesh);
	Survivor->GetBootsMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void UCCustomizeComponent::OnRep_ReplicatedSingleName()
{
	UpdateSingleMesh();
}

void UCCustomizeComponent::UpdateSingleMesh()
{
	FSkeletalSingleMeshRow* singleMeshRow = CustomizeSingleData->FindRow<FSkeletalSingleMeshRow>(ReplicatedSingleName, TEXT("Single"));

	USkeletalMesh* bodyMesh = singleMeshRow->BodyMesh;
	Survivor->GetBodyMeshComponent()->SetSkeletalMesh(bodyMesh);
	Survivor->GetBodyMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	USkeletalMesh* handsMesh = singleMeshRow->HandsMesh;
	Survivor->GetHandsMeshComponent()->SetSkeletalMesh(handsMesh);
	Survivor->GetHandsMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	USkeletalMesh* accessoryMesh = singleMeshRow->AccessoryMesh;
	Survivor->GetAccessoryMeshComponent()->SetSkeletalMesh(accessoryMesh);
	Survivor->GetAccessoryMeshComponent()->AttachToComponent(Survivor->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
}

void UCCustomizeComponent::OnRep_ReplicatedSkinColorName()
{
	UpdateSkinColor();
}

void UCCustomizeComponent::UpdateSkinColor()
{
	FSkeletalSkinMeshColorRow* skinColorRow = CustomizeSkinColorData->FindRow<FSkeletalSkinMeshColorRow>(ReplicatedSkinColorName, TEXT("SkinColor"));
	FLinearColor skinColor = skinColorRow->SkinMeshColor;

	UMaterialInterface* headSkinMaterial = Survivor->GetHeadMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempHeadSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(headSkinMaterial);
	if (IsValid(tempHeadSkinDynamicMaterial))
	{
		tempHeadSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicHeadSkinColorMaterial = UMaterialInstanceDynamic::Create(headSkinMaterial, this);
		dynamicHeadSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetHeadMeshComponent()->SetMaterial(0, dynamicHeadSkinColorMaterial);
	}

	UMaterialInterface* bodySkinMaterial = Survivor->GetBodyMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempBodySkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(bodySkinMaterial);
	if (IsValid(tempBodySkinDynamicMaterial))
	{
		tempBodySkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicBodySkinColorMaterial = UMaterialInstanceDynamic::Create(bodySkinMaterial, this);
		dynamicBodySkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetBodyMeshComponent()->SetMaterial(0, dynamicBodySkinColorMaterial);
	}

	UMaterialInterface* handsSkinMaterial = Survivor->GetHandsMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempHandsSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(handsSkinMaterial);
	if (IsValid(tempHandsSkinDynamicMaterial))
	{
		tempHandsSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicHandsSkinColorMaterial = UMaterialInstanceDynamic::Create(handsSkinMaterial, this);
		dynamicHandsSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetHandsMeshComponent()->SetMaterial(0, dynamicHandsSkinColorMaterial);
	}

	UMaterialInterface* pantsSkinMaterial = Survivor->GetPantsMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempPantsSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(pantsSkinMaterial);
	if (IsValid(tempPantsSkinDynamicMaterial))
	{
		tempPantsSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicPantsSkinColorMaterial = UMaterialInstanceDynamic::Create(pantsSkinMaterial, this);
		dynamicPantsSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetPantsMeshComponent()->SetMaterial(0, dynamicPantsSkinColorMaterial);
	}

	UMaterialInterface* bootsSkinMaterial = Survivor->GetBootsMeshComponent()->GetMaterial(0);
	UMaterialInstanceDynamic* tempBootsSkinDynamicMaterial = Cast<UMaterialInstanceDynamic>(bootsSkinMaterial);
	if (IsValid(tempBootsSkinDynamicMaterial))
	{
		tempBootsSkinDynamicMaterial->SetVectorParameterValue("Skin Tint", skinColor);
	}
	else
	{
		UMaterialInstanceDynamic* dynamicBootsSkinColorMaterial = UMaterialInstanceDynamic::Create(bootsSkinMaterial, this);
		dynamicBootsSkinColorMaterial->SetVectorParameterValue("Skin Tint", skinColor);
		Survivor->GetBootsMeshComponent()->SetMaterial(0, dynamicBootsSkinColorMaterial);
	}
}

void UCCustomizeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCCustomizeComponent, ReplicatedHeadName);
	DOREPLIFETIME(UCCustomizeComponent, ReplicatedHeadColorName);
	DOREPLIFETIME(UCCustomizeComponent, ReplicatedPantsName);
	DOREPLIFETIME(UCCustomizeComponent, ReplicatedBootsName);
	DOREPLIFETIME(UCCustomizeComponent, ReplicatedSingleName);
	DOREPLIFETIME(UCCustomizeComponent, ReplicatedSkinColorName);
}
