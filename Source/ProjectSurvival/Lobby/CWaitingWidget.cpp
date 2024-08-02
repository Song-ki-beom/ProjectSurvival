// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/CWaitingWidget.h"
#include "Lobby/CLobbySurvivor.h"
#include "Lobby/CDifficultyWidget.h"

UCWaitingWidget::UCWaitingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // 위젯 생성자
{

}



bool UCWaitingWidget::Initialize()
{
	bool Sucess = Super::Initialize();
	return Sucess;
}

void UCWaitingWidget::SetUpWidget()
{
	this->AddToViewport();
	LobbySurvivor = Cast<ACLobbySurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());

}

void UCWaitingWidget::TearDownWidget()
{
	this->RemoveFromViewport();
}
// 커스터마이징 정보 전달하는 코드
