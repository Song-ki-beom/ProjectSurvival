#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"



UCLASS()
class PROJECTSURVIVAL_API ACAttachment : public AActor
{
	GENERATED_BODY()

public:
	ACAttachment();
protected:
	virtual void BeginPlay() override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class USceneComponent* Root;
	

public: 
	UFUNCTION(BlueprintNativeEvent)
		void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() ;
	UFUNCTION(BlueprintNativeEvent)
		void OnEndEquip();
	virtual void OnEndEquip_Implementation() {};
	UFUNCTION(BlueprintNativeEvent)
		void OnUnEquip();
	virtual void OnUnEquip_Implementation() ;

public:
	UFUNCTION(BlueprintCallable, Category = "Attach")
		void AttachTo(FName InSocketName);

	class UBoxComponent* GetHitBox() { return HitBox; }

protected: //Collision & Character
	UPROPERTY(EditAnywhere)
		class UBoxComponent* HitBox;
	UPROPERTY(BlueprintReadOnly, Category = "Game")
		class ACharacter* OwnerCharacter;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Game")
		class UStaticMeshComponent* Mesh;
};
