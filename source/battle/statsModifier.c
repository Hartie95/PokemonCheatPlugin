#include "statsModifier.h"

#define HP_JUMP 0xE
#define PP_JUMP 0xF4 + 0x22

u32  *POINTER_1 = 0, 
	 *POINTER_2 = 0;

void initStatsModifier(int edition)
{
	switch(edition)
	{
		case PKXY:
		{
			POINTER_1 = (u32*)0x081FB284;
			POINTER_2 = (u32*)0x081FB624;
		}
		break;
		
		case ORAS:
		{
			POINTER_1 = (u32*)0x081FB58C;
			POINTER_2 = (u32*)0x081FB92C;
		}
		break;
		
		default:
			break;
	}
}

void setMaxHP(u16 HP)
{
	if(*POINTER_1 > 0x08000000 && *POINTER_1 < 0x08DF0000 && *POINTER_2 > 0x08000000 && *POINTER_2 < 0x08DF0000 && *(vu32*)(*POINTER_1+ HP_JUMP) == *(vu32*)(*POINTER_2+ HP_JUMP))
	{
		*(vu16*)(*POINTER_1 + HP_JUMP) = HP;
		*(vu16*)(*POINTER_2 + HP_JUMP) = HP;
	}
}

void setCurrentHP(u16 HP)
{
	if(*POINTER_1 > 0x08000000 && *POINTER_1 < 0x08DF0000 && *POINTER_2 > 0x08000000 && *POINTER_2 < 0x08DF0000 && *(vu32*)(*POINTER_1+ HP_JUMP) == *(vu32*)(*POINTER_2+ HP_JUMP))
	{
		*(vu16*)(*POINTER_1 + HP_JUMP + 0x2) = HP;
		*(vu16*)(*POINTER_2 + HP_JUMP + 0x2) = HP;
	}
}

void setAllPPMax()
{
	if(*POINTER_1 < 0x08000000 || *POINTER_1 > 0x08DF0000 || *POINTER_2 < 0x08000000 || *POINTER_2 > 0x08DF0000)
		return;
	
	u8* first = (u8*)(*POINTER_1 + PP_JUMP + 0x2);
	u8* second = (u8*)(*POINTER_2 + PP_JUMP + 0x2);
	
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