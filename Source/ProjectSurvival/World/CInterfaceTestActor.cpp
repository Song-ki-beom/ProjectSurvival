// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CInterfaceTestActor.h"
#include "Utility/CDebug.h"
// Sets default values
ACInterfaceTestActor::ACInterfaceTestActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);


}

// Called when the game starts or when spawned
void ACInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	
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
	}
}

void ACInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void ACInterfaceTestActor::BeginInteract()
{
	CDebug::Print("상호작용 시작됨");
}

void ACInterfaceTestActor::EndInteract()
{
	CDebug::Print("상호작용 끝남");

}

void ACInterfaceTestActor::Interact()
{
	CDebug::Print("상호작용!!");

}

