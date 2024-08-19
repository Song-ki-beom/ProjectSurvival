

#include "ActorComponents/CInteractionComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

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
	return OwnerCharacter->GetWorldTimerManager().IsTimerActive(TimerHandleInteraction); //상호작용 시작으로부터 타이머 설정 
}

void UCInteractionComponent::DoInteract()
{
	BeginInteract();
}

void UCInteractionComponent::FinishInteract()
{
	EndInteract();
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

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, 1.0f, 0, 3.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			//UInteractionInterface 를 상속받고 있는지 검출(상호작용 가능한 액터인지)
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance) //상호작용 범위면
				{

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
	TargetInteractable->BeginFocus(); //지금 검출해서 찾은 것으로 Focus 시작 




}

void UCInteractionComponent::NoInteractableFound()
{
	if (IsInteracting())
	{
		OwnerCharacter->GetWorldTimerManager().ClearTimer(TimerHandleInteraction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject())) //상호작용한 타겟 액터가 PickUp 되면서 Destroy() 되었는지 체크
		{
			TargetInteractable->EndFocus(); //Destroy 되기 전에 타겟 해제하여 널포인터 참조 방지 
		}

		//상호작용 위젯을 숨김 구현 부분

		//상호작용 관련 액터 초기화 (최근 액터 , 현재 타겟 액터)
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

//상호작용 시작 
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

		//상호작용하려는 액터가 검출되고 이벤트가 일어나기까지 걸리는 시간 체크 
		if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.15f)) 
		{	//짧으면 바로 Interact 
			Interact();
		}
		else
		{
			//Duraction 이후에 실행 
			OwnerCharacter->GetWorldTimerManager().SetTimer(TimerHandleInteraction, this, &UCInteractionComponent::Interact, TargetInteractable->InteractableData.InteractionDuration,false);
		}
	}

}

void UCInteractionComponent::EndInteract()
{
	OwnerCharacter->GetWorldTimerManager().ClearTimer(TimerHandleInteraction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}

}

void UCInteractionComponent::Interact()
{
	OwnerCharacter->GetWorldTimerManager().ClearTimer(TimerHandleInteraction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact();
	}

}




