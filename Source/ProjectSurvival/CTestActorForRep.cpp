#include "CTestActorForRep.h"
#include "Utility/CDebug.h"
#include "Character/CSurvivorController.h"
#include "Net/UnrealNetwork.h"

ACTestActorForRep::ACTestActorForRep()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("TestActorMesh");
	SetRootComponent(PickupMesh);
	MyGUID = FGuid::NewGuid();
}

void ACTestActorForRep::OnServerSetOwnwer_Implementation(class AController* survivorController)
{
	this->SetOwner(survivorController);

}


void ACTestActorForRep::RequestTestRPC_Implementation()
{
	if (HasAuthority())
	{
		BroadcastTestRPC();
		//ClientTestRPC();
	}
	else
	{
		CDebug::Print(TEXT("This actor does not have authority (client-side)."));
	}
}

void ACTestActorForRep::BeginPlay()
{
	Super::BeginPlay();
	/*if (!HasAuthority())
	{
		RequestTestRPC();
	}*/
	if (!HasAuthority())
	{
		RequestTestRPC();
	}
	
}

void ACTestActorForRep::BroadcastTestRPC_Implementation()
{
	CDebug::Print(MyGUID.ToString());

}

void ACTestActorForRep::ClientTestRPC_Implementation()
{
	CDebug::Print(MyGUID.ToString());

}

void ACTestActorForRep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
	{
		RequestTestRPC();
	}
}

