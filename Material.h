#ifndef __MATERIAL__H__
#define __MATERIAL__H__
#include "Globals.h"
#include <vector>
#include "ModuleTextures.h"
#include "Component.h"



class Material : public Component {

	public:

		Material(GameObject* own);
		Material();
		virtual ~Material();
		void AddTexture(Texture* tex);
		Texture* FindtexturebyType(TEXTURE_TYPE type);
		void UI_draw();
		bool HasTextures()const;
		void CleanUp();
		void Serialize(JSON_File* doc);


	private: 
		//// it will only have one texture for now
		std::vector<Texture*> textures;


};



#endif
