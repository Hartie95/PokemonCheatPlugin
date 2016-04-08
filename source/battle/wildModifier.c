#include "wildModifier.h"

u8	 	*PK6_LOCATION_1 = 0, 
		*PK6_LOCATION_2 = 0;
u16		*TID = 0,
		*SID = 0;
		
u32		*CATCH_POINTER = 0,
		CATCH_CHECK_ADDRESS = 0;
			 
void initWildModifier(int edition)
{
	switch(edition)
	{
		case PKXY:
		{
			PK6_LOCATION_1 = (u8*)0x63f890;
			PK6_LOCATION_2 = (u8*)0x081FF744;
			TID = (u16*)0x08C79C3C;
			SID = (u16*)0x08C79C3E;
			CATCH_POINTER = (u32*)0x081FBA18;
			CATCH_CHECK_ADDRESS = 0x081FB1F8;
		}
		break;
		
		case ORAS:
		{
			PK6_LOCATION_1 = (u8*)0x081FEEC8;
			PK6_LOCATION_2 = (u8*)0x081FFA6C;
			TID = (u16*)0x08C81340;
			SID = (u16*)0x08C81342;
			CATCH_POINTER = (u32*)0x081FBD20;
			CATCH_CHECK_ADDRESS = 0x081FB500;
		}
		break;
		
		default:
			break;
	}
}

void updateChecksum(PK6* in)
{
	in->checksum = checksum(in, 232);
}

Result getCurrentPokemon(PK6* out)
{
	return decryptPokemon(PK6_LOCATION_1, out);
}

void setCurrentPokemon(PK6* in)
{
	updateChecksum(in);
	encryptPokemon(in, PK6_LOCATION_1);
	encryptPokemon(in, PK6_LOCATION_2);
}

void makeShiny(PK6* pk6)
{
	u16 s_xor = (*TID ^ *SID ^ (u16)(pk6->PID & 0x0000FFFF)) & 0xFFF0;
	pk6->PID = (pk6->PID & 0x000FFFFF) | (s_xor << 16);
}

void setIV(PK6* pk6, Stats s, u8 amount)
{
	if(amount > 31) return;
	pk6->IVs = (pk6->IVs & ~(0xFF << (5 * s))) | (amount << (5 * s));
}

void setAutomaticCatchSuccess()
{
	if(*CATCH_POINTER > 0x08000000 && *CATCH_POINTER < 0x08DF0000)
	{
		u32* first = (u32*)(*CATCH_POINTER + 0x4C);
		first = (u32*)(*first + 0x4);
		
		if(*first == CATCH_CHECK_ADDRESS)
		{							
			first = (u32*)(CATCH_CHECK_ADDRESS + 0x10);
			u8* second = (u8*)(*first + 0xB2);
			
			*second = 0x1;
		}
	}
}

u8 isShiny(PK6* pk6)
{
	return (*TID ^ *SID ^ (u16)(pk6->PID & 0x0000FFFF) ^ (u16)((pk6->PID & 0xFFFF0000) >> 16)) < 16;
}