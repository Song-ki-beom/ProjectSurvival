

#include "ActorComponents/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"
#include "CGameInstance.h"


UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


bool UCStatusComponent::IsStarving()
{
	return (CurrentHunger / MaxHunger) <= 0;
}


void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	if (gameInstance != nullptr)
	{
		DifficultyCoef = FMath::Clamp(gameInstance->GetDifficultyCoeff(),0.75f,5.0f);
	}
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CurrentHealth = MaxHealth;
	CurrentHunger = MaxHunger;
	if (GetOwner()->HasAuthority()) //서버만 실행 
	{
		GetWorld()->GetTimerManager().SetTimer(HungerReductionTimerHandle, this, &UCStatusComponent::ReduceHungerByTime, 1.0f, true); //1초마다 반복해서 실행 
		if (OwnerCharacter->Tags.Contains("Player"))
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRecoverTimerHandle, this, &UCStatusComponent::RecoverStaminaByTime, 0.35f, true); //0.35초마다 반복해서 실행 
		}
		
	}
		
}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentHunger <= 0)
	{
		TimeSinceStarvation += DeltaTime;


		if (OwnerCharacter->Tags.Contains("Player"))
		{
			// 5초가 지났을 때마다 Starvation데미지 적용
			if (TimeSinceStarvation >= 5.0f)
			{
				if (GetOwner()->HasAuthority())
				{
					ApplyDamage(7.0f);
				}
				APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
				if (PlayerController && PlayerController->IsLocalController())
				{
					PlayerController->ClientStartCameraShake(StarveCameraShakeClass, 1.0f);
				}
				// 타이머 리셋
				TimeSinceStarvation = 0.0f;
			}
		}
	}
		
}

void UCStatusComponent::RecoverHunger(float RecoverAmount)
{
	float NewHunger = CurrentHunger + RecoverAmount;
	NewHunger = FMath::Clamp(NewHunger, 0.0f, MaxHunger);
}



void UCStatusComponent::ReduceHungerByTime()
{
	if (CurrentHunger > 0)
	{
		float NewHunger = CurrentHunger - (HungerDecreaseAmount * DifficultyCoef);
		NewHunger = FMath::Clamp(NewHunger, 0.0f, MaxHunger);
		BroadcastUpdateHunger(NewHunger);
	}

}

void UCStatusComponent::RecoverStaminaByTime()
{
	if (CurrentStamina >= 0)
	{
		float NewStamina = CurrentStamina + StaminaIncreaseAmount;
		NewStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
		BroadcastUpdateStamina(NewStamina);
	}

}

void UCStatusComponent::ReduceStamina(float ReduceAmount)
{
	if (CurrentStamina > 0)
	{
		float NewStamina = CurrentStamina - ReduceAmount;
		NewStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
		BroadcastUpdateStamina(NewStamina);
	}

}

bool UCStatusComponent::CanSpendStamina(float ReduceAmount)
{
	if (CurrentStamina - ReduceAmount >=0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UCStatusComponent::SuspendStaminaRecover()
{
	GetWorld()->GetTimerManager().PauseTimer(StaminaRecoverTimerHandle);
}

void UCStatusComponent::ProceedStaminaRecover()
{
	GetWorld()->GetTimerManager().UnPauseTimer(StaminaRecoverTimerHandle);
}

void UCStatusComponent::ApplyDamage(float InAmount)
{
	float NewHealth = CurrentHealth -(InAmount*DifficultyCoef);
	NewHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	BroadcastUpdateHealth(NewHealth);
	
	
}

bool UCStatusComponent::CheckHPCoefChanged()
{
	if (CoefChanged == true )
	{
		CoefChanged = false;
		return true;
	}
	return false;
}

void UCStatusComponent::BroadcastUpdateHealth_Implementation(float NewHealth)
{
	CurrentHealth = NewHealth;

	float NewDamagedHPCoef = (float)(1 - (CurrentHealth / MaxHealth));
	int32 NewDamagedHealthCoef = NewDamagedHPCoef / 0.1f; //Default:0.25f
	if (DamagedHealthCoef != NewDamagedHealthCoef)
	{
		DamagedHealthCoef = NewDamagedHealthCoef;
		CoefChanged = true;
	}
	

	OnHealthUpdated.Broadcast(CurrentHealth / MaxHealth);
	if (CurrentHealth / MaxHealth < 0.45f)
	{
		OnLowHealthDetected.Broadcast();
	}
}

void UCStatusComponent::BroadcastUpdateHunger_Implementation(float NewHunger)
{
	CurrentHunger = NewHunger;
	OnHungerUpdated.Broadcast(CurrentHunger / MaxHunger);
};


void UCStatusComponent::BroadcastUpdateStamina_Implementation(float NewStamina)
{
	CurrentStamina = NewStamina;
	OnStaminaUpdated.Broadcast(CurrentStamina / MaxStamina);
}