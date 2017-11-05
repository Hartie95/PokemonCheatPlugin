#ifndef RNGMODIFIER_H
#define RNGMODIFIER_H

#include "../helpers.h"

void initRNGModifier(int edition);

u16 create_A_IDs_12bits(u16 TIDxSID);
u32 makeShinySeed(u32 i, u16 A_IDs_12bits);

void writeFullShinyTable();

#endif