#include "ModuleTextures.h"
#include "Application.h"

#include "glew\include\GL\glew.h"

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

bool ModuleTextures::Init(JSON_File* conf)
{
	bool ret = true;
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	LOG("Devil init");
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

uint ModuleTextures::LoadTexture(const char* path)
{

	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(path); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			LOG("Image conversion failed: %s\n", ilGetError());
		}

		App->renderer3D->LoadTexBuffer(&textureID, 1, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_FILTER);
	}
	else // If we failed to open the image file in the first place...
	{
		LOG("Error loading the file : %s\n", ilGetError());
		
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	return textureID; 
}
