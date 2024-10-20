#include "ActorComponents/CInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Character/CSurvivorController.h"
#include "World/CPickUp.h"
#include "Widget/CMainHUD.h"
#include "Widget/Inventory/CItemBase.h"
#include "Widget/Inventory/CInteractionWidget.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Character/CSurvivor.h"
#include "Utility/CDebug.h"

UCInteractionComponent::UCInteractionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	InteractionCheckFrequency = 0.2f;
	InteractionCheckDistance = 300.0f;

}


void UCInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCInteractionComponent, TargetInteractable);
	
}


void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter) 
	{
		ACSurvivorController* playerController = Cast<ACSurvivorController>(OwnerCharacter->GetController());
		if (playerController && playerController->IsLocalController())
		{
			HUD = Cast<ACMainHUD>(playerController->GetHUD());
		}
	}
}


void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//몇 초 지났는지 확인... 
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck(); //검출 
	}

}


bool UCInteractionComponent::IsInteracting() const
{
	return OwnerCharacter->GetWorldTimerManager().IsTimerActive(LongPressTimerHandle); //상호작용 시작으로부터 타이머 설정 
}


//E키 누르면 
void UCInteractionComponent::DoInteract()
{
	// 상세 정보 타이머 시작
	
	GetWorld()->GetTimerManager().SetTimer(LongPressTimerHandle, this, &UCInteractionComponent::ToggleHiddenMenu, 0.5f, false);

	//주위 범위 상호작용 트리거 타이머 시작 
	GetWorld()->GetTimerManager().SetTimer(InteractAroundTimerHandle, this, &UCInteractionComponent::GatherAround, 0.7f, false);

}

//E 키 떼면 
void UCInteractionComponent::FinishInteract()
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(LongPressTimerHandle); 

	if (IsValid(TargetInteractable.GetObject()))
	{
		const FInteractableData* InteractableData = &TargetInteractable->InteractableData;
		if (InteractableData->InteractableType == EInteractableType::Build && !InteractableData->bIsDropMesh)
		{
			if (ElapsedTime < 0.25f && ElapsedTime >= 0) //0.25 초 내의 짧은 E 키 Released 면 
				BeginInteract();
			else
			{
				HUD->HideHiddenMenu();
				EndInteract();
			}
		}
		else if (InteractableData->InteractableType == EInteractableType::Container && !InteractableData->bIsDropMesh)
		{
			if (ElapsedTime < 0.25f && ElapsedTime >= 0) //0.25 초 내의 짧은 E 키 Released 면 
				BeginInteract();
			else
			{
				HUD->HideHiddenMenu();
				EndInteract();
			}
		}
		else
		{
			if (ElapsedTime < 0.25f && ElapsedTime >= 0) //0.25 초 내의 짧은 E 키 Released 면 
				BeginInteract();
			else
			{
				HUD->HideHiddenMenu();
				EndInteract();
			}
		}

	}
	
	//조건에 상관없이 범위 상호작용 트리거 종료
	OwnerCharacter->GetWorldTimerManager().ClearTimer(InteractAroundTimerHandle);
	

	
}




//인터렉션 메뉴 Switch On/OFF
void UCInteractionComponent::ToggleHiddenMenu()
{
	if (HUD)
		HUD->ToggleHiddenMenu();
}

void UCInteractionComponent::ShowHiddenMenu()
{
	if (HUD)
		HUD->ShowHiddenMenu();

}

void UCInteractionComponent::HideHiddenMenu()
{
	if (HUD)
		HUD->HideHiddenMenu();
}


void UCInteractionComponent::ExtraOptionButtonUp()
{
	if (HUD)
		HUD->ExtraOptionButtonUp();
}

void UCInteractionComponent::ExtraOptionButtonDown()
{
	if (HUD)
		HUD->ExtraOptionButtonDown();
}

void UCInteractionComponent::RecallInteract(bool bIsLongPressed)
{
	Interact(bIsLongPressed);
}




void UCInteractionComponent::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

//탐색 함수 
void UCInteractionComponent::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	/*FVector Start = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y, OwnerCharacter->GetActorLocation().Z + 45.0f) + ForwardVector.GetSafeNormal() * TraceOffset;*/

	//눈 시점 location
	FVector TraceStart = OwnerCharacter->GetPawnViewLocation() + OwnerCharacter->GetActorForwardVector() * 50.0f;//트레이스 벡터 초기화 
	FVector TraceEnd = TraceStart + (OwnerCharacter->GetViewRotation().Vector() * InteractionCheckDistance) ;

	//액터 자체의 forward Vector 와 pawn의 controller 에서의 방향벡터와의 내적을 통해 캐릭터가 앞을 보고 있는지 판단 
	float LookDirection =  FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), OwnerCharacter->GetViewRotation().Vector());
	
	//0~1양수면 같은 방향을 , 0~-1음수면 반대방향 
	if (LookDirection > 0)
	{ // 같은 방향일때만 Trace 

		////DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, 1.0f, 0, 3.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		FHitResult TraceHit;

		// 트레이스 박스의 크기 및 회전 
		FVector BoxHalfSize(35.0f, 35.0f, 35.0f);
		FQuat BoxRotation = OwnerCharacter->GetViewRotation().Quaternion();

		//디버그 BoxTrace
		/*DrawDebugBox(GetWorld(), TraceStart, BoxHalfSize, BoxRotation, FColor::Green, false, 1.0f);
		DrawDebugBox(GetWorld(), TraceEnd, BoxHalfSize, BoxRotation, FColor::Red, false, 1.0f);*/

		if (GetWorld()->SweepSingleByChannel(TraceHit, TraceStart, TraceEnd, BoxRotation, ECC_Visibility, FCollisionShape::MakeBox(BoxHalfSize), QueryParams))
		{
			//UInteractionInterface 를 상속받고 있는지 검출(상호작용 가능한 액터인지)
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable) 
				{
					HideHiddenMenu();
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable) //최근에 감지한 액터면 
				{
					return;
				}
			}


		}

	}
	NoInteractableFound();
}

//상호작용 가능 조건을 충족하는 액터를 찾았을시 
void UCInteractionComponent::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) // 현재 이미 상호작용중이면 상호작용 종료 
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable) //최근에 이미 상호작용한 액터를 발견했으면 
	{
		TargetInteractable = InteractionData.CurrentInteractable; //타겟을 최근에발견한 것으로 설정 
		if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
		{
			TargetInteractable->EndFocus();
		}
		
	}


	InteractionData.CurrentInteractable = NewInteractable; //기록 갱신 
	TargetInteractable = NewInteractable;
	if(HUD)
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData); //인터렉션 위젯 갱신 
	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		TargetInteractable->BeginFocus(); //지금 검출해서 찾은 것으로 Focus 시작 
	}

}

void UCInteractionComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		OwnerCharacter->GetWorldTimerManager().ClearTimer(LongPressTimerHandle);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject())) //상호작용한 타겟 액터가 PickUp 되면서 Destroy() 되었는지 체크
		{
			TargetInteractable->EndFocus(); //Destroy 되기 전에 타겟 해제하여 널포인터 참조 방지 
		}

		//상호작용 위젯을 숨김 구현 부분
		if (HUD)
			HUD->HideInteractionWidget(); 

		//상호작용 관련 액터 초기화 (최근 액터 , 현재 타겟 액터)
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

//상호작용 시작 (PickUp , Open 등등)
void UCInteractionComponent::BeginInteract()
{
	//상호작용 하기전에 타겟 적절한지 다시 한번 확인  
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			//TargetInteractable->BeginInteract(); // 이후 지울것
		}
		Interact(false);
	}
}



void UCInteractionComponent::EndInteract()
{
	OwnerCharacter->GetWorldTimerManager().ClearTimer(LongPressTimerHandle);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}

}



void UCInteractionComponent::Interact(bool bIsLongPressed)
{
	OwnerCharacter->GetWorldTimerManager().ClearTimer(LongPressTimerHandle);
	if (IsValid(TargetInteractable.GetObject()))
	{
		ACSurvivor* playerCharacter = Cast<ACSurvivor>(OwnerCharacter);
		if (playerCharacter)
		{
			TargetInteractable->OnRequestDesroyCalled.AddUObject(this, &UCInteractionComponent::RequestDestroy);
			TargetInteractable->OnUpdatePartialAdded.AddUObject(this, &UCInteractionComponent::RequestUpdatePartialAdded);
			TargetInteractable->Interact(playerCharacter, bIsLongPressed);
		}
	}
}

void UCInteractionComponent::InteractAroundPlayer()
{
	//if (!IsValid(TargetInteractable.GetObject()))
	//{
	//	GatherAround();
	//	CDebug::Print("GatherAround Called");
	//}
	// 
	// 
	//{
	//	CDebug::Print("Clear Timer Called");
	//	GetWorld()->GetTimerManager().ClearTimer(InteractAroundTimerHandle);
	//}
}

void UCInteractionComponent::GatherAround()
{
	if (HUD && HUD->GetInteractionWidget()->GetVisibility() == ESlateVisibility::Visible)
		return;

	float SphereRadius = 200.0f; // 탐색 반지름 설정

	FVector TraceStart = OwnerCharacter->GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);

	// 멀티 스피어 트레이스 실행
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceStart, 
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius),
		QueryParams
	);

	// 트레이스된 PickUp Actor들을 담을 TArray
	TArray<ACPickUp*> PickUpsInSphere;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			
			if (IsValid(Hit.GetActor())&&Hit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				
				TargetInteractable = Hit.GetActor();
				RequestUpdateTarget(Hit.GetActor());

				ACPickUp* pickUpActor = Cast<ACPickUp>(Hit.GetActor());
				if (pickUpActor)
				{
					if (pickUpActor->ItemReference->ItemType == EItemType::Build || pickUpActor->ItemReference->ItemType == EItemType::Container)
					{
						if (pickUpActor->ItemReference->bIsDropMesh)
							Interact(false);
					}
					else
						Interact(false);
				}
			}
		}
	}

	// 디버그 스피어 그리기
	DrawDebugSphere(GetWorld(), TraceStart, SphereRadius, 50, FColor::Green, false, 0.1f);

	


}

void UCInteractionComponent::RequestUpdatePartialAdded_Implementation(int32 InQuantity)
{
	
			if (OwnerCharacter->HasAuthority())
				BroadcastUpdatePartialAdded(InQuantity);
				
}


void UCInteractionComponent::BroadcastUpdatePartialAdded_Implementation(int32 InQuantity)
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		ACPickUp* PickupActor = Cast<ACPickUp>(TargetInteractable.GetObject());
		if (PickupActor)
		{


			if (OwnerCharacter->HasAuthority())
			{
				PickupActor->UpdatePartialAdded(InQuantity);
			}


		}

	}
}



void UCInteractionComponent::RequestDestroy_Implementation()
{

	if (IsValid(TargetInteractable.GetObject()))
	{
		ACPickUp* PickupActor = Cast<ACPickUp>(TargetInteractable.GetObject());
		if (PickupActor)
		{


			if(OwnerCharacter->HasAuthority())
					PickupActor->Destroy();

		}

	}




}

void UCInteractionComponent::RequestUpdateTarget_Implementation(AActor* InActor)
{
	TargetInteractable = InActor;

}
