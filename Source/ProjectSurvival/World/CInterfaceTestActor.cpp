// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CInterfaceTestActor.h"
#include "Utility/CDebug.h"
// Sets default values
ACInterfaceTestActor::ACInterfaceTestActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);


}

// Called when the game starts or when spawned
void ACInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	//인터페이스에서의 InteractableData 할당 
	InteractableData = InstanceInteractableData;

}

// Called every frame
void ACInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		//깊이 버퍼에 메쉬를 등록 
		Mesh->SetRenderCustomDepth(true);
		Mesh->bRenderCustomDepth = true;
		Mesh->SetCustomDepthStencilValue(252);

	}

}

void ACInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
		Mesh->SetCustomDepthStencilValue(0);

	}

}

void ACInterfaceTestActor::BeginInteract()
{
	//CDebug::Print("Start InterAction");
}

void ACInterfaceTestActor::EndInteract()
{
	//CDebug::Print("End InterAction");

}

void ACInterfaceTestActor::Interact(class ACSurvivor* PlayerCharacter, bool bIsLongPressed)
{
	//CDebug::Print("Do Interaction!!");

}

