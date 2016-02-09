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

#define DEFINE_ENCOUNTER 0
#define ENTRY_ENCOUNTER_RANDOMIZE 1
#define ENTRY_ENCOUNTER_ITEM 2
#define DEFINE_VARIATION 3
#define ENTRY_VARIATION_RANDOMIZE 4
#define ENTRY_VARIATION_ITEM 5
#define DEFINE_LEVEL 6
#define ENTRY_LEVEL_RANDOMIZE 7
#define ENTRY_LEVEL_ITEM 8
#define ENTRY_DEXNAV 9
#define DEFINE_ITEM_COUNT 10
#define ENTRY_ITEM_COUNT_1 11
#define ENTRY_ITEM_COUNT_2 12
#define ENTRY_ITEM_COUNT_3 13
#define DEFINE_BETA_CHEATS 14
#define ENTRY_MEDICINE_COUNT_1 15
#define ENTRY_MAX_MONEY 16
#define ENTRY_MAX_REPEL 17
#define ENTRY_MAX_OPOWER 18

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
	addMenuEntry("Item count modifier");
	addCheatMenuEntry(" Firts item  999x");
	addCheatMenuEntry(" Second item 999x");
	addCheatMenuEntry(" Third item  999x");
	addMenuEntry("Untested Cheats");
	addCheatMenuEntry(" Firts Medicine 999x");
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
	// TODO: handle on cheat item is select or unselected
	//Only one at time possible
	//WildEncounterModifier
	if(id==ENTRY_ENCOUNTER_RANDOMIZE&&enable==true)
	{	
		disableCheat(ENTRY_ENCOUNTER_ITEM);
		disableCheat(ENTRY_ITEM_COUNT_1);
	}
	else if(id==ENTRY_ENCOUNTER_ITEM&&enable==true)
	{	
		disableCheat(ENTRY_ENCOUNTER_RANDOMIZE);
		disableCheat(ENTRY_ITEM_COUNT_1);
	}
	else if(id==ENTRY_ITEM_COUNT_1&&enable==true)
	{	
		disableCheat(ENTRY_ENCOUNTER_ITEM);
		disableCheat(ENTRY_ENCOUNTER_RANDOMIZE);
	}


	//WildVariationModifier
	if(id==ENTRY_VARIATION_RANDOMIZE&&enable==true)
	{
		disableCheat(ENTRY_VARIATION_ITEM);
		disableCheat(ENTRY_ITEM_COUNT_2);
	}
	if(id==ENTRY_VARIATION_ITEM&&enable==true)
	{	
		disableCheat(ENTRY_VARIATION_RANDOMIZE);
		disableCheat(ENTRY_ITEM_COUNT_2);
	}
	else if(id==ENTRY_ITEM_COUNT_2&&enable==true)
	{	
		disableCheat(ENTRY_VARIATION_ITEM);
		disableCheat(ENTRY_VARIATION_RANDOMIZE);
	}

	//WildLevelModifier
	if(id==ENTRY_LEVEL_RANDOMIZE&&enable==true)
	{
		disableCheat(ENTRY_LEVEL_ITEM);
		disableCheat(ENTRY_ITEM_COUNT_3);
	}
	if(id==ENTRY_LEVEL_ITEM&&enable==true)
	{	
		disableCheat(ENTRY_LEVEL_RANDOMIZE);
		disableCheat(ENTRY_ITEM_COUNT_3);
	}
	else if(id==ENTRY_ITEM_COUNT_3&&enable==true)
	{	
		disableCheat(ENTRY_LEVEL_ITEM);
		disableCheat(ENTRY_LEVEL_RANDOMIZE);
	}

	if(id==ENTRY_DEXNAV)
	{
		if(edition==PKXY)
			disableCheat(ENTRY_DEXNAV);
	}
}

// Handle cheats
void handleCheats() {
	if(cheatEnabled[ENTRY_ENCOUNTER_RANDOMIZE]
			||cheatEnabled[ENTRY_ENCOUNTER_ITEM]
			||cheatEnabled[ENTRY_VARIATION_RANDOMIZE]
			||cheatEnabled[ENTRY_VARIATION_ITEM]) 
	{
		u32 pokemonAddress=0;
		u32 variationAddress=0;
		u32 levelAddress=0;
		bool onlyUnseenPoke=false;
		bool updateDexNav=false;

		if(cheatEnabled[ENTRY_ENCOUNTER_RANDOMIZE])
			pokemonAddress=1;
		else if(cheatEnabled[ENTRY_ENCOUNTER_ITEM])
		{
			pokemonAddress=getItemCountAddress(1);
			onlyUnseenPoke=true;
		}

		if(cheatEnabled[ENTRY_VARIATION_RANDOMIZE])
			variationAddress=1;
		else if(cheatEnabled[ENTRY_VARIATION_ITEM])
			variationAddress=getItemCountAddress(2);


		if(cheatEnabled[ENTRY_LEVEL_RANDOMIZE])
			levelAddress=1;
		else if(cheatEnabled[ENTRY_LEVEL_ITEM])
			levelAddress=getItemCountAddress(3);

		if(cheatEnabled[ENTRY_DEXNAV])
			updateDexNav=true;

		u32 key = getKey();
		if(cheatEnabled[ENTRY_ENCOUNTER_ITEM])
		{
			switch(key)
			{
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_1):
					addToItemCountAt(1,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1):
					removeFromItemCountAt(1,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10):
					addToItemCountAt(1,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10):
					removeFromItemCountAt(1,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTON_A):
					setItemCountAt(1,721);
					break;
			}
		}

		if(cheatEnabled[ENTRY_VARIATION_ITEM])
		{
			switch(key)
			{
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_ADD_1):
					addToItemCountAt(2,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTON_B | BUTTONCOMBINATION_ADD_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_REM_1):
					removeFromItemCountAt(2,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_ADD_10):
					addToItemCountAt(2,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_VARIATION | BUTTONCOMBINATION_REM_10):
					removeFromItemCountAt(2,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTON_A):
					setItemCountAt(2,27);
					break;
			}
		}

		if(cheatEnabled[ENTRY_LEVEL_ITEM])
		{
			switch(key)
			{
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_ADD_1):
					addToItemCountAt(3,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTON_B | BUTTONCOMBINATION_ADD_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_REM_1):
					removeFromItemCountAt(3,1);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_1);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_ADD_10):
					addToItemCountAt(3,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_ADD_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER_LEVEL | BUTTONCOMBINATION_REM_10):
					removeFromItemCountAt(3,10);
					waitKeyCombinationChanged(BUTTONCOMBINATION_ENCOUNTER | BUTTONCOMBINATION_REM_10);
					break;
				case (BUTTONCOMBINATION_ENCOUNTER | BUTTON_A):
					setItemCountAt(3,100);
					break;
			}
		}
		setWildPokemonfromAddress(pokemonAddress,variationAddress, levelAddress,onlyUnseenPoke,updateDexNav);
	}

	if(cheatEnabled[ENTRY_ITEM_COUNT_1]) {
		setItemCountAt(1,999);
	}
	if(cheatEnabled[ENTRY_ITEM_COUNT_2]) {
		setItemCountAt(2,999);
	}
	if(cheatEnabled[ENTRY_ITEM_COUNT_3]) {
		setItemCountAt(3,999);
	}
	if(cheatEnabled[ENTRY_MEDICINE_COUNT_1]) {
		setMedicineCountAt(1,999);
	}
	if(cheatEnabled[ENTRY_MAX_MONEY]) {
		setMoney(0x98967F);
	}
	if(cheatEnabled[ENTRY_MAX_REPEL]) {
		setRemainingRepel(0x00FA004D);
	}
	if(cheatEnabled[ENTRY_MAX_OPOWER]) {
		setRemainingOPower(10);
	}

}

bool isCategory(int id)
{
	int categorys[4]={DEFINE_VARIATION,DEFINE_ENCOUNTER,DEFINE_ITEM_COUNT,DEFINE_LEVEL};
	int i;
	for(i=0;i<4;i++)
	{	
		if (categorys[i]==id)
			return true;
	}
	return false;
}

// scan and handle events
void scanCheatMenu() {
	int ret = scanMenu();
	if (ret != -1) 
	{
		if(!isCategory(ret))
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