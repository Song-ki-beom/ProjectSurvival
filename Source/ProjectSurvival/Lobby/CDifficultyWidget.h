#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CDifficultyWidget.generated.h"

/**
* 이 위젯은 CWaitingWidget에 바인드 되어 출력된다.
* FDifficultyInfo - 난이도 버튼 클릭마다 변경할 텍스트들을 담고있는 구조체, 데이터 테이블로 관리
* ActivateStartButton() - 서버의 StartButton을 활성화,CWaitingWidget에서 틱 호출
* DeactivateStartButton() - 서버의 StartButton을 비활성화,CWaitingWidget에서 틱 호출
* SetClientStartButton() - 클라이언트의 StartButton비활성화,CWaitingWidget에서 한번만 호출
*/

USTRUCT(BlueprintType)
struct FDifficultyInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* DifficultyTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultySummary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText DifficultyDetail_4;
};

UCLASS()
class PROJECTSURVIVAL_API UCDifficultyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	void ActivateStartButton();
	void DeactivateStartButton();
	void SetClientStartButton();
	void UpdateDifficultyInfo(int InIndex);
	void UpdateClientDifficulty();

private:
	UFUNCTION()
		void OnEasy();
	UFUNCTION()
		void OnNormal();
	UFUNCTION()
		void OnHard();
	UFUNCTION()
		void OnExtreme();
	UFUNCTION()
		void OnStart();

private:
	UPROPERTY(meta = (BindWidget))
		class UImage* DifficultyImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_1;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_3;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DifficultyDesc_4;

	UPROPERTY(meta = (BindWidget))
		class UButton* EasyButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* NormalButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* HardButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ExtremeButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* StartSwitcher;

	class UDataTable* DifficultyData;
	int32 ServerDifficultyIndex = 0;
};

