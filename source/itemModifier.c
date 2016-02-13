#include "itemModifier.h"

#define ITEMIDPOSITION 0
#define ITEMCOUNTPOSITION 2

int currentEdition=0;

unsigned int ItemDataLength=0;
unsigned int MaxItemID=0;

//ItemBagAdresses
unsigned int ItemPointerOffset	  =0;
unsigned int KeyItemPointerOffset =0;
unsigned int TMHMPointerOffset	  =0;
unsigned int MedicinePointerOffset=0;
unsigned int BerryPointerOffset	  =0;

void initItemModifier(int edition)
{
	currentEdition=edition;
	switch (edition)
	{
		//XY
		case PKXY:
		{
			ItemDataLength 	= 0x4;
			MaxItemID		= 717;

			ItemPointerOffset 	 = 0x08C67564;//1600/0x640 //400
			KeyItemPointerOffset = 0x08c67ba4;//384 /0x180 // 96
			TMHMPointerOffset	 = 0x08C67D24;//424 /0x1A8 //106
			MedicinePointerOffset= 0x08C67ECC;//256 /0x100 // 64
			BerryPointerOffset	 = 0x08C67FCC;//440 /0x120 //110
											  //max 776 Items
		}
		break;

		//ORAS
		case ORAS:
		{
			ItemDataLength 	= 0x4;
			MaxItemID		= 775;

			ItemPointerOffset 	 = 0x08C6EC70;//1600/0x640 //400
			KeyItemPointerOffset = 0x08c6F2B0;//384 /0x180 // 96
			TMHMPointerOffset	 = 0x08c6f430;//432 /0x1b0 //108
			MedicinePointerOffset= 0x08C6F5E0;//256 /0x100 // 64
			BerryPointerOffset	 = 0x08C6F6E0;//440 /0x120 //110
											  //max 778 Items
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
		case tmBag:
			address=TMHMPointerOffset;
			break;
		case berryBag:
			address=BerryPointerOffset;
			break;
		case keyItemBag: 
			address=KeyItemPointerOffset;
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
u16 getItemIdAt(u32 position, ItemBags bag)
{
	u32 itemPointer=getItemPossitionAddress(position, bag);
	if(itemPointer>0)
	{
		return *(vu16*)itemPointer;
	}
	return 0;
}

void setItemIdAt(u32 position, ItemBags bag, u32 id)
{
	u32 itemPointer=getItemPossitionAddress(position, bag);
	if(itemPointer>0)
	{
		*(vu16*)itemPointer=id;
	}
}

u32 setItemIdFromTo(u16 startId, u16 endId, u16 startPosition, ItemBags bag)
{
	u32 position=startPosition;
	u32 curId;

	for(curId=startId;curId<=endId;curId++)
	{
		u32 itemPointer=getItemPossitionAddress(position, bag);
		if(itemPointer>0)
		{
			*(vu16*)itemPointer=curId;

			*(vu16*)(itemPointer+ITEMCOUNTPOSITION)=999;
		}
		position++;
	}
	return startPosition+(endId-startId+1);;
}

void incrementItemIdAt(u32 position, ItemBags bag, u32 count)
{
	u32 itemPointer=getItemPossitionAddress(position, bag);
	if(itemPointer>0)
	{
		if((*(vu16*)itemPointer+count)<MaxItemID)
			*(vu16*)itemPointer+=count;
		else
			*(vu16*)itemPointer=MaxItemID;
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

void getAllItems()
{
	//XY
	/*ItemPointerOffset 	 = 0x08C67564;//1600/0x640 //400
	KeyItemPointerOffset = 0x08c67ba4;//384 /0x180 // 96
	TMHMPointerOffset	 = 0x08C67D24;//424 /0x1A8 //106
	MedicinePointerOffset= 0x08C67ECC;//256 /0x100 // 64
	BerryPointerOffset	 = 0x08C67FCC;//440 /0x120 //110
												//max 776 Items
	
	//ORAS
	ItemPointerOffset 	 = 0x08C6EC70;//1600/0x640 //400
	KeyItemPointerOffset = 0x08c6F2B0;//384 /0x180 // 96 177/-81
	TMHMPointerOffset	 = 0x08c6f430;//432 /0x1b0 //108 107/ 1
	MedicinePointerOffset= 0x08C6F5E0;//256 /0x100 // 64  51/13
	BerryPointerOffset	 = 0x08C6F6E0;//440 /0x120 //110  68/42
												//max 778 Items*/
	u32 nextItemPosition;

	//fillItemBag
	nextItemPosition = 1;
	nextItemPosition = setItemIdFromTo(  1, 16,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(492,500,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(576,576,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo( 55,112,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(113,115,nextItemPosition,itemBag); //???
	nextItemPosition = setItemIdFromTo(116,119,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(120,133,nextItemPosition,itemBag); //???
	nextItemPosition = setItemIdFromTo(135,148,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(213,215,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(217,327,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(426,427,nextItemPosition,itemBag); //???
	nextItemPosition = setItemIdFromTo(487,491,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(537,564,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(575,575,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(577,577,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(580,590,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(592,615,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(639,640,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(644,644,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(646,650,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(652,685,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(699,699,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(710,711,nextItemPosition,itemBag);
	nextItemPosition = setItemIdFromTo(715,715,nextItemPosition,itemBag);
	if(currentEdition==ORAS)
	{
		nextItemPosition = setItemIdFromTo(752,764,nextItemPosition,itemBag);
		nextItemPosition = setItemIdFromTo(768,770,nextItemPosition,itemBag);
	}

	//fillMedicineBag
	nextItemPosition = 1;
	nextItemPosition = setItemIdFromTo( 17, 54,nextItemPosition,medicineBag);
	nextItemPosition = setItemIdFromTo(134,134,nextItemPosition,medicineBag);
	nextItemPosition = setItemIdFromTo(504,504,nextItemPosition,medicineBag);
	nextItemPosition = setItemIdFromTo(565,570,nextItemPosition,medicineBag);
	nextItemPosition = setItemIdFromTo(591,591,nextItemPosition,medicineBag);
	nextItemPosition = setItemIdFromTo(645,645,nextItemPosition,medicineBag);
	
	//fillBerryBag
	nextItemPosition = 1;
	nextItemPosition = setItemIdFromTo(149,212,nextItemPosition,berryBag);
	nextItemPosition = setItemIdFromTo(686,688,nextItemPosition,berryBag);
	
	//TM/HMBag
	nextItemPosition = 1;
	nextItemPosition = setItemIdFromTo(328,419,nextItemPosition,tmBag);
	nextItemPosition = setItemIdFromTo(618,620,nextItemPosition,tmBag);
	nextItemPosition = setItemIdFromTo(690,694,nextItemPosition,tmBag);
	nextItemPosition = setItemIdFromTo(420,425,nextItemPosition,tmBag);
	if(currentEdition==ORAS)
	{
		nextItemPosition = setItemIdFromTo(737,737,nextItemPosition,tmBag);
	}

	//keyItemBag
	/*nextItemPosition = 1;
	nextItemPosition = setItemIdFromTo(216,216,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(428,484,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(501,503,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(505,531,nextItemPosition,keyItemBag);//Datacards(27)
	nextItemPosition = setItemIdFromTo(532,536,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(574,574,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(578,579,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(616,617,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(621,638,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(641,643,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(651,651,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(689,689,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(695,698,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(700,709,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(712,714,nextItemPosition,keyItemBag);
	nextItemPosition = setItemIdFromTo(716,717,nextItemPosition,keyItemBag);
	
	if(currentEdition==ORAS)
	{
		nextItemPosition = setItemIdFromTo(718,736,nextItemPosition,keyItemBag);
		nextItemPosition = setItemIdFromTo(738,751,nextItemPosition,keyItemBag);
		nextItemPosition = setItemIdFromTo(765,766,nextItemPosition,keyItemBag);
		nextItemPosition = setItemIdFromTo(771,775,nextItemPosition,keyItemBag);
	}*/
}