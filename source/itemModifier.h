#include "helpers.h"

typedef enum ItemBagsEnum 
{  
	itemBag,
	medicineBag,
	tmBag,
	berryBag,
	baseitemBaf
} ItemBags;

void initItemModifier(int edition);

// Item modification
void setItemIdAt(u32 position, ItemBags bag, u32 id);
void incrementItemIdAt(u32 position, ItemBags bag, u32 count);
void reduceItemIdAt(u32 position, ItemBags bag, u32 count);

// Item count modification
u32 getItemCountAddress(u32 position,ItemBags);
void setItemCountAt(u32 position,ItemBags, u32 count);
void addToItemCountAt(u32 position,ItemBags, u32 count);
void removeFromItemCountAt(u32 position,ItemBags, u32 count);

void deleteItem(u32 position, ItemBags bag);
