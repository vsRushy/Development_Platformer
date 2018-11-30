#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIImage.h"

GUIImage::GUIImage(int x, int y) : GUIElement(type, x, y)
{
	type = GUI_ELEMENT_TYPE::GUI_IMAGE;
}

void GUIImage::Update(float dt)
{

}