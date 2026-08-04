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

Vector2 GetVector2Placement(float x, float y, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor, GameContext gameContext)
{
	return (Vector2){
		x * gameContext.guiScale + gameContext.screenWidth * (float)horizontalAnchor / 2,
		y * gameContext.guiScale + gameContext.screenHeight * (float)verticalAnchor / 2,
	};
}
