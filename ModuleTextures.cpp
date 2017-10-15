#include "ModuleTextures.h"
#include "Application.h"
#include "Console.h"

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

void Texture::SetId(uint id)
{
	
	this->id = id;
}

void Texture::SetWidth(int w)
{
	width = w;
}

void Texture::Setheight(int h)
{
	height = h;
}

ModuleTextures::ModuleTextures(bool start_enabled) : Module(start_enabled)
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Init(JSON_File* conf)
{

	SetName("Textures");
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

Texture * ModuleTextures::GetTexture()
{
	
	return textures.front();
}

bool ModuleTextures::Empty()
{
	return textures.empty();
}

uint ModuleTextures::LoadTexture(const char* path)
{

	ILuint imageID;				
	GLuint textureID;		
	ILboolean success;			
	ILenum error;				
	ilGenImages(1, &imageID); 		
	ilBindImage(imageID); 			
	success = ilLoadImage(path); 	
	LOG("Texture load: Loading %s", path);
											
	if (success)
	{

		Texture* tex;
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		
		if (!success)
		{
			LOG("Texture load: Image conversion failed: %s\n", ilGetError());
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,0,ilGetInteger(IL_IMAGE_FORMAT),ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),0,ilGetInteger(IL_IMAGE_FORMAT),GL_UNSIGNED_BYTE,	ilGetData());
		LOG("Texture load: Texture generated");
		tex = new Texture();
		tex->SetWidth(ImageInfo.Width);
		tex->Setheight(ImageInfo.Height);
		tex->SetId(textureID);
		textures.push_back(tex);
		
	}
	else 
	{
		LOG("Texture load: Error loading the file %s", path);
		
	}

	/*Texture* tex = new Texture();
	tex->SetId((uint)textureID);
	ilDeleteImages(1, &imageID);
	textures.push_back(tex);*/
	return textureID; 
}
