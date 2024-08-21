#include "Build/CStructure_Ceiling.h"

ACStructure_Ceiling::ACStructure_Ceiling()
{
	PreviewBox = CreateDefaultSubobject<UBoxComponent>("PrivewBox");
	ForwardBox = CreateDefaultSubobject<UBoxComponent>("ForwardBox");
	BackwardBox = CreateDefaultSubobject<UBoxComponent>("BackwardBox");
	LeftBox = CreateDefaultSubobject<UBoxComponent>("LeftBox");
	RightBox = CreateDefaultSubobject<UBoxComponent>("RightBox");

	PreviewBox->SetupAttachment(StaticMesh);
	ForwardBox->SetupAttachment(StaticMesh);
	BackwardBox->SetupAttachment(StaticMesh);
	LeftBox->SetupAttachment(StaticMesh);
	RightBox->SetupAttachment(StaticMesh);
}

void ACStructure_Ceiling::CheckCenter()
{
}

void ACStructure_Ceiling::CheckDown_Forward()
{
}

void ACStructure_Ceiling::CheckDown_Backward()
{
}

void ACStructure_Ceiling::CheckDown_Left()
{
}

void ACStructure_Ceiling::CheckDown_Right()
{
}
