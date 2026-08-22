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

void LoadAssets();
void UnloadAssets();

#endif //ASSETS_H
