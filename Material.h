#ifndef __MATERIAL__H__
#define __MATERIAL__H__
#include "Globals.h"
#include <vector>
#include "ModuleTextures.h"
#include "Component.h"



class Material : public Component {

	public:

		Material(GameObject* own);
		virtual ~Material();
		void AddTexture(Texture* tex);
		Texture* FindtexturebyType(TEXTURE_TYPE type);


	private: 
		//// it will only have one texture for now
		std::vector<Texture*> textures;


};



#endif
