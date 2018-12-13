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
#include "GUIButton.h"
#include "GUILabel.h"
#include "GUIPanel.h"
#include "GUIInputBox.h"

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
	image_textures = App->tex->Load("gui/Image_Textures.png");
	button_textures = App->tex->Load("gui/Start_Quit_Buttons_01.png");
	inputbox_textures = App->tex->Load("gui/textbox.png");

	default_font_used = App->fonts->Load("fonts/open_sans/OpenSans-Regular.ttf");

	return true;
}

// Called before render is available
bool j1GUIManager::Update(float dt)
{
	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr) gui_elements[i]->Update(dt);

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_IMAGE)
			gui_elements[i]->Draw(image_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_PANEL)
			gui_elements[i]->Draw(image_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_BUTTON)
			gui_elements[i]->Draw(button_textures);

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_INPUTBOX)
		{
			gui_elements[i]->Draw(inputbox_textures);
			gui_elements[i]->DrawInputBox();
		}

	for (uint i = 0; i < gui_elements.Count(); ++i)
		if (gui_elements[i] != nullptr && gui_elements[i]->type == GUI_ELEMENT_TYPE::GUI_LABEL)
			gui_elements[i]->DrawLabel();

	return true;
}

// Called before quitting
bool j1GUIManager::CleanUp()
{
	LOG("Freeing all gui elements");

	App->tex->UnLoad(atlas);
	App->tex->UnLoad(image_textures);
	App->tex->UnLoad(button_textures);
	App->tex->UnLoad(inputbox_textures);

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

GUIElement* j1GUIManager::CreateGUIImage(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIImage(x, y, a, son);

	if (ret != nullptr)
		gui_elements.PushBack(ret);

	return ret;
}

GUIElement* j1GUIManager::CreateGUIButton(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, SDL_Rect a_1, SDL_Rect a_2, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIButton(x, y, a, a_1, a_2, son);

	if (ret != nullptr)
		gui_elements.PushBack(ret);

	return ret;
}

GUIElement* j1GUIManager::CreateGUILabel(GUI_ELEMENT_TYPE type, int x, int y, p2SString text, SDL_Color color, _TTF_Font* font, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUILabel(x, y, text, color, font, son);


	if (ret != nullptr)
		gui_elements.PushBack(ret);

	return ret;
}

GUIElement* j1GUIManager::CreateGUIPanel(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect a, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIPanel(x, y, a, son);

	if (ret != nullptr)
		gui_elements.PushBack(ret);

	return ret;
}

GUIElement* j1GUIManager::CreateGUIInputBox(GUI_ELEMENT_TYPE type, int x, int y, SDL_Color color, _TTF_Font* font, SDL_Rect a, GUIElement* son)
{
	GUIElement* ret = nullptr;

	ret = new GUIInputBox(x, y, color, font, a, son);

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