#ifndef MENU_H
#define MENU_H

#include "helpers.h"

extern GAME_PLUGIN_MENU gamePluginMenu;

void initMenu();
void addMenuEntry(u8* str);
void addCheatMenuEntry(u8* str);
void addOrCheatMenuEntry(u8* str);
u32 updateMenu();

#endif