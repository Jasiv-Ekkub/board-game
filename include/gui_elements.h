#ifndef GUI_ELEMENTS
#define GUI_ELEMENTS

#include <raylib.h>
#include <player.h>

void GuiBoxText(Rectangle bounds, const char* text);
void GuiPlayerInfo(Rectangle bounds, Player player);
void GuiGameOver(Rectangle bounds, Player winner);

#endif //GUI_ELEMENTS
