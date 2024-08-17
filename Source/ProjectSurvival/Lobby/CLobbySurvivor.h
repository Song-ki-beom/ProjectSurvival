#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "CLobbySurvivor.generated.h"

/**
* SetSurvivorName(const FText& InText) - CCustomizeWidget에서 호출됨, 서버인 경우 직접 설정, 클라인 경우 설정 요청
* PerformSetSurvivorName(const FText& InText) - 리플리케이션 변수 변경, 위치 변경, 위젯 업데이트 호출
* RequestSetSurvivorName(const FText& InText) - 클라에서 호출해서 서버에 요청, 서버에서 RequestSetSurvivorName_Implementation 찾아 실행
* OnRep_ReplicatedSurvivorName() - ReplicatedSurvivorName 변수가 바뀔때 호출되는 함수
*/

USTRUCT(BlueprintType)
struct FSkeletalHeadMeshRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* HeadMesh;
};

USTRUCT(BlueprintType)
struct FSkeletalHeadMeshColorRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor HeadMeshColor;
};

USTRUCT(BlueprintType)
struct FSkeletalPantsMeshRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* PantsMesh;
};

USTRUCT(BlueprintType)
struct FSkeletalBootsMeshRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* BootsMesh;
};

USTRUCT(BlueprintType)
struct FSkeletalSkinMeshColorRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor SkinMeshColor;
};

USTRUCT (BlueprintType)
struct FSkeletalSingleMeshRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* AccessoryMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* HandsMesh;
};

UCLASS()
class PROJECTSURVIVAL_API ACLobbySurvivor : public ACharacter
{
	GENERATED_BODY()

public:
	ACLobbySurvivor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void InitCustomize();
	void SetLocalValue();
	void SetSurvivorName(const FText& InText);
	void PerformSetSurvivorName(const FText& InText);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSurvivorName(const FText& InText);
	void UpdateSurvivorNameWidget();

	int32 GetHairRowNumber() { return ValidHairRowNumber; }
	int32 GetHairColorRowNumber() { return ValidHairColorRowNumber; }
	int32 GetPantsRowNumber() { return ValidPantsRowNumber; }
	int32 GetBootsRowNumber() { return ValidBootsRowNumber; }
	int32 GetSkinColorRowNumber() { return ValidSkinColorRowNumber; }

	UFUNCTION()
		void SetHeadMesh(int32 InIndex);
	UFUNCTION()
		void SetHeadMeshColor(int32 InIndex);
	UFUNCTION()
		void SetPantsMesh(int32 InIndex);
	UFUNCTION()
		void SetBootsMesh(int32 InIndex);
	UFUNCTION()
		void SetSkinColor(int32 InIndex);


	// 클라이언트 입장 시
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestReady();

	UFUNCTION(NetMulticast, Reliable)
		void BroadcastSetText(int InIndex);

	TArray<FName> GetCustomizeRowNames();

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);

	// 머리 리플리케이트
	void PerformSetHeadMesh(int32 InIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetHeadMesh(int32 InIndex);
	UFUNCTION()
		void UpdateSkeletalHeadMesh();
	UFUNCTION()
		void OnRep_ReplicatedHeadName();

	// 머리색 리플리케이트
	void PerformSetHeadMeshColor(int32 InIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetHeadMeshColor(int32 InIndex);
	UFUNCTION()
		void UpdateSkeletalHeadMeshColor();
	UFUNCTION()
		void OnRep_ReplicatedHeadColorName();

	// 바지 리플리케이트
	void PerformSetPantsMesh(int32 InIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetPantsMesh(int32 InIndex);
	UFUNCTION()
		void UpdateSkeletalPantsMesh();
	UFUNCTION()
		void OnRep_ReplicatedPantsName();

	// 신발 리플리케이트
	void PerformSetBootsMesh(int32 InIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetBootsMesh(int32 InIndex);
	UFUNCTION()
		void UpdateSkeletalBootsMesh();
	UFUNCTION()
		void OnRep_ReplicatedBootsName();

	// 피부색 리플리케이트
	void PerformSetSkinColor(int32 InIndex);
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestSetSkinColor(int32 InIndex);
	UFUNCTION()
		void UpdateSkinColor();
	UFUNCTION()
		void OnRep_ReplicatedSkinColorName();

	UFUNCTION()
		void OnRep_ReplicatedSurvivorName();

	// 클라이언트 입장 시 난이도 선택된 것 유지
	UFUNCTION(Server, Reliable, WithValidation)
		void RequestDifficultyUpdate();
	UFUNCTION()
		void ClientDifficultyUpdate();

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	// 이름 위젯
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class UUserWidget> SurvivorNameClass;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* SurvivorNameWidgetComponent;

	FVector OriginalSpringArmLocation;

	// 커스터마이징 관련
	class UDataTable* CustomizeHeadData;
	class UDataTable* CustomizeHeadColorData;
	class UDataTable* CustomizePantsData;
	class UDataTable* CustomizePantsColorData;
	class UDataTable* CustomizeBootsData;
	class UDataTable* CustomizeSkinColorData;
	class UDataTable* CustomizeSingleData;

	UPROPERTY()
		int32 ValidHairRowNumber;
	UPROPERTY()
		int32 ValidHairColorRowNumber;
	UPROPERTY()
		int32 ValidPantsRowNumber;
	UPROPERTY()
		int32 ValidBootsRowNumber;
	UPROPERTY()
		int32 ValidSkinColorRowNumber;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedHeadName)
		FName ReplicatedHeadName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedHeadColorName)
		FName ReplicatedHeadColorName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedPantsName)
		FName ReplicatedPantsName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedBootsName)
		FName ReplicatedBootsName;
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSkinColorName)
		FName ReplicatedSkinColorName;
	
	UPROPERTY()
		class USkeletalMeshComponent* Head;
	UPROPERTY()
		class USkeletalMeshComponent* Pants;
	UPROPERTY()
		class USkeletalMeshComponent* Boots;
	UPROPERTY()
		class USkeletalMeshComponent* Accessory;
	UPROPERTY()
		class USkeletalMeshComponent* Body;
	UPROPERTY()
		class USkeletalMeshComponent* Hands;

	UMaterialInterface* MeshColorMaterial;
	UMaterialInstanceDynamic* DynamicHeadMeshColorMaterial;
	UMaterialInstanceDynamic* DynamicBootsMeshColorMaterial;
	UMaterialInstanceDynamic* DynamicPantsMeshColorMaterial;
	UMaterialInstanceDynamic* DynamicBodyMeshColorMaterial;
	UMaterialInstanceDynamic* DynamicHandsMeshColorMaterial;

	TArray<FName> CustomizeRowNames;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSurvivorName)
		FText ReplicatedSurvivorName;
};
