#include "itemModifier.h"

unsigned int ItemPointerOffset=0;
unsigned int ItemDataLength=0;
unsigned int MedicinePointerOffset=0;
unsigned int MedicineDataLength=0;

#define ITEMIDPOSITION 0
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

u32 getBagStartAddress(ItemBags bag)
{
	u32 address=0;
	switch(bag)
	{
		case itemBag:
			address=ItemPointerOffset;
			break;
		case medicineBag:
			address=MedicinePointerOffset;
			break;
	}
	return address;
}

u32 getItemPossitionAddress(u32 position, ItemBags bag)
{
	u32 itemPointer = getBagStartAddress(bag);
	if(itemPointer>0)
	{
		position--;
		itemPointer+=ItemDataLength*position;
	}
	return itemPointer;
}

// Item modification
void setItemIdAt(u32 position, ItemBags bag, u32 id)
{
	u32 itemPointer=getItemPossitionAddress(position, bag);
	if(itemPointer>0)
	{
		*(vu16*)itemPointer=id;
	}
}

/*Todo get better max value*/
void incrementItemIdAt(u32 position, ItemBags bag, u32 count)
{
	u32 itemPointer=getItemPossitionAddress(position, bag);
	if(itemPointer>0)
	{
		if((*(vu16*)itemPointer+count)<0x3FF)
			*(vu16*)itemPointer+=count;
		else
			*(vu16*)itemPointer=0x3FF;
	}
}

void reduceItemIdAt(u32 position, ItemBags bag, u32 count)
{
	u32 itemPointer=getItemPossitionAddress(position, bag);
	if(itemPointer>0)
	{
		if(count<*(vu16*)itemPointer)
			*(vu16*)itemPointer-=count;
		else
			*(vu16*)itemPointer=1;
	}
}

// Item count modification
u32 getItemCountAddress(u32 position, ItemBags bag)
{
	u32 itemPointer = getItemPossitionAddress(position,bag);
	if(itemPointer>0)
	{
		itemPointer+=ITEMCOUNTPOSITION;
	}
	return itemPointer;
}

void setItemCountAt(u32 position, ItemBags bag, u32 count)
{
	u32 itemCountPointer=getItemCountAddress(position, bag);
	if(itemCountPointer>0)
	{
		*(vu16*)itemCountPointer=count;
	}
}

void addToItemCountAt(u32 position, ItemBags bag, u32 count)
{
	u32 itemCountPointer=getItemCountAddress(position, bag);
	if(itemCountPointer>0)
	{
		if(((u32)*(vu16*)itemCountPointer)+count<0xFFFF)
			*(vu16*)itemCountPointer+=count;
		else
			*(vu16*)itemCountPointer=0xFFFF;
	}
}

void removeFromItemCountAt(u32 position, ItemBags bag, u32 count)
{	
	u32 itemCountPointer=getItemCountAddress(position, bag);
	if(itemCountPointer>0)
	{
		if(*(vu16*)itemCountPointer>count)
			*(vu16*)itemCountPointer-=count;
		else
			*(vu16*)itemCountPointer=1;
	}
}

/* open bag and the item will be deleted*/
void deleteItem(u32 position, ItemBags bag)
{
	u32 itemCountPointer=getItemCountAddress(position, bag);
	if(itemCountPointer>0)
	{
		*(vu16*)itemCountPointer=0;
	}
}