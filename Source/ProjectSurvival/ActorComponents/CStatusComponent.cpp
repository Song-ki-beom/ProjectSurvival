

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
		DifficultyCoef = gameInstance->GetDifficultyCoeff();
	}
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	if(GetOwner()->HasAuthority()) //서버만 실행 
		GetWorld()->GetTimerManager().SetTimer(StaminaReductionTimerHandle, this, &UCStatusComponent::ReduceStaminaByTime, 5.0f, true); //5초마다 반복해서 실행 
}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


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

}

void UCStatusComponent::BroadcastUpdateStamina_Implementation(float NewStamina)
{
	CurrentStamina = NewStamina;
	OnStaminaUpdated.Broadcast(CurrentStamina / MaxStamina);
};
