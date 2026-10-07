#include <layout_engine.h>
#include <game_context.h>

Rectangle GetRectanglePlacement(float x, float y, float width, float height, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor)
{
	Rectangle screenBounds = GetScreenBounds();

	float guiScale = GetGuiScale() * screenBounds.height / 810;

	x *= guiScale;
	y *= guiScale;
	width *= guiScale;
	height *= guiScale;


	return (Rectangle){
		x + (screenBounds.width - width) * (float)horizontalAnchor / 2,
		y + (screenBounds.height - height) * (float)verticalAnchor / 2,
		width,
		height,
	};
}

Vector2 GetVector2Placement(float x, float y, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor)
{
	Rectangle screenBounds = GetScreenBounds();

	float guiScale = GetGuiScale() * screenBounds.height / 810;

	x *= guiScale;
	y *= guiScale;

	return (Vector2){
		x + screenBounds.width * (float)horizontalAnchor / 2,
		y + screenBounds.height * (float)verticalAnchor / 2,
	};
}

Vector2 GetTextOffset(Font font, const char* text, int fontSize, int fontSpacing, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor)
{
	Vector2 offset = MeasureTextEx(font, text, fontSize, fontSpacing);

	return (Vector2){
		offset.x * (float)horizontalAnchor / 2,
		offset.y * (float)verticalAnchor / 2,
	};
}
