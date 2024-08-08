#include "Character/CSurvivor_BuildTest.h"
///
#include "ActorComponents/CBuildComponent.h"

ACSurvivor_BuildTest::ACSurvivor_BuildTest()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BuildComponent = CreateDefaultSubobject<UCBuildComponent>("BuildComponent");
}

void ACSurvivor_BuildTest::BeginPlay()
{
	Super::BeginPlay();
}

void ACSurvivor_BuildTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACSurvivor_BuildTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
