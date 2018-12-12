#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIPanel.h"

GUIPanel::GUIPanel(int x, int y, SDL_Rect image_area, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_PANEL;
	area = image_area;
}

void GUIPanel::Update(float dt)
{
	Move();
}
