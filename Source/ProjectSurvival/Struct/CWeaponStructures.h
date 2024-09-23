
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CWeaponStructures.generated.h"


UENUM()
enum class EHitActorType : uint8
{
	Survivor UMETA(DisplayName = "Survivor"),
	Bear UMETA(DisplayName = "Bear"),
	MAX UMETA(DisplayName = "MAX")
};

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float  PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName ActionID;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float  PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bFixedCamera = true;
	// 파티클 or 나이아가라
	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;
	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void  DoAction(class ACharacter* InOwner);

};

USTRUCT()
struct FDamageData 
{
	GENERATED_BODY()
public:
	class ACharacter* Character;
	class AActor* Causer;
	FName HitID;
};

USTRUCT()
struct FHitData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName ID;
	UPROPERTY(EditAnywhere)
		EHitActorType HitActorType;
	UPROPERTY(EditAnywhere)
		float DamageAmount;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		float Launch = 100.0f;
	UPROPERTY(EditAnywhere)
		float StopTime = 0.0f;    // Hit Stop
	UPROPERTY(EditAnywhere)
		class USoundWave* Sound;
	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect;
	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	void PlayMontage(class ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation, USkeletalMeshComponent* InMesh = nullptr, FName InSocketName = NAME_None);

public:
	TArray<APawn*> StopPawns;
};




USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:  
	FName HitID;
};




UCLASS()
class PROJECTSURVIVAL_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
};
