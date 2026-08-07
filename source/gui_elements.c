#include <gui_elements.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <style_buisness.h>


void LoadStyle()
{
	GuiLoadStyleBuisness();
}

void GuiPlayerInfo(Rectangle bounds, Player player)
{
	GuiDrawRectangle(bounds,
		GuiGetStyle(DEFAULT, BORDER_WIDTH),
		Fade(GetColor(GuiGetStyle(DEFAULT, BORDER)), guiAlpha),
		GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL))
	);

	float dh = bounds.height / 2;
	char buffer[32];

	Rectangle textBounds = {bounds.x, bounds.y, bounds.width, dh};
	GuiDrawText(player.name, GetTextBounds(DEFAULT, textBounds), TEXT_ALIGN_CENTER, Fade(player.color, guiAlpha));
	textBounds.y += dh;
	snprintf(buffer, 32, "$%i", player.money);
	GuiDrawText(buffer, GetTextBounds(DEFAULT, textBounds), TEXT_ALIGN_CENTER, Fade(GetColor(GuiGetStyle(DEFAULT, TEXT)), guiAlpha));
}
