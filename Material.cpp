#include "Material.h"
#include "glew\include\GL\glew.h"
#include "ModuleImGui.h"

Material::Material(GameObject* own) : Component(own)
{
	this->SetType(MATERIAL);
}

Material::~Material()
{
	for (int i = 0; i < textures.size(); i++) {
		delete textures[i];
	}
}

void Material::AddTexture(Texture * tex)
{
	textures.push_back(tex);
}

Texture * Material::FindtexturebyType(TEXTURE_TYPE type)
{
	for (uint i = 0; i < textures.size(); i++) {
		if (textures[i]->GetType() == type) {
			return textures[i];
		}
	}
}

void Material::UI_draw()
{
	ImGui::Separator();
	if (!textures.empty()) {
		for (uint i = 0; i < textures.size(); i++) {
			if (textures[i]->GetType() == DIFFUSE) {
				ImGui::Text("Diffuse: ");
				ImGui::Separator();
			}
			//////////
			//here will check other types of textures
			///////////
			ImGui::Text("Width: %d", textures[i]->Getwidth());
			ImGui::Text("Height: %d", textures[i]->Getheight());
			ImGui::Text("Id texture: %d", textures[i]->Getid());
			ImGui::Text("Preview:");
			ImTextureID img = (void*)textures[i]->Getid();

			ImGui::Image(img, ImVec2(100, 100));
		}
	}
	else
		ImGui::Text("No textures loaded");
}



