#include "3dstypes.h"
#include "ctr/types.h"

typedef struct PK6
{

	u32 encryptionKey;
	u16 sanityPlaceholder;
	u16 checksum;
	
	u16 pokedexID;
	u16 heldItem;
	u16 TID;
	u16 SID;
	u32 experience;
	u8 ability;
	u8 abilityNumber;
	u16 hitRemainingTainingBag;
	u32 PID;
	u8 nature;
	u8 form;
	u8 EVs[6];
	u8 contest[6];
	u8 markings;
	u8 pokerus;
	u32 goldMedalFlags;
	u8 ribbons[6];
	u16 unused_1;
	u8 contestRibbons;
	u8 battleRibbons;
	u8 superTrainingFlags;
	u8 unused_2[5];
	
	u8 nickname[26];
	u16 moves[4];
	u8 currentPP[4];
	u8 movePPUps[4];
	u16 relearnMoves[4];
	u8 superTrainingFlag;
	u8 unused_3;
	u32 IVs;
	
	u8 latestHandlerName[26];
	u8 handlerGender;
	u8 currentHandler;
	u16 geolocation[5];
	u32 unused_4;
	u8 handlerFriendship;
	u8 handlerAffection;
	u8 handlerMemoryIntensity;
	u8 handlerMemoryLine;
	u8 handlerMemoryFeeling;
	u8 unused_5;
	u16 handlerMemoryTextVar;
	u32 unused_6;
	u8 fullness;
	u8 enjoyment;
	
	u8 trainerName[26];
	u8 trainerFriendship;
	u8 trainerAffection;
	u8 trainerMemoryIntensity;
	u8 trainerMemoryLine;
	u16 trainerMemoryTextVar;
	u8 trainerMemoryFeeling;
	u8 dateEggReceived[3];
	u8 dateMet[3];
	u8 unused_7;
	u16 eggLocation;
	u16 metLocation;
	u8 ball;
	u8 encounterLevel;
	u8 encounterType;
	u8 trinerGameID;
	u8 countryID;
	u8 regionID;
	u8 consoleRegionID;
	u8 languageID;
	u32 unused_8;

} PK6;

typedef enum
{
	HP,
	ATTACK,
	DEFENSE,
	SPEED,
	SP_ATTACK,
	SP_DEFENSE
}Stats;

Result shuffleArray(u8 *array, u8 sv);
u32 seedStep(u32 seed);

u16 checksum(PK6 *data, u32 len);

Result decryptPokemon(u8 *enc, PK6 *res);
Result encryptPokemon(PK6 *dec, u8 *enc);
Result decryptBattleStats(u32 key, PK6 *enc, PK6 *dec);