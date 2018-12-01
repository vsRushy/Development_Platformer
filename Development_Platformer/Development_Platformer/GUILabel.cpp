#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUILabel.h"

GUILabel::GUILabel(int x, int y, p2SString text, uint size) : GUIElement(type, x, y, area)
{
	type = GUI_ELEMENT_TYPE::GUI_LABEL;
	this->text = text;
	this->size = size;

	// Create rect
	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.GetString(), width, height, App->fonts->default_font);
	area.w = width;
	area.h = height;
}

void GUILabel::Update(float dt)
{

}

void GUILabel::SetText(p2SString text)
{
	this->text = text;

	// Create new rect
	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.GetString(), width, height, App->fonts->default_font);
	area.w = width;
	area.h = height;
}