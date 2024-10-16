

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

bool UCStatusComponent::IsFatalHealth()
{
	return (CurrentHealth / MaxHealth) <= 0.25;
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
	CurrentStamina = MaxStamina;
	CurrentFriendship = 0;
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
	
	if (OwnerCharacter->Tags.Contains("Player"))
	{
		if (CurrentHunger <= 0)
		{
			TimeSinceStarvation += DeltaTime;



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
					//PlayerController->ClientStartCameraShake(StarveCameraShakeClass, 1.0f);
				}
				// 타이머 리셋
				TimeSinceStarvation = 0.0f;
			}

		}
	}

		
}



bool UCStatusComponent::IsExhausted()
{
	if (bIsExhausted == true) return true;

	if (IsStarving() && IsFatalHealth())
	{
		BroadcastChangeExhausted();
		return true;
	}
	
	return false;
}

void UCStatusComponent::BroadcastChangeExhausted_Implementation()
{
	OnExhaustDetected.Broadcast(true);
	bIsExhausted = true;
}

void UCStatusComponent::BroadcastCancelExhausted_Implementation()
{
	if (bIsExhausted)
		bIsExhausted = false;
}

bool UCStatusComponent::IsFriendly()
{
	
	return bIsFriendly;

}


void UCStatusComponent::StackFriendShip(float StackAmount)
{
	float NewFriendShip = CurrentFriendship + StackAmount*2;
	NewFriendShip = FMath::Clamp(NewFriendShip, 0.0f, MaxFriendShip);
	BroadcastUpdateFriendShip(NewFriendShip);
}

void UCStatusComponent::SetRespawnStatus()
{
	CurrentHealth = MaxHealth;
	CurrentHunger = MaxHunger;
	CurrentStamina = MaxStamina;

	BroadcastUpdateHunger(CurrentHunger);
	BroadcastUpdateHealth(CurrentHunger);
	BroadcastUpdateStamina(CurrentStamina);
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

void UCStatusComponent::ReduceHunger(float DecreaseAmount)
{
	if (CurrentHunger > 0)
	{
		float NewHunger = CurrentHunger - DecreaseAmount;
		NewHunger = FMath::Clamp(NewHunger, 0.0f, MaxHunger);
		BroadcastUpdateHunger(NewHunger);
	}

}

void UCStatusComponent::RecoverHunger(float RecoverAmount)
{
	float NewHunger = CurrentHunger + RecoverAmount;
	NewHunger = FMath::Clamp(NewHunger, 0.0f, MaxHunger);
	BroadcastUpdateHunger(NewHunger);

}

void UCStatusComponent::RecoverHealth(float RecoverAmount)
{
	float NewHealth = CurrentHealth + RecoverAmount;
	NewHealth = FMath::Clamp(NewHealth, 0.0f, MaxHunger);
	BroadcastUpdateHealth(NewHealth);

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
	if (bCoefChanged == true )
	{
		bCoefChanged = false;
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
		bCoefChanged = true;
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
	if ((!bIsExhausted) && (CurrentHunger / MaxHunger <= 0))
		OnStarvationDetected.Broadcast(true);

	OnHungerUpdated.Broadcast(CurrentHunger / MaxHunger);
};


void UCStatusComponent::BroadcastUpdateStamina_Implementation(float NewStamina)
{
	CurrentStamina = NewStamina;
	OnStaminaUpdated.Broadcast(CurrentStamina / MaxStamina);
}

void UCStatusComponent::BroadcastUpdateFriendShip_Implementation(float NewFriendShip)
{
	CurrentFriendship = NewFriendShip;
	
		if (CurrentFriendship >= MaxFriendShip)
		{
			if (OwnerCharacter->HasAuthority())
			{
			RecoverHunger(MaxHunger);
			RecoverHealth(MaxHealth);
			}
			bIsFriendly = true;
			OnBecameFriendly.Broadcast();
		}
	
	OnFriendShipUpdated.Broadcast(CurrentFriendship / MaxFriendShip);

}
