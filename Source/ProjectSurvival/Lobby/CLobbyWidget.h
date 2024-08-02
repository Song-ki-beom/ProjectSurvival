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

private:
	//��ư ��� �������� �������Ʈ ������ �ִ� ��ư �̸��� ���ƾ���. �̸��� ������ meta = (BindWidget)�� ���� ���ε� ��
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
		class UWidget* MainMenu; //���� ����ġ�� ���� �ڷ������� UWidget�� ���� �����ϴ�. (�� ����ȭ���� �ֻ��� ������, �̸� ���ƾ���)
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* CreateMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher; //�� ���� �ȿ��� ȭ�� ��ȯ�� �� �ʿ��� (�����̵� ���� �����̵� �ٲٱ� ���� ����)
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* HostServerName;
	UPROPERTY()
		class UCServerRow* ServerRow;
	UPROPERTY()
		TSubclassOf<class UUserWidget> ServerRowClass;

	//��ư�� ���ε��ų �Լ���
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


	ICLobbyInterface* LobbyInterface;
	TOptional<uint32> SelectedIndex;
};

