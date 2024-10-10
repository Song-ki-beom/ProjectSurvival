#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/EngineTypes.h"
#include "Lobby/CLobbyInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Engine/DataTable.h"
#include "CGameInstance.generated.h"




UCLASS()
class PROJECTSURVIVAL_API UCGameInstance : public UGameInstance, public ICLobbyInterface
{
	GENERATED_BODY()

public:
	UCGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	//레벨 블루프린트에서 호출하는 함수
	UFUNCTION(BlueprintCallable)
		void LoadLobbyWidget();

	UFUNCTION()
		void Create(FString InServerName) override;
	UFUNCTION()
		void Join(uint32 InIndex) override;
	UFUNCTION()
		void RenewServerList() override;
	UFUNCTION(BlueprintCallable)
		void RemoveAllWidgets();
	UFUNCTION(BlueprintCallable)
		void CreateChattingBox();
	UFUNCTION(BlueprintCallable)
		void CreateMap();
	UFUNCTION(BlueprintCallable)
		void CreateLoadingScreen();
	UFUNCTION(NetMulticast, Reliable)
		void BroadcastCreateLoadingScreen();


	void SaveCustomizeRowNames(TArray<FName> InRowNames) { CustomizeRowNames = InRowNames; }
	TArray<FName> GetCustomizeRowNames() { return CustomizeRowNames; }

	void SaveSurvivorName(FText InText) { SurvivorName = InText; }
	FText GetLobbySurvivorName() { return SurvivorName; }

	float GetDifficultyCoeff() { return DifficultyCoeff; }
	void SetDifficultyCoeff(float InCoeff);

private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();
	void CheckNetDriver();

	UFUNCTION()
		void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);
	
		void OnLevelAddedToWorld(UWorld* World, ELevelTick TickType, float DeltaSeconds);
		UFUNCTION()
		void OnPreLoadMap(const FString& MapName);
		UFUNCTION()
		void OnPostLoadMapWithWorld(UWorld* InLoadedWorld);
public:
	UPROPERTY()
		FString playerName;
	UPROPERTY()
		FName currentSessionName;
	UPROPERTY()
		UDataTable* DestructibleDataTable;
	UPROPERTY()
		UDataTable* HitDataTable;

private:
	UPROPERTY()
		TSubclassOf<class UUserWidget> LobbyClass;
	UPROPERTY()
		class UCLobbyWidget* LobbyWidget;
	UPROPERTY(VisibleAnywhere)
		class TSubclassOf<class UUserWidget> LoadingScreenWidgetClass;
	UPROPERTY()
		class UCLoadingScreenWidget* LoadingScreenWidget;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	IOnlineIdentityPtr IdentityInterface;
	TSharedPtr<const FUniqueNetId> playerNetId;
	FString DesiredServerName;

	UPROPERTY()
		TArray<FName> CustomizeRowNames;

	UPROPERTY()
		FText SurvivorName;

	UPROPERTY()
		float DifficultyCoeff;
	
public:
	UPROPERTY()
		TSubclassOf<class UUserWidget> ChattingBoxClass;
	UPROPERTY()
		class UCChattingBox* ChattingBox;

	UPROPERTY()
		TSubclassOf<class UCMiniMap> MiniMapClass;
	UPROPERTY()
		class UCMiniMap* MiniMap;

	UPROPERTY()
		TSubclassOf<class UCWorldMap> WorldMapClass;
	UPROPERTY()
		class UCWorldMap* WorldMap;

public:
	FORCEINLINE
		TSharedPtr<const FUniqueNetId> GetLocalPlayerNetId() { return playerNetId; };


};


