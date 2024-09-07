

#include "ActorComponents/CInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Character/CSurvivorController.h"
#include "World/CPickUp.h"
#include "Widget/CMainHUD.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Character/CSurvivor.h"
UCInteractionComponent::UCInteractionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	InteractionCheckFrequency = 0.2f;
	InteractionCheckDistance = 300.0f;

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

//void UCInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	
//	
//
//}

bool UCInteractionComponent::IsInteracting() const
{
	return OwnerCharacter->GetWorldTimerManager().IsTimerActive(LongPressTimerHandle); //상호작용 시작으로부터 타이머 설정 
}


//E키 누르면 
void UCInteractionComponent::DoInteract()
{
	// 상세 정보 타이머 시작
	bIsLongPress = false;
	GetWorld()->GetTimerManager().SetTimer(LongPressTimerHandle, this, &UCInteractionComponent::ToggleHiddenMenu, 0.5f, false);
	
}

//E 키 떼면 
void UCInteractionComponent::FinishInteract()
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(LongPressTimerHandle);

	if (IsValid(TargetInteractable.GetObject()))
	{
		const FInteractableData* InteractableData = &TargetInteractable->InteractableData;
		if (InteractableData->InteractableType == EInteractableType::Build)
		{
			HUD->HideHiddenMenu();
			EndInteract();
		}
		else
		{
			if (ElapsedTime < 0.25f && ElapsedTime >= 0) //0.25 초 내의 짧은 E 키 Released 면 
				BeginInteract();
			else
				EndInteract();
		}

	}

	
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

void UCInteractionComponent::RecallInteract()
{
	Interact();
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

	//눈 시점 location
	FVector TraceStart{ OwnerCharacter->GetPawnViewLocation()}; //트레이스 벡터 초기화 
	FVector TraceEnd{ TraceStart + (OwnerCharacter->GetViewRotation().Vector() * InteractionCheckDistance) };

	//액터 자체의 forward Vector 와 pawn의 controller 에서의 방향벡터와의 내적을 통해 캐릭터가 앞을 보고 있는지 판단 
	float LookDirection =  FVector::DotProduct(OwnerCharacter->GetActorForwardVector(), OwnerCharacter->GetViewRotation().Vector());
	
	//0~1양수면 같은 방향을 , 0~-1음수면 반대방향 
	if (LookDirection > 0)
	{ // 같은 방향일때만 Trace 

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, 1.0f, 0, 3.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
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
		TargetInteractable->EndFocus(); //타겟 엑터에 가하고 있는 Focus 해제 

	}


	InteractionData.CurrentInteractable = NewInteractable; //기록 갱신 
	TargetInteractable = NewInteractable;
	if(HUD)
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData); //인터렉션 위젯 갱신 
	TargetInteractable->BeginFocus(); //지금 검출해서 찾은 것으로 Focus 시작 


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
			TargetInteractable->BeginInteract();
		}
			Interact();
		
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



void UCInteractionComponent::Interact()
{
	OwnerCharacter->GetWorldTimerManager().ClearTimer(LongPressTimerHandle);
	if (IsValid(TargetInteractable.GetObject()))
	{
		ACSurvivor* playerCharacter = Cast<ACSurvivor>(OwnerCharacter);
		if (playerCharacter)
		{
			TargetInteractable->OnRequestDesroyCalled.AddUObject(this, &UCInteractionComponent::RequestDestroy);
			TargetInteractable->OnUpdatePartialAdded.AddUObject(this, &UCInteractionComponent::RequestUpdatePartialAdded);
			TargetInteractable->Interact(playerCharacter);
		}
			
	}
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


