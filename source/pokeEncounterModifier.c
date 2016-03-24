#include "pokeEncounterModifier.h"

unsigned int PointerOffset=0;
unsigned int EncDataLength=0;
unsigned int ByteJump=0;
unsigned int DexOffset=0;
unsigned int curEdition=0;

void initWildPokeModifier(int edition)
{
	curEdition=edition;
	switch (edition)
	{
		//XY
		case PKXY:
		{
			PointerOffset = 0x08CEC564;
			EncDataLength = 0x178;
			ByteJump = 0x10;
			DexOffset = 0x08CC8D4C;
		}
		break;

		//ORAS
		case ORAS:
		{
			PointerOffset = 0x08D06468;
			EncDataLength = 0xF4;
			ByteJump = 0x0E;
			DexOffset = 0x08CE0CB8;
		}
		break;
	}
}

int RandMinMax(int low, int high)
{
	return low + (rand() % (high - low + 1));
}

bool isObtained(int value)
{
	if ((DexOffset >= 0x08000000) && (DexOffset < 0x08DF0000))
	{
		unsigned int ObtainedOffset = Read32(DexOffset);
		if ((ObtainedOffset >= 0x08000000) && (ObtainedOffset < 0x08DF0000))
		{
			//check if all obtained
			int i;
			for (i = 0; i < 721; i++)
			{
				if ((Read8(ObtainedOffset + 0x0C + (i / 8)) & (1 << (i % 8))) == 0)
				{
					break;
				}

				if (i == 720)
				{
					return false;
				}
			}

			//check if value is set
			if ((Read8(ObtainedOffset + 0x0C + ((value - 1) / 8)) & (1 << ((value - 1) % 8))) != 0)
			{
				return true;
			}
		}
	}
	return false;
}

int getRandomVariation(int pokemon)
{
	int result=pokemon;
	switch (result)
	{
		//XY Forms
		case 421://Cherrim
		case 422://Shellos
		case 423://Gastrodon
		case 487://Giratina
		case 492://Shaymin
		case 550://Basculin
		case 555://Darmanitan
		case 641://Tornadus
		case 642://Thundurus
		case 645://Landoorus
		case 647://Keldeo
		case 648://Meloetta
		case 681://Aegislash
		case 716://Xerneas
		{
			result += (0x800 * RandMinMax(0, 1));
		}
		break;

		case 412://Burmy
		case 413://Wormadam
		case 646://Kyurem
		{
			result += (0x800 * RandMinMax(0, 2));
		}
		break;

		case 351://Castform
		case 386://Deoxys
		case 585://Deerling
		case 586://Sawsbuck
		case 710://Pumpkaboo
		case 711://Gourgeis
		{
			result += (0x800 * RandMinMax(0, 3));
		}
		break;

		case 649://Genesect
		case 669://Flabébé 
		case 671://Florges
		{
			result += (0x800 * RandMinMax(0, 4));
		}
		break;

		case 479://Rotom
		case 670://Floette
		{
			result += (0x800 * RandMinMax(0, 5));
		}
		break;

		case 676://Furfrou
		{
			result += (0x800 * RandMinMax(0, 9));
		}
		break;

		case 493://Arceus
		{
			result += (0x800 * RandMinMax(0, 17));
		}
		break;

		case 666://Vivillon
		{
			result += (0x800 * RandMinMax(0, 19));
		}
		break;

		case 201://Unown
		{
			result += (0x800 * RandMinMax(0, 27));
		}
		break;

		//ORAS Forms
		case 26://Pikachu
		{
			if (curEdition == ORAS)
			{
				result += (0x800 * RandMinMax(0, 6));
			}
		}
		break;

		case 720://Hoopa
		{
			if (curEdition == ORAS)
			{
				result += (0x800 * RandMinMax(0, 1));
			}
		}
		break;

		//XY Megas 1
		case 3://Venusaur
		case 9://Blastoise
		case 65://Alakazam
		case 94://Gengar
		case 115://Kangaskhan
		case 127://Pinsir
		case 130://Gyarados
		case 142://Aerodactyl
		case 181://Ampharos
		case 208://Steelix
		case 212://Scizor
		case 214://Heracross
		case 229://Houndoom
		case 248://Tyranitar
		case 257://Blazikin
		case 282://Gardevoir
		case 303://Mawile
		case 306://Aggron
		case 308://Medicham
		case 310://Manectric
		case 354://Banette
		case 359://Absol
		case 380://Latias
		case 381://Latios
		case 445://Garchomp
		case 448://Lucario
		case 460://Abomasnow
		{
			result += (0x800 * RandMinMax(0, 1));
		}
		break;

		//XY Megas 2
		case 6://Charizard
		case 150://Mewtwo
		{
			result += (0x800 * RandMinMax(0, 2));
		}
		break;

		//ORAS Megas 1
		case 15://Beedrill
		case 18://Pidgeot
		case 80://Slowbro
		case 254://Sceptile
		case 260://Swampert
		case 302://Sableye
		case 319://Sharpedo
		case 323://Camerupt
		case 334://Altaria
		case 362://Glalie
		case 373://Salamence
		case 376://Metagross
		case 382://Kyogre
		case 383://Groudon
		case 384://Rayquaza
		case 428://Lopunny
		case 475://Gallade
		case 531://Audino
		case 719://Diancie
		{
			if (curEdition == ORAS)
			{
				result += (0x800 * RandMinMax(0, 1));
			}
		}
		break;
	}
	return result;
}

int GetRandomPokemon(bool checkObtained)
{
	int result;

	do
	{
		result = RandMinMax(1, 721);
	}
	while (isObtained(result)&&checkObtained);
	
	return result;
}

unsigned short last = 0xFFFF;
unsigned short pok[721];

void setWildPokemonfromAddress(	u32 pokemonAddress,
								u32 pokemonVariation,
								u32 pokemonLevel,
								bool enableObtainedCheck,
								bool updatePokeRadar)
{
	if (PointerOffset != 0x00)
	{
		unsigned int ZOOffset = Read32(PointerOffset);

		//Check for valid pointer
		if ((ZOOffset >= 0x08000000) && (ZOOffset < 0x08DF0000))
		{
			//Check if ZO File contains encounter data
			if (Read32(ZOOffset + 0x10) != Read32(ZOOffset + 0x14))
			{
				//If the route has changed
				if(last == Read16(PointerOffset+4)) return;
				last = Read16(PointerOffset+4);
				
				unsigned int EncOffset = ZOOffset + Read32(ZOOffset + 0x10) + ByteJump;
				
				//Make sure the table for the dexnav is exactley the same as the one used for normal encounters	
				if(curEdition == ORAS && updatePokeRadar)
					memcpy((void*)(0x16B3df40 + Read32(0x16B3df40 + 4 + Read16(PointerOffset+4)*4) + ByteJump), (void*)EncOffset, 0xF4);
				
				int i;
				int pokemon=1;
				u32 currentEncOffset;
				for (i = 0; i < EncDataLength; i += 4)
				{
					if (Read8(EncOffset + i + 2) != 0x01)
					{
						currentEncOffset=EncOffset+i;
						//setPokemon
						if(pokemonAddress>1)
							pokemon=Read16(pokemonAddress);
						else if(pokemonAddress==1)
							pokemon=GetRandomPokemon(enableObtainedCheck);
						else
							pokemon=Read16(currentEncOffset);
						//setPokemonVariation
						if(pokemonVariation>1)
							pokemon+=0x800 *(Read16(pokemonVariation)-1);
						else if(pokemonVariation==1)
							pokemon=getRandomVariation(pokemon);
						
						//setPokemonLevel
						if(pokemonLevel>1)
						{
							u8 level=Read8(pokemonLevel);
							if(level<2)
								level=2;
							Write8(currentEncOffset+2, level);
						}
						else if(pokemonLevel==1)
							Write8(currentEncOffset+2, RandMinMax(2,100));
						//Write8(EncOffset + i+3, 1); //maybefiller?

						//Associate the pokemon id replaced with the one we use to overwrite it
						pok[Read16(currentEncOffset)-1] = pokemon;
						
						Write16(currentEncOffset, pokemon);
					}
				}

				/*Update DexNav => the dexnav is not updated when we go to a location without any possible encounter,
				 *so when we return to the previous route the dexnav does not show the good species, a battle is necessary to reload it.
				 */
				if(updatePokeRadar&&curEdition==ORAS)
				{
					int j;
					for (j = 0; j < EncDataLength; j += 4)
					{
						//Replace the pokemon id according to the one associated with it in the array
						unsigned int loc = 0x16B3df40 + Read32(0x16B3df40 + 4 + Read16(PointerOffset+4)*4) + ByteJump + j;
						if(Read16(loc) != 0) Write16(loc, pok[Read16(loc)-1]);
					
					}
				}
			}
			else
			{
				last = 0xFFFF;
			}
		}
	}
}	
