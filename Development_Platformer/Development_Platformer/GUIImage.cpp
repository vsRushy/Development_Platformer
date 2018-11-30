#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIImage.h"

GUIImage::GUIImage(int x, int y, SDL_Rect image_area) : GUIElement(type, x, y, area)
{
	type = GUI_ELEMENT_TYPE::GUI_IMAGE;
	area = image_area;
}

void GUIImage::Update(float dt)
{

}