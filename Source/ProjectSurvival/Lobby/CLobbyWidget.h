#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lobby/CLobbyInterface.h"
#include "CLobbyWidget.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	FString HostUserName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
};

UCLASS()
class PROJECTSURVIVAL_API UCLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCLobbyWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

public:
	void SetUpWidget();
	void TearDownWidget();
	void SetLobbyInterface(ICLobbyInterface* InLobbyInterface);
	void SetServerList(TArray<FServerData> InServerName);
	void SelectIndex(uint32 InIndex);

private:
	void UpdateChildren();
	//버튼에 바인드시킬 함수들
	UFUNCTION()
		void SetHostServerName();
	UFUNCTION()
		void CreateServer();
	UFUNCTION()
		void FindServer();
	UFUNCTION()
		void ExitGame();
	UFUNCTION()
		void JoinServer();
	UFUNCTION()
		void RenewServer();
	UFUNCTION()
		void Cancle();

private:
	//버튼 멤버 변수명은 블루프린트 위젯에 있는 버튼 이름과 같아야함. 이름이 같으면 meta = (BindWidget)를 통해 바인드 됨
	UPROPERTY(meta = (BindWidget))
		class UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* FindButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* RenewButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinCancleButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CreateCancleButton;
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu; //위젯 스위치를 위한 자료형으로 UWidget이 가장 무난하다. (한 위젯화면의 최상위 변수로, 이름 같아야함)
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* CreateMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher; //한 위젯 안에서 화면 전환할 때 필요함 (슬라이드 쇼의 슬라이드 바꾸기 같은 개념)
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* HostServerName;
	UPROPERTY()
		class UCServerRow* ServerRow;
	UPROPERTY()
		TSubclassOf<class UUserWidget> ServerRowClass;

	ICLobbyInterface* LobbyInterface;
	TOptional<uint32> SelectedIndex;
};

