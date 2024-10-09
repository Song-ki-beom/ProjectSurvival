// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CLoadingScreenManager.h"


void ACLoadingScreenManager::BeginPlay()
{
    Super::BeginPlay();

    // 로딩 화면 위젯 생성
    if (LoadingScreenClass)
    {
        LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
        if (LoadingScreenWidget)
        {
            LoadingScreenWidget->AddToViewport();
            // Progress Bar 위젯의 참조 가져오기 (위젯 블루프린트 내의 ProgressBar 요소를 가져옵니다)
            LoadingProgressBar = Cast<UProgressBar>(LoadingScreenWidget->GetWidgetFromName(TEXT("LoadingProgressBar")));
        }
    }

    // 레벨 스트리밍 시작
    StartLevelStreaming(TEXT("MyStreamingLevel"));
}

void ACLoadingScreenManager::StartLevelStreaming(const FString& LevelName)
{
    //FLatentActionInfo LatentInfo;
    //LatentInfo.CallbackTarget = this;
    //LatentInfo.ExecutionFunction = FName("OnLevelStreamLoaded");
    //LatentInfo.Linkage = 0;
    //LatentInfo.UUID = 12345;

    //// 레벨 로딩 시작
    //UGameplayStatics::LoadStreamLevel(this, FName(LevelName), true, false, LatentInfo);

    //// 로딩 상태를 계속 업데이트
    //GetWorld()->GetTimerManager().SetTimerForNextTick([this, LevelName]()
    //    {
    //        const ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), FName(*LevelName));
    //        if (StreamingLevel)
    //        {
    //            float LoadingProgress = StreamingLevel->GetLevelStreamingStatus() == LEVEL_Visible ? 1.0f : StreamingLevel->GetLevelStreamingStatus() / 100.0f;
    //            UpdateLoadingScreen(LoadingProgress);

    //            if (LoadingProgress < 1.0f)
    //            {
    //                // 아직 로딩 중이면 다음 틱에서 다시 업데이트
    //                GetWorld()->GetTimerManager().SetTimerForNextTick([this, LevelName]() { StartLevelStreaming(LevelName); });
    //            }
    //        }
    //    });
}

void ACLoadingScreenManager::UpdateLoadingScreen(float Progress)
{

}
