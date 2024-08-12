#include "Widget/Build/CBuildWidget.h"
#include "Components/Image.h"

void UCBuildWidget::SaveStructureInfo(ESelectedStructure InKey, UTexture2D* InTexture, TSubclassOf<ACStructure> InClass, EBuildStructureElement InElement)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		StructureImageQ->SetBrushFromTexture(InTexture);
		StructureQ = InClass;
		ElementQ = InElement;
		break;
	}
	case ESelectedStructure::W:
	{
		StructureImageW->SetBrushFromTexture(InTexture);
		StructureW = InClass;
		ElementW = InElement;
		break;
	}
	case ESelectedStructure::E:
	{
		StructureImageE->SetBrushFromTexture(InTexture);
		StructureE = InClass;
		ElementE = InElement;
		break;
	}
	case ESelectedStructure::A:
	{
		StructureImageA->SetBrushFromTexture(InTexture);
		StructureA = InClass;
		ElementA = InElement;
		break;
	}
	case ESelectedStructure::S:
	{
		StructureImageS->SetBrushFromTexture(InTexture);
		StructureS = InClass;;
		ElementS = InElement;
		break;
	}
	case ESelectedStructure::D:
	{
		StructureImageD->SetBrushFromTexture(InTexture);
		StructureD = InClass;
		ElementD = InElement;
		break;
	}
	case ESelectedStructure::Z:
	{
		StructureImageZ->SetBrushFromTexture(InTexture);
		StructureZ = InClass;
		ElementZ = InElement;
		break;
	}
	case ESelectedStructure::X:
	{
		StructureImageX->SetBrushFromTexture(InTexture);
		StructureX = InClass;
		ElementX = InElement;
		break;
	}
	case ESelectedStructure::C:
	{
		StructureImageC->SetBrushFromTexture(InTexture);
		StructureC = InClass;
		ElementC = InElement;
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

EBuildStructureElement UCBuildWidget::GetStructureElement(ESelectedStructure InKey)
{
	switch (InKey)
	{
	case ESelectedStructure::Q:
	{
		if (IsValid(StructureQ))
			return ElementQ;
		break;
	}
	case ESelectedStructure::W:
	{
		if (IsValid(StructureW))
			return ElementW;
		break;
	}
	case ESelectedStructure::E:
	{
		if (IsValid(StructureE))
			return ElementE;
		break;
	}
	case ESelectedStructure::A:
	{
		if (IsValid(StructureA))
			return ElementA;
		break;
	}
	case ESelectedStructure::S:
	{
		if (IsValid(StructureS))
			return ElementS;
		break;
	}
	case ESelectedStructure::D:
	{
		if (IsValid(StructureD))
			return ElementD;
		break;
	}
	case ESelectedStructure::Z:
	{
		if (IsValid(StructureZ))
			return ElementZ;
		break;
	}
	case ESelectedStructure::X:
	{
		if (IsValid(StructureX))
			return ElementX;
		break;
	}
	case ESelectedStructure::C:
	{
		if (IsValid(StructureC))
			return ElementC;
		break;
	}
	}
	return EBuildStructureElement::None;
}
