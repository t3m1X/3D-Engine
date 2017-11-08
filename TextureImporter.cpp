#include "TextureImporter.h"
#include "ModuleFilesystem.h"
#include "ModuleRenderer3D.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"


#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

TextureImporter::TextureImporter()
{
}

TextureImporter::~TextureImporter()
{
}

bool TextureImporter::Import(const char * path, std::string & output_file)
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = App->fs->Load(path, &buffer);

	if (buffer != NULL) {
		ret = Import(buffer, size, output_file, App->fs->GetFileFromPath(path).c_str());
	}

	if (buffer != nullptr) {
		delete[] buffer;
	}

	return ret;
}

bool TextureImporter::Import(const void * buffer, uint size, std::string & output_file, const char * file_name = DEFAULT_TEXTURE_NAME)
{
	bool ret = false;

	if (buffer != NULL) {
		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size)) { 
			ilEnable(IL_FILE_OVERWRITE); 

			ILuint ilsize;
			ILubyte* data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); 
			ilsize = ilSaveL(IL_DDS, NULL, 0); 
			if (ilsize > 0) {
				data = new ILubyte[ilsize];
				if (ilSaveL(IL_DDS, data, ilsize) > 0) {
					ret = App->fs->SaveUnique(App->fs->texture_library_path.c_str(), (char*)data, file_name, "dds", ilsize, output_file, (file_name == DEFAULT_TEXTURE_NAME));
				}

				if (data != nullptr) {
					delete[] data;
				}

			}
			ilDeleteImages(1, &imageID);
		}
	}

	if (ret == false)
		LOG("ERROR importing texture from buffer");

	return ret;
}

bool TextureImporter::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = false;

	return ret;
}

bool TextureImporter::Load(const char * exported_file)
{
	bool ret = false;

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
			LOG("Texture Image conversion Failed: %d %s", error, iluErrorString(error));
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
			new_tex->SetPath(exported_file);
			new_tex->SetName(App->fs->GetFileFromPath(exported_file).c_str());

			//Clamping Method
			GLint clampParam;
			switch (clamp_type) {
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

			App->tex->textures.push_back(new_tex);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else {
		error = ilGetError();
		LOG("Image Load Error %d %s", error, iluErrorString(error));
	}

	//glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &imageID);

	LOG("Texture Load End:\n\t%s", exported_file);

	return ret;
}

bool TextureImporter::Load(const char * exported_file, Texture * tex)
{
		bool ret = false;

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
				LOG("Texture Image conversion Failed: %d %s", error, iluErrorString(error));
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
				new_tex->SetPath(exported_file);
				new_tex->SetName(App->fs->GetFileFromPath(exported_file).c_str());

				//Clamping Method
				GLint clampParam;
				switch (clamp_type) {
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

				tex = new_tex;
				App->tex->textures.push_back(new_tex);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else {
			error = ilGetError();
			LOG("Image Load Error %d %s", error, iluErrorString(error));
		}

		//glBindTexture(GL_TEXTURE_2D, 0);

		ilDeleteImages(1, &imageID);

		LOG("Texture Load End:\n\t%s", exported_file);

		return ret;
	};


