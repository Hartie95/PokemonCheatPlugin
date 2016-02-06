#include "otherModifiers.h"

unsigned int MoneyPointerOffset=0;
unsigned int RepelPointerOffset=0;
unsigned int OPowerPointerOffset=0;

void initOtherModifiers(int edition)
{
	switch (edition)
	{
		//XY
		case PKXY:
		{
			MoneyPointerOffset = 0x08C6A6AC;
			RepelPointerOffset = 0x08C8546C;
			OPowerPointerOffset = 0x0;
		}
		break;

		//ORAS
		case ORAS:
		{
			MoneyPointerOffset = 0x08C71DC0;
			RepelPointerOffset = 0x08C8546C ;
			OPowerPointerOffset = 0x8C83D94;
		}
		break;
	}
}
void setMoney(u32 count)
{
	(*(vu32*)(MoneyPointerOffset))=count;
}
void setRemainingRepel(u32 count)
{
	(*(vu32*)(RepelPointerOffset))=count;
}
void setRemainingOPower(u32 count)
{
	if(OPowerPointerOffset!=0)
		(*(vu16*)(OPowerPointerOffset))=count;
}