#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleFilesystem.h"

ModuleImporter::ModuleImporter(bool start_enabled)
{
	SetName("Impoter");
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	bool ret = true;
	CheckAssets();
	return ret;
}

void ModuleImporter::CheckAssets()
{
	std::vector<string> assets = App->fs->ListFiles(App->fs->GetAssetsPath().c_str());
	std::vector<string> fbx_assets;
	std::vector<string> loaded_scenes = App->fs->ListFiles(App->fs->GetLibraryPath().c_str(), "rse");
	std::vector<string> loaded_textures = App->fs->ListFiles(App->fs->GetTextureLibraryPath().c_str());

	for (int i = 0; i < assets.size(); ++i)
	{
		if (assets[i].find("fbx") == assets[i].npos)
		{
			fbx_assets.push_back(assets[i]);
		}
	}
}
