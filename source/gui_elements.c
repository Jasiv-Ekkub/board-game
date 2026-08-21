#include <gui_elements.h>
#include <raygui.h>
#include <stdio.h>

void GuiDrawBox(Rectangle bounds)
{
	int border = GuiGetStyle(DEFAULT, BORDER_WIDTH);
	DrawRectangleRec(bounds, GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
	DrawRectangleRec(
		(Rectangle){
				bounds.x + border,
				bounds.y + border,
				bounds.width - border*2,
				bounds.height - border*2
			},
		GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL))
	);

}

void DrawTextCentered(const char* text, Vector2 position, Color color)
{
	Font font = GuiGetFont();
	int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
	int textSpacing = GuiGetStyle(DEFAULT, TEXT_SPACING);

	Vector2 offset = MeasureTextEx(font, text, textSize, textSpacing);
	offset.x /= 2;
	offset.y /= 2;
	DrawTextPro(font, text, position, offset, 0, textSize, textSpacing, color);
}

void GuiBoxText(Rectangle bounds, const char* text)
{
	GuiDrawBox(bounds);
	DrawTextCentered(text, (Vector2){bounds.x + bounds.width/2, bounds.y + bounds.height/2}, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)));
}

void GuiPlayerInfo(Rectangle bounds, Player player)
{
	GuiDrawBox(bounds);

	DrawTextCentered(player.name, (Vector2){bounds.x + bounds.width/2, bounds.y + bounds.height/3}, player.color);

	char buffer[32];
	if(player.money < 0)
	{
		snprintf(buffer, 32, "Bankrupt");
	}
	else
	{
		snprintf(buffer, 32, "$%i", player.money);
	}
	DrawTextCentered(buffer, (Vector2){bounds.x + bounds.width/2, bounds.y + bounds.height/3 * 2}, player.color);
}

void GuiGameOver(Rectangle bounds, Player winner)
{
	GuiDrawBox(bounds);
	Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	DrawTextCentered("GAME OVER", (Vector2){bounds.x + bounds.width/2, bounds.y + bounds.height/4}, textColor);
	DrawTextCentered("The winner is:", (Vector2){bounds.x + bounds.width/2, bounds.y + bounds.height*2/4}, textColor);
	char buffer[64];
	snprintf(buffer, 64, "%s $%i", winner.name, winner.money);
	DrawTextCentered(buffer, (Vector2){bounds.x + bounds.width/2, bounds.y + bounds.height*3/4}, winner.color);
}
