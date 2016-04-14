#include "otherModifiers.h"

unsigned int MoneyPointerOffset=0;
unsigned int RepelPointerOffset=0;
unsigned int OPowerPointerOffset=0;

vu32* HATCHING_STEP_COUNTER;


void initOtherModifiers(int edition)
{
	switch (edition)
	{
		//XY
		case PKXY:
		{
			MoneyPointerOffset  = 0x08C6A6AC;
			RepelPointerOffset  = 0x08C7D23A;
			OPowerPointerOffset = 0x08C7BB64;
			
			HATCHING_STEP_COUNTER = (vu32*)0x08C68261;
		}
		break;

		//ORAS
		case ORAS:
		{
			MoneyPointerOffset  = 0x08C71DC0;
			RepelPointerOffset  = 0x08C8546E;
			OPowerPointerOffset = 0x08C83D94;
			
			HATCHING_STEP_COUNTER = (vu32*)0x08C6F975;
		}
		break;
	}
}

void setMoney(u32 count)
{
	(*(vu32*)(MoneyPointerOffset))=count;
}

void setRemainingRepel(u8 count)
{
	(*(vu8*)(RepelPointerOffset))=count;
}

void setRemainingOPower(u8 count)
{
	if(OPowerPointerOffset!=0)
		(*(vu8*)(OPowerPointerOffset))=count;
}

void setHatchingStepCounter(u16 steps)
{
	*HATCHING_STEP_COUNTER = steps;
}