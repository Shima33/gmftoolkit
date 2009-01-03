#include "cGMF.h"
#include "cGMFTypes.h"

int cGMF::readMain(int id, int flag, int size)
{
	if (id == 1 && flag == 0)
		modelType = size;

	if (id == 1 && flag == 2)
		readSceneInfo(size);

	if (id == 7 && flag  == 2)
		materialList = readMaterialList(size);

	if (id == 18 && flag == 2)
		objectList = readObjectList(size);

	return 0;
}

int cGMF::readSceneInfo(int size)
{
	scene = new cGMFScene();
	scene->name = getString();
	scene->firstFrame = getInteger();
	scene->lastFrame = getInteger();
	scene->frameSpeed = getInteger();
	scene->ticksPerFrame = getInteger();
	scene->backgroundStatic.red = *(unsigned char*)getBytes(1);
	scene->backgroundStatic.green = *(unsigned char*)getBytes(1);
	scene->backgroundStatic.blue = *(unsigned char*)getBytes(1);
	getBytes(1);
	scene->ambientStatic.red = *(unsigned char*)getBytes(1);
	scene->ambientStatic.green = *(unsigned char*)getBytes(1);
	scene->ambientStatic.blue = *(unsigned char*)getBytes(1);
	getBytes(1);
	return 0;
}

cGMFMaterialList* cGMF::readMaterialList(int size)
{
	cGMFMaterialList* list = new cGMFMaterialList();
	list->number = getInteger();

	list->materials = new cGMFMaterial *[list->number];

	int i;
	for (i = 0; i < list->number; i++)
	{
		int id, flag, size;
		id = getInteger();
		flag = getInteger();
		size = getInteger();
		list->materials[i] = readMaterial(size);
	}
	return list;
}

cGMFMaterial* cGMF::readMaterial(int size)
{
	cGMFMaterial* material = new cGMFMaterial();
	material->ref = getInteger();
	material->name = getString();
	material->classname = getString();
	material->ambient.red = *(unsigned char*)getBytes(1);
	material->ambient.green = *(unsigned char*)getBytes(1);
	material->ambient.blue = *(unsigned char*)getBytes(1);
	getBytes(1);
	material->diffuse.red = *(unsigned char*)getBytes(1);
	material->diffuse.green = *(unsigned char*)getBytes(1);
	material->diffuse.blue = *(unsigned char*)getBytes(1);
	getBytes(1);
	material->specular.red = *(unsigned char*)getBytes(1);
	material->specular.green = *(unsigned char*)getBytes(1);
	material->specular.blue = *(unsigned char*)getBytes(1);
	getBytes(1);
	material->shine = getFloat();
	material->shineStrength = getFloat();
	material->transparency = getFloat();
	material->wiresize = getFloat();
	material->shading = getInteger();
	material->XPFallof = getFloat();
	material->selfIllum = getFloat();
	material->fallof = getInteger();
	material->XPType = getInteger();

	material->texNum = getInteger();
	if (material->texNum > 0)
	{
		int id, flags, size;
		id = getInteger();
		flags = getInteger();
		size = getInteger();
		material->textureList = readTextureList(size);
	}

	material->matNum = getInteger();
	if (material->matNum > 0)
	{
		int id, flags, size;
		id = getInteger();
		flags = getInteger();
		size = getInteger();
		material->materialList = readMaterialList(size);
	}

	return material;
}

cGMFTexture* cGMF::readTexture(int size)
{
	cGMFTexture* texture = new cGMFTexture();
	texture->name = getString();
	texture->classname = getString();
	texture->bitmap = getString();
	texture->amount = getFloat();
	texture->style = getInteger();
	texture->type = getInteger();
	texture->UO = getFloat();
	texture->VO = getFloat();
	texture->UT = getFloat();
	texture->VT = getFloat();
	texture->UVWAngle = getFloat();
	texture->UVWBlur = getFloat();
	texture->UVWBlurOffset = getFloat();
	texture->UVWNoiseAmount = getFloat();
	texture->UVWNoiseSize = getFloat();
	texture->UVWNoiseLevel = getInteger();
	texture->UVWNoisePhase = getFloat();

	getBytes(8);

	texture->bitmapFilter = getInteger();
	texture->bitmapChannel = getInteger();

	getBytes(4);

	return texture;
}

cGMFTextureList* cGMF::readTextureList(int size)
{
	cGMFTextureList* list = new cGMFTextureList();
	list->number = getInteger();

	list->textures = new cGMFTexture *[list->number];

	int i;
	for (i = 0; i < list->number; i++)
	{
		int id, flags, size;
		id = getInteger();
		flags = getInteger();
		size = getInteger();
		list->textures[i] = readTexture(size);
	}

	return list;
}
