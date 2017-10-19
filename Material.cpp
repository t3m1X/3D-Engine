#include "Material.h"
#include "glew\include\GL\glew.h"

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



