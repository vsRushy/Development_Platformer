#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUISlider.h"
#include "j1Input.h"

GUISlider::GUISlider(int x, int y, SDL_Rect image_area, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_IMAGE;
	area = image_area;
}

void GUISlider::Update(float dt)
{
	Move();
}
