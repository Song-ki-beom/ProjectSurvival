// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "ActorComponents/CStateComponent.h"
#include "CMontageComponent.generated.h"



USTRUCT()
struct FMontagesData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStateType Type;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSURVIVAL_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontageComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable;

	UFUNCTION()
		void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);


public:
	void  PlayAnimMontage(EStateType InType);
	void  PlayDeadMontage();
	void  Montage_Play(UAnimMontage* InMontage, float InPlayRate);
	void  Montage_Play_Section(UAnimMontage* InMontage, FName SectionName);


private:
	FMontagesData* MontageDatas[(int32)EStateType::Max];  

private:
	class ACharacter* OwnerCharacter;              //������Ʈ ������ Parrent�� ����
};
