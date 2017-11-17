#include "ModuleTextures.h"
#include "Application.h"
#include "Console.h"
#include "ModuleFileSystem.h"
#include "ModuleJson.h"

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

void Texture::SetTextureType(TEXTURE_TYPE t)
{
	type = t;
}



TEXTURE_TYPE Texture::GetType()
{
	return type;
}



void Texture::SetFormat(const int & f)
{
	format = f;
}

void Texture::SetName(const char * n)
{
	name = n;
}

std::string Texture::GetPath() const
{
	return path;
}

int Texture::GetFormat() const
{
	return format;
}

const char * Texture::GetName() const
{
	return name;
}

void Texture::Serialize(JSON_File * doc)
{
	doc->SetNumber("type", this->type);
	doc->SetString("path", this->path.c_str());
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
	clamp = clampingTexType_ClampRepeat;
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	LOG_OUT("Devil init");
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
	if (!textures.empty()) {
		for (list<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			delete *it;
		}
		LOG_OUT("Deleted all textures");
	}
	return true;
}

Texture * ModuleTextures::GetTexture()
{
	
	return textures.front();
}

void ModuleTextures::Clear()
{
	for (list<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		delete *it;
	}

	
}

bool ModuleTextures::Empty()
{
	return textures.empty();
}

Texture* ModuleTextures::LoadTexture(const char* path)
{

	ILuint imageID;				
	GLuint textureID;		
	ILboolean success;			
	ILenum error;		
/*	const char* drop_filedir = path;
	const char* filext = strrchr(drop_filedir, '.');
	for (char* p = filext + 1; *p != '\0'; *p = toupper(*p), p++);*/

	ilGenImages(1, &imageID); 		
	ilBindImage(imageID); 			
	success = ilLoadImage(path); 	
	LOG_OUT("Texture load: Loading %s", path);
											
	if (success)
	{
		LOG_OUT("Image loaded succesfully");
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
			LOG_OUT("Texture load: Image conversion failed: %s\n", ilGetError());
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,0,ilGetInteger(IL_IMAGE_FORMAT),ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),0,ilGetInteger(IL_IMAGE_FORMAT),GL_UNSIGNED_BYTE,	ilGetData());
		LOG_OUT("Texture load: Texture generated");
		tex = new Texture();
		tex->SetWidth(ImageInfo.Width);
		tex->Setheight(ImageInfo.Height);
		tex->SetId(textureID);
		tex->SetTextureType(DIFFUSE);/// for now we just have diffuse
		textures.push_back(tex);
		glBindTexture(GL_TEXTURE_2D, 0);
		return tex;
	}
	else 
	LOG_OUT("Texture load: Error loading the file %s", path);
	return nullptr; 
}

bool ModuleTextures::ImportTexture(const char * path, std::string & output_file)
{
	bool ret = false;

	ret = Import(path, output_file);

	if (ret == false)
		LOG_OUT("ERROR importing texture %s", path);

	return ret;
}

Texture * ModuleTextures::LoadDDSTexture(const char * path)
{
	Texture* tex = nullptr;

	tex = Load(path, tex);

	return tex;
}

Texture * ModuleTextures::LoadToDDS(const char * path, std::string & output_file)
{
	Texture* tex = nullptr;

	if (ImportTexture(path, output_file)) {

		tex = LoadDDSTexture(output_file.c_str());

	}
	else {
		LOG_OUT("ERROR Importing texture %s", GetFile(path).c_str());
	}

	return tex;
}

Texture * ModuleTextures::LoadToDDS(const char * path)
{
	Texture* tex = nullptr;

	std::string output_file;

	if (ImportTexture(path, output_file)) {
		tex = LoadDDSTexture(output_file.c_str());
	}
	else {
		LOG_OUT("ERROR Importing texture %s", GetFile(path).c_str());
	}

	return tex;
}

bool ModuleTextures::Import(const char * path, std::string & output_file)
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = App->fs->Load(path, &buffer);

	if (buffer != NULL) {
		ret = Import(buffer, size, output_file, GetFile(path).c_str());
	}

	
	return ret;
}

bool ModuleTextures::Import(const void * buffer, uint size, std::string & output_file, const char * file_name)
{
	bool ret = false;

	if (buffer != NULL) {
		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size)) { // loads the buffer
			ilEnable(IL_FILE_OVERWRITE); // Enables File Overwriting (self-explanatory :s)

			ILuint ilsize;
			ILubyte* data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // Compression mode [DXT]
			ilsize = ilSaveL(IL_DDS, NULL, 0); // Just setting new buffer size
			if (ilsize > 0) {
				data = new ILubyte[ilsize];
				if (ilSaveL(IL_DDS, data, ilsize) > 0) {
					ret = App->fs->SaveUnique(LIBRARY_TEXTURES, (char*)data, file_name, "dds", ilsize, output_file, (file_name == "default"));
				}

				if (data != nullptr) {
					delete[] data;
				}

			}
			ilDeleteImages(1, &imageID);
		}
	}

	if (ret == false)
		LOG_OUT("ERROR importing texture from buffer");

	return ret;
}

Texture* ModuleTextures::Load(const char * exported_file)
{
	uint textureID = 0;
	ILuint imageID;

	Texture* new_tex = nullptr;

	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(exported_file)) {
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		//Flip the image if it is upside-down
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
			error = ilGetError();
			LOG_OUT("Texture Image conversion Failed: %d %s", error, iluErrorString(error));
		}
		else {

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			new_tex = new Texture();

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			new_tex->SetId(textureID);
			new_tex->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			new_tex->Setheight(ilGetInteger(IL_IMAGE_HEIGHT));
			new_tex->SetFormat(ilGetInteger(IL_IMAGE_FORMAT));
			new_tex->path = exported_file;
			new_tex->SetName(GetFile(exported_file).c_str());

			//Clamping Method
			GLint clampParam;
			switch (clamp) {
			case clampingTexType_ClampToEdge:
				clampParam = GL_CLAMP_TO_EDGE;
				break;
			case clampingTexType_ClampRepeat:
				clampParam = GL_REPEAT;
				break;
			case clampingTexType_ClampMirroredRepeat:
				clampParam = GL_MIRRORED_REPEAT;
				break;
			default:
				clampParam = GL_REPEAT;
				break;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampParam);
			// Interpolation Method
			GLint interParam;
			switch (interpolation_type) {
			case interpolationTexType_Nearest:
				interParam = GL_NEAREST;
				break;
			case interpolationTexType_Linear:
				interParam = GL_LINEAR;
				break;
			default:
				interParam = GL_LINEAR;
				break;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interParam);

			//Texture Specifications
			glTexImage2D(GL_TEXTURE_2D, 0, new_tex->GetFormat(), new_tex->Getwidth(), new_tex->Getheight(), 0, new_tex->GetFormat(), GL_UNSIGNED_BYTE, ilGetData());

			textures.push_back(new_tex);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else {
		error = ilGetError();
		LOG_OUT("Image Load Error %d %s", error, iluErrorString(error));
	}

	//glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &imageID);

	LOG_OUT("Texture Load End:\n\t%s", exported_file);

	return new_tex;
}

Texture * ModuleTextures::Load(const char * exported_file, Texture * tex)
{
	char* buffer = nullptr;
	std::string file = exported_file;
	uint size = App->fs->Load(file.c_str(), &buffer);

	Texture* new_tex = nullptr;

	if (buffer != nullptr) {
		if (size > 0) {
			uint textureID = 0;
			ILuint imageID;
			ILenum error;

			ilGenImages(1, &imageID);
			ilBindImage(imageID);

			if (ilLoadL(IL_DDS, buffer, size)) {
				ILinfo ImageInfo;
				iluGetImageInfo(&ImageInfo);
				//Flip the image if it is upside-down
				if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
					iluFlipImage();
				}
				if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
					error = ilGetError();
					LOG_OUT("Texture Image conversion Failed: %d %s", error, iluErrorString(error));
				}
				else {

					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

					new_tex = new Texture();

					glGenTextures(1, &textureID);
					glBindTexture(GL_TEXTURE_2D, textureID);

					new_tex->SetId(textureID);
					new_tex->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
					new_tex->Setheight(ilGetInteger(IL_IMAGE_HEIGHT));
					new_tex->SetFormat(ilGetInteger(IL_IMAGE_FORMAT));
					new_tex->path = exported_file;
					new_tex->SetName(GetFile(exported_file).c_str());

					//Clamping Method
					GLint clampParam;
					switch (clamp) {
					case clampingTexType_ClampToEdge:
						clampParam = GL_CLAMP_TO_EDGE;
						break;
					case clampingTexType_ClampRepeat:
						clampParam = GL_REPEAT;
						break;
					case clampingTexType_ClampMirroredRepeat:
						clampParam = GL_MIRRORED_REPEAT;
						break;
					default:
						clampParam = GL_REPEAT;
						break;
					}
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampParam);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampParam);
					// Interpolation Method
					GLint interParam;
					switch (interpolation_type) {
					case interpolationTexType_Nearest:
						interParam = GL_NEAREST;
						break;
					case interpolationTexType_Linear:
						interParam = GL_LINEAR;
						break;
					default:
						interParam = GL_LINEAR;
						break;
					}
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interParam);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interParam);

					//Texture Specifications
					glTexImage2D(GL_TEXTURE_2D, 0, new_tex->GetFormat(), new_tex->Getwidth(), new_tex->Getheight(), 0, new_tex->GetFormat(), GL_UNSIGNED_BYTE, ilGetData());

					textures.push_back(new_tex);

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			else {
				error = ilGetError();
				LOG_OUT("Image Load Error %d %s", error, iluErrorString(error));
			}
			ilDeleteImages(1, &imageID);
		}
	}

	LOG_OUT("Texture Load End:\n\t%s", exported_file);

	return new_tex;
}
