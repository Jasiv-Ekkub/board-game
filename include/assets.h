#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

typedef enum SoundId
{
	UI_CLICK_SOUND=0,
	//===
	SOUND_COUNT
} SoundId;
extern Sound sounds[SOUND_COUNT];

typedef enum ModelId
{
	BOARD_MODEL=0,
	PAWN_MODEL,
	SITE_MODEL,
	HOUSE_MODEL,
	VILLA_MODEL,
	APARTAMENT_MODEL,
	//===
	MODEL_COUNT,
} ModelId;
extern Model models[MODEL_COUNT];

void LoadAssets();
void UnloadAssets();

#endif //ASSETS_H
