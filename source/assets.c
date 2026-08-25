#include <assets.h>
#include <board_rendering.h>

Shader shader = {0};
Sound sounds[SOUND_COUNT] = {0};
Model models[MODEL_COUNT] = {0};
Image images[IMAGE_COUNT] = {0};
Texture textures[TEXTURE_COUNT] = {0};

void LoadAssets()
{
	sounds[UI_CLICK_SOUND] = LoadSound("resource/sound/ui_blip.wav");

	shader = LoadShader("resource/shader/shader.vs", "resource/shader/shader.fs");

	textures[BOARD_TEXTURE] = LoadTextureDefault();

	models[BOARD_MODEL] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 4));
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
	
	images[START_IMAGE] = LoadImageDefault();
	images[PRISON_IMAGE] = LoadImageDefault();
	images[PARKING_IMAGE] = LoadImageDefault();
	images[POLICEMAN_IMAGE] = LoadImageDefault();
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
}
