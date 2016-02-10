#include "helpers.h"
#include "menu.h"
#include "pokeEncounterModifier.h"
#include "itemModifier.h"
#include "otherModifiers.h"

#define BUTTONCOMBINATION_ENCOUNTER BUTTON_L|BUTTON_R
#define BUTTONCOMBINATION_ENCOUNTER_VARIATION BUTTONCOMBINATION_ENCOUNTER|BUTTON_B
#define BUTTONCOMBINATION_ENCOUNTER_LEVEL BUTTONCOMBINATION_ENCOUNTER|BUTTON_X
#define BUTTONCOMBINATION_ADD_1 BUTTON_DU
#define BUTTONCOMBINATION_ADD_10 BUTTON_DR
#define BUTTONCOMBINATION_REM_1 BUTTON_DD
#define BUTTONCOMBINATION_REM_10 BUTTON_DL

u32 threadStack[0x1000];
Handle fsUserHandle;
FS_archive sdmcArchive;

u8 cheatEnabled[64];
int isNewNtr = 0;
int edition = 0;

enum menuEntrysEnum
{
	headerEncounter,
	entryEncounterRandomize,
	entryEncounterItem,
	headerVariation,
	entryVariationRandomize,
	entryVariationItem,
	headerWildLevel,
	entryWildLevelRandomize,
	entryWildLevelItem,
	entryUpdateDexnav,
	headerItemModifiers,
	entryItemCount1,
	entryItemCount2,
	entryItemCount3,
	entryMedicineCount1,
	entryMedicineCount2,
	entryMedicineCount3,
	headerBetaCheats,
	entryMaxMoney,
	entryMaxRepel,
	entryMaxOpower
};

// init 
void initCheatMenu() {
	initMenu();
	addMenuEntry("Wild Encounter modifier");
	addOrCheatMenuEntry(" Random(only unseen)");
	addOrCheatMenuEntry(" First item count");
	addMenuEntry("Wild Encounter variation modifier");
	addOrCheatMenuEntry(" Random");
	addOrCheatMenuEntry(" Second item count");
	addMenuEntry("Wild Encounter level modifier");
	addOrCheatMenuEntry(" Random");
	addOrCheatMenuEntry(" Third item count");
	addCheatMenuEntry("Update Pokeradar(ORAS only)");
	addMenuEntry("Item modifier");
	addCheatMenuEntry(" Firts item  999x");
	addCheatMenuEntry(" Second item 999x");
	addCheatMenuEntry(" Third item  999x");
	addCheatMenuEntry(" Firts Medicine 999x");
	addCheatMenuEntry(" Second Medicine 999x");
	addCheatMenuEntry(" Third Medicine 999x");
	addMenuEntry("Untested Cheats");
	addCheatMenuEntry(" Max Money");
	addCheatMenuEntry(" Unlimited repel");
	addCheatMenuEntry(" Unlimited opower(ORAS only)");

	
	updateMenu();
}
void initPlugins()
{
	edition=getEdition();
	initWildPokeModifier(edition);
	initItemModifier(edition);
	initOtherModifiers(edition);
}

int scanMenu() {
	u32 i;
	for (i = 0; i < gamePluginMenu.count; i++) {
		if (gamePluginMenu.state[i]) {
			gamePluginMenu.state[i] = 0;
			return i;
		}
	}
	return -1;
}

// detect language (0: english)
int detectLanguage() {
	// unimplemented
	return 0;
}
// update the menu status
void updateCheatEnableDisplay(int id) {
	gamePluginMenu.buf[gamePluginMenu.offsetInBuffer[id] + 1] = cheatEnabled[id] ? 'X' : ' ';
}

void disableCheat(int id)
{
	cheatEnabled[id]=false;
	updateCheatEnableDisplay(id);
}

// this function will be called when the state of cheat item changed
void onCheatItemChanged(int id, int enable) {
	//Only one at time possible
	//WildEncounterModifier
	if(id==entryEncounterRandomize&&enable==true)
	{	
		disableCheat(entryEncounterItem);
		disableCheat(entryItemCount1);
	}
	else if(id==entryEncounterItem&&enable==true)
	{	
		disableCheat(entryEncounterRandomize);
		disableCheat(entryItemCount1);
	}
	else if(id==entryItemCount1&&enable==true)
	{	
		disableCheat(entryEncounterItem);
		disableCheat(entryEncounterRandomize);
	}

	
	//WildVariationModifier
	if(id==entryVariationRandomize&&enable==true)
	{
		disableCheat(entryVariationItem);
		disableCheat(entryItemCount2);
	}
	if(id==entryVariationItem&&enable==true)
	{	
		disableCheat(entryVariationRandomize);
		disableCheat(entryItemCount2);
	}
	else if(id==entryItemCount2&&enable==true)
	{	
		disableCheat(entryVariationItem);
		disableCheat(entryVariationRandomize);
	}


	//WildLevelModifier
	if(id==entryWildLevelRandomize&&enable==true)
	{
		disableCheat(entryWildLevelItem);
		disableCheat(entryItemCount3);
	}
	if(id==entryWildLevelItem&&enable==true)
	{	
		disableCheat(entryWildLevelRandomize);
		disableCheat(entryItemCount3);
	}
	else if(id==entryItemCount3&&enable==true)
	{	
		disableCheat(entryWildLevelItem);
		disableCheat(entryWildLevelRandomize);
	}

	//only in oras supported
	if(id==entryUpdateDexnav)
	{
		if(edition==PKXY)
			disableCheat(entryUpdateDexnav);
	}

	//no address for XY
	if(id==entryMaxOpower)
	{
		if(edition==PKXY)
			disableCheat(entryMaxOpower);
	}
}

// Handle cheats
void handleCheats() {
	if(cheatEnabled[entryEncounterRandomize]
			||cheatEnabled[entryEncounterItem]
			||cheatEnabled[entryVariationRandomize]
			||cheatEnabled[entryVariationItem]) 
	{
		u32 pokemonAddress=0;
		u32 variationAddress=0;
		u32 levelAddress=0;
		bool onlyUnseenPoke=false;
		bool updateDexNav=false;

		if(cheatEnabled[entryEncounterRandomize])
			pokemonAddress=1;
		else if(cheatEnabled[entryEncounterItem])
		{
			pokemonAddress=getItemCountAddress(1,itemBag);
			onlyUnseenPoke=true;
		}

		if(cheatEnabled[entryVariationRandomize])
			variationAddress=1;
		else if(cheatEnabled[entryVariationItem])
			variationAddress=getItemCountAddress(2,itemBag);


		if(cheatEnabled[entryWildLevelRandomize])
			levelAddress=1;
		else if(cheatEnabled[entryWildLevelItem])
			levelAddress=getItemCountAddress(3,itemBag);

		if(cheatEnabled[entryUpdateDexnav])
			updateDexNav=true;

		u32 key = getKey();
		if(cheatEnabled[entryEncounterItem])
		{
			switch(key)
			{
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_1):
					addToItemCountAt(1,itemBag,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1):
					removeFromItemCountAt(1,itemBag,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10):
					addToItemCountAt(1,itemBag,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10):
					removeFromItemCountAt(1,itemBag,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTON_A):
					setItemCountAt(1,itemBag,721);
					break;
			}
		}

		if(cheatEnabled[entryVariationItem])
		{
			switch(key)
			{
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_ADD_1):
					addToItemCountAt(2,itemBag,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTON_B | BUTTONCOMBINATION_ADD_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_REM_1):
					removeFromItemCountAt(2,itemBag,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_ADD_10):
					addToItemCountAt(2,itemBag,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_REM_10):
					removeFromItemCountAt(2,itemBag,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTON_A):
					setItemCountAt(2,itemBag,27);
					break;
			}
		}

		if(cheatEnabled[entryWildLevelItem])
		{
			switch(key)
			{
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_ADD_1):
					addToItemCountAt(3,itemBag,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTON_B | BUTTONCOMBINATION_ADD_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_REM_1):
					removeFromItemCountAt(3,itemBag,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_ADD_10):
					addToItemCountAt(3,itemBag,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_REM_10):
					removeFromItemCountAt(3,itemBag,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTON_A):
					setItemCountAt(3,itemBag,100);
					break;
			}
		}
		setWildPokemonfromAddress(pokemonAddress,variationAddress, levelAddress,onlyUnseenPoke,updateDexNav);
	}

	if(cheatEnabled[entryItemCount1]) {
		setItemCountAt(1,itemBag,999);
	}
	if(cheatEnabled[entryItemCount2]) {
		setItemCountAt(2,itemBag,999);
	}
	if(cheatEnabled[entryItemCount3]) {
		setItemCountAt(3,itemBag,999);
	}
	if(cheatEnabled[entryMedicineCount1]) {
		setItemCountAt(1,medicineBag,999);
	}
	if(cheatEnabled[entryMedicineCount2]) {
		setItemCountAt(2,medicineBag,999);
	}
	if(cheatEnabled[entryMedicineCount3]) {
		setItemCountAt(3,medicineBag,999);
	}
	if(cheatEnabled[entryMaxMoney]) {
		setMoney(0x98967F);
	}
	if(cheatEnabled[entryMaxRepel]) {
		setRemainingRepel(0x00FA004D);
	}
	if(cheatEnabled[entryMaxOpower]) {
		setRemainingOPower(10);
	}

}

bool isHeader(int id)
{
	u32 headerCount=5;
	int headerIDs[5]={
						headerVariation,
						headerEncounter,
						headerBetaCheats,
						headerItemModifiers,
						headerWildLevel
					};
	int i;
	for(i=0;i<headerCount;i++)
	{	
		if (headerIDs[i]==id)
			return true;
	}
	return false;
}

// scan and handle events
void scanCheatMenu() {
	int ret = scanMenu();
	if (ret != -1) 
	{
		if(!isHeader(ret))
		{
			cheatEnabled[ret] = !cheatEnabled[ret];
			updateCheatEnableDisplay(ret);
			onCheatItemChanged(ret, cheatEnabled[ret]);
		}
	}
}



void gamePluginEntry() {
	u32 ret, key;
	INIT_SHARED_FUNC(plgGetIoBase, 8);
	INIT_SHARED_FUNC(copyRemoteMemory, 9);
	// wait for game starts up (5 seconds)
	svc_sleepThread(5000000000);

	if (((NS_CONFIG*)(NS_CONFIGURE_ADDR))->sharedFunc[8]) {
		isNewNtr = 1;
	} else {
		isNewNtr = 0;
	}
	
	if (isNewNtr) {
		IoBasePad = plgGetIoBase(IO_BASE_PAD);
	}
	initPlugins();
	initCheatMenu();
	while (1) {
		svc_sleepThread(100000000);
		scanCheatMenu();
		handleCheats();
	}
}