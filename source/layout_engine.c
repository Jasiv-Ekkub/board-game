#include <layout_engine.h>
#include <game_context.h>

Rectangle GetRectanglePlacement(float x, float y, float width, float height, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor)
{
	float guiScale = GetGuiScale();

	x *= guiScale;
	y *= guiScale;
	width *= guiScale;
	height *= guiScale;

	Rectangle screenBounds = GetScreenBounds();

	return (Rectangle){
		x + (screenBounds.width - width) * (float)horizontalAnchor / 2,
		y + (screenBounds.height - height) * (float)verticalAnchor / 2,
		width,
		height,
	};
}
