//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "CActorInventoryComponent.generated.h"
//
////DECLARE_MULTICAST_DELEGATE(FOnActorInventoryUpdated);
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class PROJECTSURVIVAL_API UCActorInventoryComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	UCActorInventoryComponent();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:
//	//FOnActorInventoryUpdated OnActorInventoryUpdated;
//
////private:
////	void PerformAddItem(class UCItemBase* InItem, const int32 AmountToAdd);
////	UFUNCTION(Server, Reliable)
////		void RequestAddItem(class UCItemBase* InItem, const int32 AmountToAdd);
//
////private:
////	UPROPERTY()
////		class AActor* OwnerActor;
////
////	UPROPERTY()
////		TArray<TWeakObjectPtr<UCItemBase>> ActorInventoryContents;
//};
