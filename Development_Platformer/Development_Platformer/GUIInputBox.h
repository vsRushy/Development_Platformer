#ifndef __GUIInputBox_H__
#define __GUIInputBox_H__

#include "GUIElement.h"
#include "j1Fonts.h"
#include "p2SString.h"
#include "j1Textures.h"

#include "SDL/include/SDL_pixels.h"

class GUILabel;

struct _TTF_Font;
struct SDL_Texture;

class GUIInputBox : public GUIElement
{
public:
	GUIInputBox(int x, int y, SDL_Color color, _TTF_Font* font, SDL_Rect im_area, GUIElement* son);
	~GUIInputBox() 
	{
		if (input_box_label != nullptr)
		{
			App->gui->DeleteGUIElement((GUIElement*)input_box_label); // Needed to cast (?)
			input_box_label = nullptr;
		}
	}

	void Update(float dt);
	
public:
	SDL_Rect background;
	GUILabel* input_box_label = nullptr;
};

#endif // __GUIInputBox_H__