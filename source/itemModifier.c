#include "itemModifier.h"

unsigned int ItemPointerOffset=0;
unsigned int ItemDataLength=0;
unsigned int MedicinePointerOffset=0;
unsigned int MedicineDataLength=0;

#define ITEMIDPOSITION 0
#define ITEMCATALOGPOSITION 1
#define ITEMCOUNTPOSITION 2

void initItemModifier(int edition)
{
	switch (edition)
	{
		//XY
		case PKXY:
		{
			ItemPointerOffset = 0x08C67564;
			ItemDataLength = 0x4;
			MedicinePointerOffset=0x08C67ECC;
			MedicineDataLength=0x4;
		}
		break;

		//ORAS
		case ORAS:
		{
			ItemPointerOffset = 0x08C6EC70;
			ItemDataLength = 4;
			MedicinePointerOffset=0x08C6F5E0;
			MedicineDataLength=0x4;
		}
		break;
	}
}

/*Item*/
u32 getItemCountAddress(u32 position)
{
	position--;
	return ItemPointerOffset+ItemDataLength*position+ITEMCOUNTPOSITION;
}
void setItemCountAt(u32 position, u32 count)
{
	position--;
	*(vu16*)(ItemPointerOffset+ItemDataLength*position+ITEMCOUNTPOSITION)=count;
}

void addToItemCountAt(u32 position, u32 count)
{
	position--;
	*(vu16*)(ItemPointerOffset+ItemDataLength*position+ITEMCOUNTPOSITION)+=count;
}

void removeFromItemCountAt(u32 position, u32 count)
{	
	position--;
	*(vu16*)(ItemPointerOffset+ItemDataLength*position+ITEMCOUNTPOSITION)-=count;
}

/*Medicine*/
u32 getMedicineCountAddress(u32 position)
{
	position--;
	return MedicinePointerOffset+MedicineDataLength*position+ITEMCOUNTPOSITION;
}
void setMedicineCountAt(u32 position, u32 count)
{
	position--;
	*(vu16*)(MedicinePointerOffset+MedicineDataLength*position+ITEMCOUNTPOSITION)=count;
}

void addToMedicineCountAt(u32 position, u32 count)
{
	position--;
	*(vu16*)(MedicinePointerOffset+MedicineDataLength*position+ITEMCOUNTPOSITION)+=count;
}

void removeFromMedicineCountAt(u32 position, u32 count)
{	
	position--;
	*(vu16*)(MedicinePointerOffset+MedicineDataLength*position+ITEMCOUNTPOSITION)-=count;
}