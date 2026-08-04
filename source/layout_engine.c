#include <layout_engine.h>

Rectangle GetRectanglePlacement(float x, float y, float width, float height, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor, GameContext gameContext)
{
	x *= gameContext.guiScale;
	y *= gameContext.guiScale;
	width *= gameContext.guiScale;
	height *= gameContext.guiScale;

	return (Rectangle){
		x + (gameContext.screenWidth - width) * (float)horizontalAnchor / 2,
		y + (gameContext.screenHeight - height) * (float)verticalAnchor / 2,
		width,
		height,
	};
}
