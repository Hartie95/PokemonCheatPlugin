#include "statsModifier.h"

#define HP_JUMP 0xE
#define PP_JUMP 0xF4 + 0x22

#define EXP_JUMP_1 -0x278
#define EXP_JUMP_2 -0x200 //Unused

u32  *PARTY_1 = 0, 
	 *PARTY_2 = 0,
	 *OPPONENT_1 = 0,
	 *OPPONENT_2 = 0,
	 *EXP = 0;


void initStatsModifier(int edition)
{
	switch(edition)
	{
		case PKXY:
		{
			PARTY_1 = (u32*)0x081FB284;
			PARTY_2 = (u32*)0x081FB624;
			OPPONENT_1 = (u32*)0x081FB2A0;
			OPPONENT_2 = (u32*)0x081FB640;
			EXP = (u32*)0x081FBA20;
		}
		break;
		
		case ORAS:
		{
			PARTY_1 = (u32*)0x081FB58C;
			PARTY_2 = (u32*)0x081FB92C;
			OPPONENT_1 = (u32*)0x081FB5A8;
			OPPONENT_2 = (u32*)0x081FB948;
			EXP = (u32*)0x081FBD28;
		}
		break;
		
		default:
			break;
	}
}

void setMaxHP(u32* first, u32* second, u16 HP)
{
	if(*first > 0x08000000 && *first < 0x08DF0000 && *second > 0x08000000 && *second < 0x08DF0000 && *(vu32*)(*first+ HP_JUMP) == *(vu32*)(*second+ HP_JUMP))
	{
		*(vu16*)(*first + HP_JUMP) = HP;
		*(vu16*)(*second + HP_JUMP) = HP;
	}	
}

void setCurrentHP(u32* first, u32* second, u16 HP)
{
	if(*first > 0x08000000 && *first < 0x08DF0000 && *second > 0x08000000 && *second < 0x08DF0000 && *(vu32*)(*first+ HP_JUMP) == *(vu32*)(*second+ HP_JUMP))
	{
		*(vu16*)(*first + HP_JUMP + 0x2) = HP;
		*(vu16*)(*second + HP_JUMP + 0x2) = HP;
	}
}

void setPartyMaxHP(u16 HP)
{
	setMaxHP(PARTY_1, PARTY_2, HP);
}

void setOpponentMaxHP(u16 HP)
{
	setMaxHP(OPPONENT_1, OPPONENT_2, HP);
}

void setPartyCurrentHP(u16 HP)
{
	setCurrentHP(PARTY_1, PARTY_2, HP);
}

void setOpponentCurrentHP(u16 HP)
{
	setCurrentHP(OPPONENT_1, OPPONENT_2, HP);
}

void setAllPPMax()
{
	if(*PARTY_1 < 0x08000000 || *PARTY_1 > 0x08DF0000 || *PARTY_2 < 0x08000000 || *PARTY_2 > 0x08DF0000)
		return;
	
	u8* first = (u8*)(*PARTY_1 + PP_JUMP + 0x2);
	u8* second = (u8*)(*PARTY_2 + PP_JUMP + 0x2);
	
	if(*first != *second)
		return;
	
	int i;
	for(i = 0; i < 4; i++, first += 0x8, second += 0x8)
	{
		*first = *(first + 0x1);
		*second = *(second + 0x1);
		
		first += 0x6;
		second += 0x6;
		
		*first = *(first + 0x1);
		*second = *(second + 0x1);
	}
}

void setGainExp(u32 exp)
{
	if(*EXP > 0x08000000 && *EXP < 0x08DF0000)
	{
		*(vu32*)(*EXP + EXP_JUMP_1) = exp;
		//*(vu32*)(*EXP + EXP_JUMP_2) = exp;
	}
}