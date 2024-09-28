

#include "ActorComponents/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "CGameInstance.h"


UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
	CurrentStamina = MaxStamina;
	if (GetOwner()->HasAuthority()) //서버만 실행 
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaReductionTimerHandle, this, &UCStatusComponent::ReduceStaminaByTime, 1.0f, true); //1초마다 반복해서 실행 
	}
		
}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentStamina <= 0)
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
				PlayerController->ClientStartCameraShake(StarveCameraShakeClass, 1.0f);
			}
			// 타이머 리셋
			TimeSinceStarvation = 0.0f;
		}
	}
		
}



void UCStatusComponent::ReduceStaminaByTime()
{
	if (CurrentStamina > 0)
	{
		float NewStamina = CurrentStamina - (StaminaDecreaseAmount* DifficultyCoef);
		NewStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina); 
		BroadcastUpdateStamina(NewStamina);
	}

}

void UCStatusComponent::ApplyDamage(float InAmount)
{
	float NewHealth = CurrentHealth -(InAmount*DifficultyCoef);
	NewHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	BroadcastUpdateHealth(NewHealth);
	
	
}

void UCStatusComponent::BroadcastUpdateHealth_Implementation(float NewHealth)
{
	CurrentHealth = NewHealth;
	OnHealthUpdated.Broadcast(CurrentHealth / MaxHealth);
	if (CurrentHealth / MaxHealth < 0.45f)
	{
		OnLowHealthDetected.Broadcast();
	}
}

void UCStatusComponent::BroadcastUpdateStamina_Implementation(float NewStamina)
{
	CurrentStamina = NewStamina;
	OnStaminaUpdated.Broadcast(CurrentStamina / MaxStamina);
};
