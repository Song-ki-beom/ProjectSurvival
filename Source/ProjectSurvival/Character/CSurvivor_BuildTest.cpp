//#include "Character/CSurvivor_BuildTest.h"
/////
//#include "ActorComponents/CBuildComponent.h"
//
//
//ACSurvivor_BuildTest::ACSurvivor_BuildTest()
//{
//	PrimaryActorTick.bCanEverTick = true;
//
//	bReplicates = true;
//
//	BuildComponent = CreateDefaultSubobject<UCBuildComponent>("BuildComponent");
//}
//
//void ACSurvivor_BuildTest::BeginPlay()
//{
//	Super::BeginPlay();
//}
//
//void ACSurvivor_BuildTest::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}
//
//void ACSurvivor_BuildTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}
//
//void ACSurvivor_BuildTest::SelectStructure(ESelectedStructure InKey, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
//{
//	switch (InKey)
//	{
//	case ESelectedStructure::Q:
//	{
//		BuildComponent->SelectQ(InClass, InElement);
//		break;
//	}
//
//	case ESelectedStructure::W:
//	{
//		BuildComponent->SelectW();
//		break;
//	}
//
//	case ESelectedStructure::E:
//	{
//		BuildComponent->SelectE();
//		break;
//	}
//
//	case ESelectedStructure::A:
//	{
//		BuildComponent->SelectA();
//		break;
//	}
//
//	case ESelectedStructure::S:
//	{
//		BuildComponent->SelectS();
//		break;
//	}
//
//	case ESelectedStructure::D:
//	{
//		BuildComponent->SelectD();
//		break;
//	}
//
//	case ESelectedStructure::Z:
//	{
//		BuildComponent->SelectZ();
//		break;
//	}
//
//	case ESelectedStructure::X:
//	{
//		BuildComponent->SelectX();
//		break;
//	}
//
//	case ESelectedStructure::C:
//	{
//		BuildComponent->SelectC();
//		break;
//	}
//	}
//}
