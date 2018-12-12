#include "j1App.h"
#include <conio.h>
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIInputBox.h"
#include "GUILabel.h"
#include "j1Render.h"

#include "SDL_ttf/include/SDL_ttf.h"

GUIInputBox::GUIInputBox(int x, int y, p2SString text, SDL_Color color, _TTF_Font* font, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	input_box_label = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, x, y, text, color, font);

	type = GUI_ELEMENT_TYPE::GUI_INPUTBOX;

	line = App->render->DrawQuad({0, 0, 2, 10}, 0, 0, 0, 255, false);
	
	input_box_label->text = text;

	input_box_label->color = color;
	input_box_label->font = font;

	// Create rect
	int width = 0, height = 0;
	//App->fonts->CalcSize(this->text, width, height, App->gui->default_font_used);
	area.w = width;
	area.h = height;
	area.x = 0;
	area.y = 0;

	position.x = x;
	position.y = y;
}

void GUIInputBox::Update(float dt)
{
	Move();
}

void GUIInputBox::DrawInputBox()
{
	line = App->render->DrawQuad({ 0, 0, 2, 10 }, 0, 0, 0, 255, false);
}



void GUIInputBox::GetInput()
{

}