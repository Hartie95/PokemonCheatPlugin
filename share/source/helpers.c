#include "helpers.h"

u32 IoBasePad = 0xFFFD4000;

u32 getKey() {
	return (*(vu32*)(IoBasePad) ^ 0xFFF) & 0xFFF;
}

void waitKeyUp() {
	while (getKey() != 0) {
		svc_sleepThread(100000000);
	}
}
void waitKeyCombinationChanged(u32 buttonCombination) {
	while (getKey() == buttonCombination) {
		svc_sleepThread(100000000);
	}
}

int getEdition()
{
	int ret=UNKNOWNGAME;
	switch (Read32(0x0700010C))
	{
		//XY
		case 0x00055D00:
		case 0x00055E00:
			ret = PKXY;
			break;

		//ORAS
		case 0x0011C400:
		case 0x0011C500:
			ret = ORAS;
			break;
	}
	return ret;
}
