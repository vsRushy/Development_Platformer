#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1GUIManager.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "GUIElement.h"
#include "GUIImage.h"

j1GUIManager::j1GUIManager()
{
	name.create("gui");
}

// Destructor
j1GUIManager::~j1GUIManager()
{

}

bool j1GUIManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1GUIManager::Start()
{
	atlas = App->tex->Load("gui/atlas.png");

	return true;
}

// Called before render is available
bool j1GUIManager::Update(float dt)
{
	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr) gui_elements[i]->Update(dt);

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_IMAGE)
			gui_elements[i]->Draw(atlas);

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_BUTTON)
			gui_elements[i]->Draw(atlas);

	return true;
}

// Called before quitting
bool j1GUIManager::CleanUp()
{
	LOG("Freeing all gui elements");

	App->tex->UnLoad(atlas);

	for (uint i = 0; i < gui_elements.Count(); ++i)
	{
		if (gui_elements[i] != nullptr)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
		}
	}

	return true;
}

GUIElement* j1GUIManager::CreateGUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a)
{
	//static_assert(? , "code needs update");
	GUIElement* ret = nullptr;

	switch (type)
	{
	case GUI_ELEMENT_TYPE::GUI_IMAGE:
		ret = new GUIImage(x, y, a);
		break;
	/*case GUI_ELEMENT_TYPE::GUI_BUTTON:
		ret = new GUIButton(x, y);
		break;*/
	default:
		break;
	}

	if (ret != nullptr)
		gui_elements.PushBack(ret);

	return ret;
}

bool j1GUIManager::Load(pugi::xml_node& save)
{
	return true;
}

bool j1GUIManager::Save(pugi::xml_node& save) const
{
	return true;
}

void j1GUIManager::DeleteGUIElement(GUIElement* e)
{
	for (int i = 0; i < gui_elements.Count(); ++i)
	{
		if (gui_elements[i] == e)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
			break;
		}
	}
}

void j1GUIManager::DeleteAllGUIElements()
{
	for (uint i = 0; i < gui_elements.Count(); ++i)
	{
		if (gui_elements[i] != nullptr)
		{
			delete gui_elements[i];
			gui_elements[i] = nullptr;
		}
	}
}