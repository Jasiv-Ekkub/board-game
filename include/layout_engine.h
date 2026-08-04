#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include <raylib.h>
#include <game_context.h>

typedef enum LayoutAnchor {
	TOP = 0,
	LEFT = 0,

	CENTER = 1,

	BOTTOM = 2,
	RIGHT = 2,
} LayoutAnchor;

Rectangle GetRectanglePlacement(float x, float y, float width, float height, LayoutAnchor horizontalAnchor, LayoutAnchor verticalAnchor, GameContext gameContext);

#endif //LAYOUT_ENGINE_H
