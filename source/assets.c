#include <assets.h>
#include <board_rendering.h>
#include <raygui.h>

Shader shader = {0};
Sound sounds[SOUND_COUNT] = {0};
Model models[MODEL_COUNT] = {0};
Image images[IMAGE_COUNT] = {0};
Texture textures[TEXTURE_COUNT] = {0};
Font font = {0};

void LoadAssets()
{
	sounds[UI_CLICK_SOUND] = LoadSound("resource/sound/ui_blip.wav");
	sounds[DING_SOUND] = LoadSound("resource/sound/end_round_ding.wav");
	sounds[DICE_HIT_SOUND] = LoadSound("resource/sound/dice_hit.mp3");
	sounds[POPUP_SOUND] = LoadSound("resource/sound/popup.wav");
	sounds[KA_CHING_SOUND] = LoadSound("resource/sound/ka-ching.mp3");

	font = LoadFontEx("resource/font/nihonium113.regular.ttf", 140, 0, 0);
	shader = LoadShader("resource/shader/shader.vs", "resource/shader/shader.fs");

	textures[BOARD_TEXTURE] = LoadTextureDefault();
	textures[DICE_TEXTURE] = LoadTexture("resource/texture/dice.png");

	models[BOARD_MODEL] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 4));
	models[DICE_MODEL] = LoadModel("resource/model/dice.glb");
	models[PAWN_MODEL] = LoadModel("resource/model/pawn.glb");
	models[SITE_MODEL] = LoadModel("resource/model/placeholder_box.glb");
	models[HOUSE_MODEL] = LoadModel("resource/model/placeholder_cone.glb");
	models[VILLA_MODEL] = LoadModel("resource/model/placeholder_sphere.glb");
	models[APARTAMENT_MODEL] = LoadModel("resource/model/placeholder_cylinder.glb");
	for(int i=0; i<MODEL_COUNT; ++i)
	{
		if(IsModelValid(models[i]))
		{
			models[i].materials[0].shader = shader;
		}
	}
	models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textures[BOARD_TEXTURE];
	models[DICE_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textures[DICE_TEXTURE];
	
	images[START_IMAGE] = LoadImageDefault();
	images[TAXATION_IMAGE] = LoadImageDefault();
	images[LOTTERY_IMAGE] = LoadImage("resource/texture/fortune.png");
	images[POLICEMAN_IMAGE] = LoadImageDefault();
	images[CHANCE_IMAGE] = LoadImage("resource/texture/question_mark.png");
}

void UnloadAssets()
{
	for(int i=0; i<SOUND_COUNT; ++i)
	{
		UnloadSound(sounds[i]);
	}
	for(int i=0; i<MODEL_COUNT; ++i)
	{
		UnloadModel(models[i]);
	}
	for(int i=0; i<IMAGE_COUNT; ++i)
	{
		UnloadImage(images[i]);
	}
	for(int i=0; i<TEXTURE_COUNT; ++i)
	{
		UnloadTexture(textures[i]);
	}
	UnloadShader(shader);
	UnloadFont(font);
}
