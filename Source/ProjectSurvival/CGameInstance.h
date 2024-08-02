#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Lobby/CLobbyInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "CGameInstance.generated.h"

UCLASS()
class PROJECTSURVIVAL_API UCGameInstance : public UGameInstance, public ICLobbyInterface
{
	GENERATED_BODY()

public:
	UCGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	//���� �������Ʈ���� ȣ���ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
		void LoadLobbyWidget();

	UFUNCTION()
		void Create(FString InServerName) override;
	UFUNCTION()
		void Join(uint32 InIndex) override;
	UFUNCTION()
		void RenewServerList() override;

	UPROPERTY()
		FString playerName;
	UPROPERTY()
		FName currentSessionName;

private:
	UPROPERTY()
		TSubclassOf<class UUserWidget> LobbyClass;
	UPROPERTY()
		class UCLobbyWidget* LobbyWidget;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	IOnlineIdentityPtr IdentityInterface;
	TSharedPtr<const FUniqueNetId> playerNetId;
	
private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();
	void CheckNetDriver();

	FString DesiredServerName;
};
