#include "helpers.h"
#include "menu.h"
#include "pokeEncounterModifier.h"
#include "itemModifier.h"
#include "otherModifiers.h"
#include "battle/wildModifier.h"
#include "battle/statsModifier.h"
#include "rng/rngModifier.h"
#include "time.h"

#define BUTTONCOMBINATION_ENCOUNTER BUTTON_L
#define BUTTONCOMBINATION_ENCOUNTER_VARIATION BUTTONCOMBINATION_ENCOUNTER|BUTTON_B
#define BUTTONCOMBINATION_ENCOUNTER_LEVEL BUTTONCOMBINATION_ENCOUNTER|BUTTON_X
#define BUTTONCOMBINATION_ITEM_MODIFIER BUTTON_R
#define BUTTONCOMBINATION_ITEM_MODIFIER_ID BUTTONCOMBINATION_ITEM_MODIFIER
#define BUTTONCOMBINATION_ITEM_MODIFIER_BAG BUTTONCOMBINATION_ITEM_MODIFIER|BUTTON_B
#define BUTTONCOMBINATION_ITEM_MODIFIER_POSITION BUTTONCOMBINATION_ITEM_MODIFIER|BUTTON_X
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
u32 curkey=0;

enum menuEntrysEnum
{
	headerGeneration,
		entryAllShiny,
	headerEncounter,
		entryEncounterRandomize,
		entryEncounterItem,
		entryEncounterShiny,
	headerVariation,
		entryVariationRandomize,
		entryVariationItem,
	headerWildLevel,
		entryWildLevelRandomize,
		entryWildLevelItem,
		entryUpdateDexnav,
	headerItemModifiers,
		entryItemModifier,
		entryItemCount1,
		entryItemCount2,
		entryItemCount3,
		entryMedicineCount1,
		entryMedicineCount2,
		entryMedicineCount3,
	headerBattleModifiers,
		entryUnlimitedHP,
		entryUnlimitedPP,
		entryCatchRate,
		entryExpPoints,	
		entryOpponent1HP,
	headerOther,
		entryQuickHatching,
		entryMaxOpower,
		entryMaxRepel,
		entryPokeRadarCharge,
	headerBetaCheats,
		entryAllItems,
		entryMaxMoney
};

// create the plugins menu content 
void initCheatMenu() {
	initMenu();
	addMenuEntry("Pokemon Generation modifier");
	addCheatMenuEntry("All Shiny Pokemon");
	
	addMenuEntry("Wild Encounter modifier");
	addOrCheatMenuEntry(" Random(only unseen)");
	addOrCheatMenuEntry(" First item count");
	addCheatMenuEntry(" Shiny Pokemon");
	
	addMenuEntry("Wild Encounter variation modifier");
	addOrCheatMenuEntry(" Random");
	addOrCheatMenuEntry(" Second item count");
	
	addMenuEntry("Wild Encounter level modifier");
	addOrCheatMenuEntry(" Random");
	addOrCheatMenuEntry(" Third item count");
	addCheatMenuEntry(" Update Dexnav(ORAS only)");
	
	addMenuEntry("Item modifier");
	addCheatMenuEntry(" Item modifier(using medicine count)");
	addCheatMenuEntry(" First item  999x");
	addCheatMenuEntry(" Second item 999x");
	addCheatMenuEntry(" Third item  999x");
	addCheatMenuEntry(" First Medicine 999x");
	addCheatMenuEntry(" Second Medicine 999x");
	addCheatMenuEntry(" Third Medicine 999x");
	
	addMenuEntry("Battle modifier");
	addCheatMenuEntry(" Unlimited HP");
	addCheatMenuEntry(" Unlimited PP");
	addCheatMenuEntry(" 100% Catch Rate");
	addCheatMenuEntry(" 1.000.000Exp Points");
	addCheatMenuEntry(" Opponent 1 HP");
	
	addMenuEntry("Other");
	addCheatMenuEntry(" Quick egg hatching");
	addCheatMenuEntry(" Unlimited opower");
	addCheatMenuEntry(" Unlimited repel");
	addCheatMenuEntry(" Unlimited Pokeradar use(YX only)");
	
	addMenuEntry("Untested Cheats");
	addCheatMenuEntry(" Get all items");
	addCheatMenuEntry(" Max Money");

	
	updateMenu();
}

//Init the plugins Modules
void initPlugins()
{
	edition=getEdition();
	initWildPokeModifier(edition);
	initItemModifier(edition);
	initOtherModifiers(edition);
	initWildModifier(edition);
	initStatsModifier(edition);
	initRNGModifier(edition);
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
void updateCheatEnableDisplay(int id) 
{
	gamePluginMenu.buf[gamePluginMenu.offsetInBuffer[id] + 2] = cheatEnabled[id] ? 'X' : ' ';
}

void disableCheat(int id)
{
	cheatEnabled[id]=false;
	updateCheatEnableDisplay(id);
}

// this function will be called when the state of cheat item changed
void onCheatItemChanged(int id, int enable) 
{
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
	else if(id==entryVariationItem&&enable==true)
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
	else if(id==entryWildLevelItem&&enable==true)
	{	
		disableCheat(entryWildLevelRandomize);
		disableCheat(entryItemCount3);
	}
	else if(id==entryItemCount3&&enable==true)
	{	
		disableCheat(entryWildLevelItem);
		disableCheat(entryWildLevelRandomize);
	}


	//shiny modifier
	if(id==entryAllShiny&&enable==true)
	{
		disableCheat(entryEncounterShiny);
	}
	else if(id==entryEncounterShiny&&enable==true)
	{	
		disableCheat(entryAllShiny);
	}


	//itemmodifier
	if(id==entryItemModifier&&enable==true)
	{	
		disableCheat(entryMedicineCount1);
		disableCheat(entryMedicineCount2);
		disableCheat(entryMedicineCount3);
	}
	else if(id==entryMedicineCount1||id==entryMedicineCount2||id==entryMedicineCount3&&enable==true)
	{	
		disableCheat(entryItemModifier);
	}


	//only in oras supported
	if(id==entryUpdateDexnav)
	{
		if(edition!=ORAS)
			disableCheat(entryUpdateDexnav);
	}

	//only in XY supported
	if(id==entryPokeRadarCharge)
	{
		if(edition!=PKXY)
			disableCheat(entryUpdateDexnav);
	}
}

// Helperfunction for the item count modifications
bool checkForItemModKeyCombo(u32 position,ItemBags bag,u32 baseCombination )
{
	bool done=false;
	if(curkey==(baseCombination | BUTTONCOMBINATION_ADD_1))
	{	
		addToItemCountAt(position,bag,1);
		waitKeyCombinationChanged(baseCombination | BUTTONCOMBINATION_ADD_1);
		done=true;
	}else if(curkey==(baseCombination | BUTTONCOMBINATION_REM_1))
	{
		removeFromItemCountAt(position,bag,1);
		waitKeyCombinationChanged(baseCombination | BUTTONCOMBINATION_REM_1);
		done=true;
	}else if(curkey==(baseCombination | BUTTONCOMBINATION_ADD_10))
	{
		addToItemCountAt(position,bag,10);
		waitKeyCombinationChanged(baseCombination | BUTTONCOMBINATION_ADD_10);
		done=true;
	}else if(curkey==(baseCombination | BUTTONCOMBINATION_REM_10))
	{
		removeFromItemCountAt(position,bag,10);
		waitKeyCombinationChanged(baseCombination | BUTTONCOMBINATION_REM_10);
		done=true;
	}
	
	return done;
}

// Handle cheats
void handleCheats() 
{
	curkey=getKey();	

	//Wild Pokemon encounter modification cheats
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
		{
			pokemonAddress=1;
			onlyUnseenPoke=true;
		}
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

		if(cheatEnabled[entryEncounterItem])
		{
			if(curkey==(BUTTONCOMBINATION_ENCOUNTER | BUTTON_A))
				setItemCountAt(1,itemBag,721);

			checkForItemModKeyCombo(1,itemBag,BUTTONCOMBINATION_ENCOUNTER);
		}

		if(cheatEnabled[entryVariationItem])
		{
			checkForItemModKeyCombo(2,itemBag,BUTTONCOMBINATION_ENCOUNTER_VARIATION);
			if(curkey==(BUTTONCOMBINATION_ENCOUNTER | BUTTON_A))
				setItemCountAt(2,itemBag,27);
		}

		if(cheatEnabled[entryWildLevelItem])
		{
			checkForItemModKeyCombo(3,itemBag,BUTTONCOMBINATION_ENCOUNTER_LEVEL);
			if(curkey==(BUTTONCOMBINATION_ENCOUNTER | BUTTON_A))
				setItemCountAt(3,itemBag,100);
		}
		setWildPokemonfromAddress(pokemonAddress,variationAddress, levelAddress,onlyUnseenPoke,updateDexNav);
	}
	
	if(cheatEnabled[entryAllShiny])
	{
		writeFullShinyTable();
	}
	
	if(cheatEnabled[entryEncounterShiny])
	{
		PK6 pkm;
		if(getCurrentPokemon(&pkm) && !isShiny(&pkm))
		{
			makeShiny(&pkm);
			setCurrentPokemon(&pkm);
		}
	}



	//Item modification cheats
	if(cheatEnabled[entryItemModifier])
	{
		bool updatedPosition=false;
		
		updatedPosition=checkForItemModKeyCombo(1,medicineBag,BUTTONCOMBINATION_ITEM_MODIFIER_POSITION);
		updatedPosition=checkForItemModKeyCombo(2,medicineBag,BUTTONCOMBINATION_ITEM_MODIFIER_BAG);
		checkForItemModKeyCombo(3,medicineBag,BUTTONCOMBINATION_ITEM_MODIFIER_ID);
			

		if(curkey==(BUTTONCOMBINATION_ITEM_MODIFIER | BUTTON_A))
		{
			setItemCountAt(1,medicineBag,1);
			setItemCountAt(2,medicineBag,1);
			setItemCountAt(3,medicineBag,1);
		}

		u32 targetItemPosition=*(vu16*)getItemCountAddress(1,medicineBag);
		u32 targetItemBag=*(vu16*)getItemCountAddress(2,medicineBag)-1;
		u32 targetItemID=*(vu16*)getItemCountAddress(3,medicineBag);
		if(updatedPosition==true)
		{
			u32 curItemID=getItemIdAt(targetItemPosition, targetItemBag);
			setItemCountAt(3,medicineBag,curItemID);
		}
		else
			setItemIdAt(targetItemPosition, targetItemBag, targetItemID);
	}

	if(cheatEnabled[entryItemCount1]) 
		setItemCountAt(1,itemBag,999);
	
	if(cheatEnabled[entryItemCount2]) 
		setItemCountAt(2,itemBag,999);
	
	if(cheatEnabled[entryItemCount3]) 
		setItemCountAt(3,itemBag,999);
	
	if(cheatEnabled[entryMedicineCount1]) 
		setItemCountAt(1,medicineBag,999);
	
	if(cheatEnabled[entryMedicineCount2]) 
		setItemCountAt(2,medicineBag,999);
	
	if(cheatEnabled[entryMedicineCount3]) 
		setItemCountAt(3,medicineBag,999);
	


	// Battle modifier cheats
	if(cheatEnabled[entryUnlimitedHP])
	{
		setPartyMaxHP(0xFFFF);
		setPartyCurrentHP(0xFFFF);
	}
	
	if(cheatEnabled[entryOpponent1HP])
	{
		setOpponentCurrentHP(1);
	}
	
	if(cheatEnabled[entryUnlimitedPP])
		setAllPPMax();
		
	if(cheatEnabled[entryExpPoints])
		setGainExp(1000000);
	
	if(cheatEnabled[entryCatchRate])
		setAutomaticCatchSuccess();



	//Other
	if(cheatEnabled[entryQuickHatching])
		setHatchingStepCounter(0x101);

	if(cheatEnabled[entryMaxOpower])
		setRemainingOPower(10);

	if(cheatEnabled[entryMaxRepel])
		setRemainingRepel(0x02);

	if(cheatEnabled[entryPokeRadarCharge])
		setPokeradarCharge(0xFF);



	//Untested/unstable/beta cheats
	if(cheatEnabled[entryMaxMoney])
		setMoney(0x98967F);
	
	if(cheatEnabled[entryAllItems])
		getAllItems();

}

//Check if the cheat with id is a header
bool isHeader(int id)
{
	u32 headerCount=6;
	int headerIDs[6]={
						headerVariation,
						headerEncounter,
						headerBetaCheats,
						headerItemModifiers,
						headerWildLevel,
						headerOther
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

// Main function of the plugin
void gamePluginEntry() {
	u32 ret, key;
	INIT_SHARED_FUNC(plgGetIoBase, 8);
	INIT_SHARED_FUNC(copyRemoteMemory, 9);
	// wait for game starts up (5 seconds)
	svc_sleepThread(5000000000);
	srand(svc_getSystemTick());

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
		svc_sleepThread(100000);
		scanCheatMenu();
		handleCheats();
	}
}
