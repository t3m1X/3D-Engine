#include "Material.h"
#include "glew\include\GL\glew.h"

Material::Material(GameObject* own) : Component(own)
{

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



