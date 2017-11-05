#ifndef HELPERS_H
#define HELPERS_H

#include "global.h"

#define UNKNOWNGAME 0
#define PKXY 1
#define ORAS 2

#define Write8(addr, data) *(volatile unsigned char*)(addr) = data
#define Write16(addr, data) *(volatile unsigned short*)(addr) = data
#define Write32(addr, data) *(volatile unsigned int*)(addr) = data
#define WRITEU8(addr, data) *(vu8*)(addr) = data
#define WRITEU16(addr, data) *(vu16*)(addr) = data
#define WRITEU32(addr, data) *(vu32*)(addr) = data

#define Read8(addr) *(volatile unsigned char*)(addr)
#define Read16(addr) *(volatile unsigned short*)(addr)
#define Read32(addr) *(volatile unsigned int*)(addr)

#define IO_BASE_PAD		1
#define IO_BASE_LCD		2
#define IO_BASE_PDC		3
#define IO_BASE_GSPHEAP		4

extern u32 IoBasePad;

u32 plgGetIoBase(u32 IoType);

u32 getKey();
void waitKeyUp();
void waitKeyCombinationChanged(u32 buttonCombination);

int getEdition();

#endif