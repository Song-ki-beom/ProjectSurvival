#include "Widget/Build/CBuildWidget.h"
#include "Components/Image.h"

void UCBuildWidget::SaveStructureInfo(ESelectedStructure InKey, UTexture2D* InTexture, TSubclassOf<ACStructure> InClass)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		StructureImageQ->SetBrushFromTexture(InTexture);
		StructureQ = InClass;
		break;
	}
	case ESelectedStructure::W:
	{
		StructureImageW->SetBrushFromTexture(InTexture);
		StructureW = InClass;
		break;
	}
	case ESelectedStructure::E:
	{
		StructureImageE->SetBrushFromTexture(InTexture);
		StructureE = InClass;
		break;
	}
	case ESelectedStructure::A:
	{
		StructureImageA->SetBrushFromTexture(InTexture);
		StructureA = InClass;
		break;
	}
	case ESelectedStructure::S:
	{
		StructureImageS->SetBrushFromTexture(InTexture);
		StructureS = InClass;;
		break;
	}
	case ESelectedStructure::D:
	{
		StructureImageD->SetBrushFromTexture(InTexture);
		StructureD = InClass;
		break;
	}
	case ESelectedStructure::Z:
	{
		StructureImageZ->SetBrushFromTexture(InTexture);
		StructureZ = InClass;
		break;
	}
	case ESelectedStructure::X:
	{
		StructureImageX->SetBrushFromTexture(InTexture);
		StructureX = InClass;
		break;
	}
	case ESelectedStructure::C:
	{
		StructureImageC->SetBrushFromTexture(InTexture);
		StructureC = InClass;
		break;
	}
	}
}

TSubclassOf<ACStructure> UCBuildWidget::GetStructureClass(ESelectedStructure InKey)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		if (IsValid(StructureQ))
			return StructureQ;
		break;
	}
	case ESelectedStructure::W:
	{
		if (IsValid(StructureW))
			return StructureW;
		break;
	}
	case ESelectedStructure::E:
	{
		if (IsValid(StructureE))
			return StructureE;
		break;
	}
	case ESelectedStructure::A:
	{
		if (IsValid(StructureA))
			return StructureA;
		break;
	}
	case ESelectedStructure::S:
	{
		if (IsValid(StructureS))
			return StructureS;
		break;
	}
	case ESelectedStructure::D:
	{
		if (IsValid(StructureD))
			return StructureD;
		break;
	}
	case ESelectedStructure::Z:
	{
		if (IsValid(StructureZ))
			return StructureZ;
		break;
	}
	case ESelectedStructure::X:
	{
		if (IsValid(StructureX))
			return StructureX;
		break;
	}
	case ESelectedStructure::C:
	{
		if (IsValid(StructureC))
			return StructureC;
		break;
	}
	}
	return nullptr;
}
