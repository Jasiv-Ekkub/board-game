#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include <raylib.h>

typedef enum LayoutAnchor {
	TOP = 0,
	LEFT = 0,

	CENTER = 1,

	BOTTOM = 2,
	RIGHT = 2,
} LayoutAnchor;

Rectangle GetRectanglePlacement(float x, float y, float width, float height, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor);
Vector2 GetVector2Placement(float x, float y, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor);
Vector2 GetTextOffset(Font font, const char* text, int fontSize, int fontSpacing, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor);

#endif //LAYOUT_ENGINE_H
