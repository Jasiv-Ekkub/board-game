#define RAYGUI_IMPLEMENTATION
#define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT 60
#include <raygui.h>
#include <style_buisness.h>

#include <gui_elements.h>
#include <stdio.h>
#include <assets.h>

void LoadStyle()
{
	GuiLoadStyleBuisness();
}


void GuiBoxText(Rectangle bounds, const char* text)
{
	GuiPanel(bounds, 0);
	GuiDrawText(text, bounds, 1, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}

void GuiPlayerInfo(Rectangle bounds, Player player)
{
	GuiPanel(bounds, 0);

	Rectangle tmp = bounds;
	tmp.height /= 2;
	tmp.y += 20;
	tmp.height -= 20;

	GuiDrawText(player.name, tmp, 1, player.color);

	char buffer[32];
	if(player.money < 0)
	{
		snprintf(buffer, 32, "Bankrupt");
	}
	else
	{
		snprintf(buffer, 32, "$%i", player.money);
	}
	tmp.y += tmp.height;
	GuiDrawText(buffer, tmp, 1, player.color);
}

void GuiGameOver(Rectangle bounds, Player winner)
{
	GuiPanel(bounds, 0);

	Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	
	Rectangle tmp = bounds;
	tmp.height /= 2;
	tmp.y += 20;
	tmp.height -= 20;

	GuiDrawText("GAME OVER\nThe winner is:", tmp, 1, textColor);
	
	char buffer[64];
	tmp.y += tmp.height;
	snprintf(buffer, 64, "%s $%i", winner.name, winner.money);
	GuiDrawText(buffer, tmp, 1, winner.color);
}

int GuiMessageBoxSfx(Rectangle bounds, const char* message, const char* options)
{
	int response = GuiMessageBox(bounds, 0, message, options);
	if(response != -1) PlaySound(sounds[UI_CLICK_SOUND]);
	return response;
}

int GuiButtonSfx(Rectangle bounds, const char* text)
{
	int response = GuiButton(bounds, text);
	if(response) PlaySound(sounds[UI_CLICK_SOUND]);
	return response;
}

int GuiSpinnerSfx(Rectangle bounds, const char *text, int *value, int minValue, int maxValue)
{
	int prev = *value;
	if(GuiSpinner(bounds, text, value, minValue, maxValue, 0) || prev != *value)
	{
		PlaySound(sounds[UI_CLICK_SOUND]);
		return 1;
	}
	return 0;
}
