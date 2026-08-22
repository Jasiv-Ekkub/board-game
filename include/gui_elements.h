#ifndef GUI_ELEMENTS
#define GUI_ELEMENTS

#include <raylib.h>
#include <player.h>

void GuiBoxText(Rectangle bounds, const char* text);
void GuiPlayerInfo(Rectangle bounds, Player player);
void GuiGameOver(Rectangle bounds, Player winner);
int GuiMessageBoxSfx(Rectangle bounds, const char* message, const char* options);
int GuiButtonSfx(Rectangle bounds, const char* text); 
#endif //GUI_ELEMENTS
