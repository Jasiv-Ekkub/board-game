#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

typedef enum SoundId
{
	UI_CLICK_SOUND=0,
	DING_SOUND,
	DICE_HIT_SOUND,
	POPUP_SOUND,
	KA_CHING_SOUND,
	//===
	SOUND_COUNT
} SoundId;
extern Sound sounds[SOUND_COUNT];

typedef enum ModelId
{
	BOARD_MODEL=0,
	DICE_MODEL,
	PAWN_MODEL,
	SITE_MODEL,
	HOUSE_MODEL,
	VILLA_MODEL,
	APARTAMENT_MODEL,
	//===
	MODEL_COUNT,
} ModelId;
extern Model models[MODEL_COUNT];

typedef enum ImageId
{
	START_IMAGE=0,
	PRISON_IMAGE,
	PARKING_IMAGE,
	POLICEMAN_IMAGE,
	//===
	IMAGE_COUNT,

} ImageId;
extern Image images[IMAGE_COUNT];

typedef enum TextureId
{
	BOARD_TEXTURE=0,
	DICE_TEXTURE,
	//===
	TEXTURE_COUNT,

} TextureId;
extern Texture textures[TEXTURE_COUNT];

extern Font font;
void LoadAssets();
void UnloadAssets();

#endif //ASSETS_H
