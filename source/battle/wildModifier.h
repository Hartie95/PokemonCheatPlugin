#ifndef WILDMODIFIER_H
#define WILDMODIFIER_H

#include "pk6.h"
#include "../helpers.h"

void initWildModifier(int edition);

void updateChecksum(PK6* in);

Result getCurrentPokemon(PK6* out);
void setCurrentPokemon(PK6* in);

void makeShiny(PK6* pk6);
void setIV(PK6* pk6, Stats s, u8 amount);

void setAutomaticCatchSuccess();

#endif