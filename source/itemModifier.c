#include "itemModifier.h"

unsigned int ItemPointerOffset=0;
unsigned int ItemDataLength=0;
unsigned int MedicinePointerOffset=0;
unsigned int MedicineDataLength=0;

void initItemModifier(int edition)
{
	switch (edition)
	{
		//XY
		case PKXY:
		{
			ItemPointerOffset = 0x08C67566;
			ItemDataLength = 0x4;
			MedicinePointerOffset=0x08C67ECE;
			MedicineDataLength=0x4;
		}
		break;

		//ORAS
		case ORAS:
		{
			ItemPointerOffset = 0x08C6EC72;
			ItemDataLength = 0x4;
			MedicinePointerOffset=0x08C6F5E2;
			MedicineDataLength=0x4;
		}
		break;
	}
}

/*Item*/
u32 getItemCountAddress(u32 position)
{
	position--;
	return ItemPointerOffset+ItemDataLength*position;
}
void setItemCountAt(u32 position, u32 count)
{
	position--;
	(*(vu16*)(ItemPointerOffset+ItemDataLength*position))=count;
}

void addToItemCountAt(u32 position, u32 count)
{
	position--;
	(*(vu16*)(ItemPointerOffset+ItemDataLength*position))+=count;
}

void removeFromItemCountAt(u32 position, u32 count)
{	
	position--;
	(*(vu16*)(ItemPointerOffset+ItemDataLength*position))-=count;
}

/*Medicine*/
u32 getMedicineCountAddress(u32 position)
{
	position--;
	return MedicinePointerOffset+MedicineDataLength*position;
}
void setMedicineCountAt(u32 position, u32 count)
{
	position--;
	(*(vu16*)(MedicinePointerOffset+MedicineDataLength*position))=count;
}

void addToMedicineCountAt(u32 position, u32 count)
{
	position--;
	(*(vu16*)(MedicinePointerOffset+MedicineDataLength*position))+=count;
}

void removeFromMedicineCountAt(u32 position, u32 count)
{	
	position--;
	(*(vu16*)(MedicinePointerOffset+MedicineDataLength*position))-=count;
}