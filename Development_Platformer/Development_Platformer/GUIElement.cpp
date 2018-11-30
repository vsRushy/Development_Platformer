#include "j1App.h"
#include "GUIElement.h"
#include "j1Render.h"
#include "j1Textures.h"

GUIElement::GUIElement(GUI_ELEMENT_TYPE type, int x, int y) : type(type), position(x, y)
{

}

GUIElement::~GUIElement()
{

}

void GUIElement::Draw(SDL_Texture* sprites, float speed)
{
	App->render->Blit(sprites, position.x, position.y, NULL, speed);
}
