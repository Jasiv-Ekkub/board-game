#include <assets.h>

Sound sounds[SOUND_COUNT] = {0};

void LoadAssets()
{
	sounds[UI_CLICK_SOUND] = LoadSound("resource/sound/ui_blip.wav");
}

void UnloadAssets()
{
	for(int i=0; i<SOUND_COUNT; ++i)
	{
		UnloadSound(sounds[i]);
	}
}
