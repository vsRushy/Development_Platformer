#ifndef __j1GUIManager_H__
#define __j1GUIManager_H__

#include "j1Module.h"
#include "p2DynArray.h"

#include "SDL/include/SDL.h"

class GUIElement;

enum class GUI_ELEMENT_TYPE
{
	UNKNOWN,
	GUI_IMAGE,
	GUI_BUTTON,
	GUI_LABEL
};

class j1GUIManager : public j1Module
{
public:

	j1GUIManager();
	virtual ~j1GUIManager();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public:
	GUIElement* CreateGUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1 = { NULL }, SDL_Rect a_2 = { NULL });
	void DeleteGUIElement(GUIElement* e);
	void DeleteAllGUIElements();

private:
	p2DynArray<GUIElement*> gui_elements = NULL;

	SDL_Texture* atlas;
};

#endif // __j1GUIManager_H__