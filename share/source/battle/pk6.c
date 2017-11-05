#include "pk6.h"
#include "string.h"

u8 aloc[] = {0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3};
u8 bloc[] = {1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2};
u8 cloc[] = {2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1};
u8 dloc[] = {3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0};

Result shuffleArray(u8 *array, u8 sv)
{
  u8 ord[] = {aloc[sv], bloc[sv], cloc[sv], dloc[sv]};
  u8 pkmcpy[232];
  u8 tmp[56];

  memcpy(pkmcpy, array, 232);
  
  int i = 0;
  for (i = 0; i < 4; i++)
  {
    memcpy(tmp, pkmcpy + 8 + 56 * ord[i], 56);
    memcpy(array + 8 + 56 * i, tmp, 56);
  }
  
  return 0;	
}

u32 seedStep(u32 seed)
{
  u32 a = 0x41C64E6D;
  u32 c = 0x00006073;

  return ((seed * a + c) & 0xFFFFFFFF);
}


u16 checksum(PK6 *data, u32 len)
{
    u16 chk = 0;
	u16* data_16 = (u16*)data;
	
	u32 i;
    for (i = 4; i < len/2; i ++)
        chk += data_16[i];
		
    return chk;
}

Result decryptPokemon(u8 *enc, PK6 *res)
{
  u8* dec = (u8*)res;
  memcpy(dec, enc, 232);

  u32 pv = res->encryptionKey;
  u8 sv = (((pv & 0x3E000) >> 0xD) % 24);

  u32 seed = pv;
  u16 tmp;
  
  int i = 0;
  for (i = 8; i < 232; i += 2)
  {
    memcpy(&tmp, dec + i, 2);
    tmp ^= (seedStep(seed) >> 16);
    seed = seedStep(seed);
    memcpy(dec + i, &tmp, 2);
  }
  shuffleArray((u8*)dec, sv);
  
  return checksum(res, 232) == ((u16*)enc)[3];
}

Result encryptPokemon(PK6 *dec, u8 *enc)
{
  u32 pv = dec->encryptionKey;
  u32 sv = (((pv & 0x3E000) >> 0xD) % 24);
  u16 tmp;

  int i = 0;
  memcpy(enc, dec, 232);
  for(i = 0; i < 11; i++)
    shuffleArray((u8*)enc, sv);

  u32 seed = pv;
  for(i = 8; i < 232; i += 2)
  {
    memcpy(&tmp, enc + i, 2);
    tmp ^= (seedStep(seed) >> 16);
    seed = seedStep(seed);
    memcpy(enc + i, &tmp, 2);
  }
  
  return 0;
}

Result decryptBattleStats(u32 key, PK6 *enc, PK6 *dec)
{
    memcpy(dec, enc, 0x1C);
    u32 seed = key;
    u16 tmp;
	
	int i;
    for (i = 0; i < 0x1C; i += 2)
    {
        memcpy(&tmp, dec + i, 2);
        tmp ^= (seedStep(seed) >> 16);
        seed = seedStep(seed);
        memcpy(dec + i, &tmp, 2);
    }
	
    return 0;
}