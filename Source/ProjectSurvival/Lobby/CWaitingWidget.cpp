// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/CWaitingWidget.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CDifficultyWidget.h"
#include "Lobby/CLobbyGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CDebug.h"

UCWaitingWidget::UCWaitingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // 위젯 생성자
{

}

bool UCWaitingWidget::Initialize()
{
	bool Sucess = Super::Initialize();

	return Sucess;
}

void UCWaitingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(LobbyGameMode))
	{
		if (LobbySurvivor)
		{
			if (DifficultyWidget)
			{
				if (LobbySurvivor->HasAuthority())
				{
					if (LobbyGameMode->CheckPlayer())
					{
						DifficultyWidget->ActivateStartButton();
					}
					else
					{
						DifficultyWidget->DeactivateStartButton();
					}
				}
			}
		}
	}

}

void UCWaitingWidget::SetUpWidget()
{
	this->AddToViewport();
	LobbySurvivor = Cast<ACLobbySurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (LobbySurvivor->HasAuthority())
	{
		LobbyGameMode = Cast<ACLobbyGameMode>(GetWorld()->GetAuthGameMode());
	}
	else
	{
		APlayerController* controller = Cast<APlayerController>(LobbySurvivor->GetController());
		controller->bShowMouseCursor = false;
		DifficultyWidget->SetClientStartButton();
	}
	UpdateDifficultyWidget(0);
}

void UCWaitingWidget::UpdateDifficultyWidget(int32 InIndex)
{
	if (IsValid(DifficultyWidget))
	{
		DifficultyWidget->UpdateDifficultyInfo(InIndex);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("DifficultyWidget is not valid - UCWaitingWidget"));
	}
}

void UCWaitingWidget::UpdateClientDifficulty()
{
	DifficultyWidget->UpdateClientDifficulty();
}
