#include "ModuleTextures.h"
#include "Application.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

Texture::Texture()
{
}

Texture::~Texture()
{
}

int Texture::Getid()
{
	return id;
}

int Texture::Getwidth()
{
	return width;
}

int Texture::Getheight()
{
	return height;
}

ModuleTextures::ModuleTextures(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Init()
{
	bool ret = true;
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleTextures::Start()
{
	return true;
}

update_status ModuleTextures::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{
	bool ret = true;
	for (list<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		delete *it;
	}

	textures.clear();

	return ret;
}

bool ModuleTextures::LoadTexture(const char* path)
{
	bool ret;

	uint* id = nullptr;

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilGenImages(1, &imageID); 		
	ilBindImage(imageID); 				
	if (path != nullptr) {
		if (ilLoadImage(path))
		{
			App->renderer3D->LoadTexBuffer(id, 1, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_FILTER);

			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);

			ret = ilutGLBindTexImage();
			ilDeleteImages(1, &imageID);

		}
		else
		{
			LOG("Error loading the file : %s\n", ilGetError());
		}
	}

	return ret; 
}
