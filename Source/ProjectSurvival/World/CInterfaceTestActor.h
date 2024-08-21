// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractionInterface.h"
#include "CInterfaceTestActor.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACInterfaceTestActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus()override;
	virtual void BeginInteract() override; //인터렉션까지의 타이머 설정
	virtual void EndInteract() override;
	virtual void Interact(class ACSurvivor* PlayerCharacter) override; // 인터렉트가 준비되면 바로 Interact 시작


protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interface Actor")
		class UStaticMeshComponent* Mesh;

	//상위 인터페이스 정의 부분에 있는 FInteractableData InteractableData 에  할당할 변수 , 에디터에서 InstanceInteractableData를 세팅하고 BeginPlay에서  InteractableData에 재할당
	UPROPERTY(EditInstanceOnly,Category = "Interface Actor")
	FInteractableData InstanceInteractableData; 


};
