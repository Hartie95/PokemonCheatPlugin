#include "rngModifier.h"

u8 *TABLE_LOCATION;
u16 A_IDs_12bits;
u32 table[0x271];


u16 create_A_IDs_12bits(u16 TIDxSID)
{
    u16 A_IDs_12bits = 0;

    A_IDs_12bits |= __builtin_popcount(TIDxSID & 0x8b80) % 2;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x720) % 2) << 1;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x2e00) % 2) << 2;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x5c00) % 2) << 3;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x9390) % 2) << 4;
    A_IDs_12bits |= (TIDxSID & 0x1000) >> 7;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x5c0) % 2) << 6;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x9800) % 2) << 7;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0x1700) % 2) << 8;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0xb800) % 2) << 9;
    A_IDs_12bits |= (__builtin_popcount(TIDxSID & 0xd800) % 2) << 10;
    A_IDs_12bits |= (TIDxSID & 0x8000) >> 4;

    return A_IDs_12bits;
}

void initRNGModifier(int edition)
{
	switch(edition)
	{
		case PKXY:
		{
			TABLE_LOCATION = (u8*)(*(u32*)(0x5EF084 + 0x10));
			u16 TIDxSID = (*(u16*)0x08C79C3C) ^ (*(u16*)0x08C79C3E); //TID xor SID
			A_IDs_12bits = create_A_IDs_12bits(TIDxSID);
		}
		break;
		
		case ORAS:
		{
			TABLE_LOCATION = (u8*)(*(u32*)(0x63106C + 0x10));
			u16 TIDxSID = (*(u16*)0x08C81340)^ (*(u16*)0x08C81342); //TID xor SID
			A_IDs_12bits = create_A_IDs_12bits(TIDxSID);
		}
		break;
		
		default:
		{
			memset(table, 0, sizeof(u32) * 0x271);
		}
			break;
	}
}

u32 makeShinySeed(u32 i, u16 A_IDs_12bits)
{
    u32 res = i;
    res &= 0xFFFF4602;

    res |= (__builtin_popcount(A_IDs_12bits & 0x401) + __builtin_popcount(res & 0xCFFA4000)) % 2;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x4) + __builtin_popcount(res & 0xDE390200)) % 2) << 2;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x8) + __builtin_popcount(res & 0xEC384400)) % 2) << 3;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x410) + __builtin_popcount(res & 0xB1DA0000)) % 2) << 4;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x822) + __builtin_popcount(res & 0x50B24000)) % 2) << 5;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x40) + __builtin_popcount(res & 0x12E20000)) % 2) << 6;
    res |= ((__builtin_popcount(A_IDs_12bits & 0xc80) + __builtin_popcount(res & 0xCB054000)) % 2) << 7;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x900) + __builtin_popcount(res & 0x521B0000)) % 2) << 8;
    res |= ((__builtin_popcount(A_IDs_12bits & 0xc00) + __builtin_popcount(res & 0xB8700000)) % 2) << 11;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x2) + __builtin_popcount(res & 0x89124000)) % 2) << 12;
    res |= ((__builtin_popcount(A_IDs_12bits & 0xe00) + __builtin_popcount(res & 0x61000000)) % 2) << 13;
    res |= ((__builtin_popcount(A_IDs_12bits & 0x800) + __builtin_popcount(res & 0x8d010000)) % 2) << 15;

    return res;
}

void writeFullShinyTable()
{
	int i;
	for(i = 0x18e; i < 0x271; i++)
		table[i] = makeShinySeed(rand(), A_IDs_12bits);
	memcpy(TABLE_LOCATION, table, 0x9c4);
}