#ifndef STATSMODIFIER_H
#define STATSMODIFIER_H

#include "../helpers.h"

void initStatsModifier(int edition);
void setPartyMaxHP(u16 HP);
void setPartyCurrentHP(u16 HP);
void setOpponentMaxHP(u16 HP);
void setOpponentCurrentHP(u16 HP);
void setAllPPMax();

void setGainExp(u32 exp);

#endif