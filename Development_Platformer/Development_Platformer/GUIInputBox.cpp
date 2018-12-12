#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIInputBox.h"
#include "GUILabel.h"
#include "j1Render.h"

#include "SDL_ttf/include/SDL_ttf.h"

#include "j1Input.h"

/* Offsets to adapt the position of the label to the position of the input box. Knowing we have only one sprite of InputBox, we define the
   offset right down here. Note that if we had various sprites of InputBox, we would need to assign the offset by creating two variables
   instead of defining them. */
#define OFFSET_X 6
#define OFFSET_Y 3

GUIInputBox::GUIInputBox(int x, int y, SDL_Color color, _TTF_Font* font, SDL_Rect im_area, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	input_box_label = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, x + OFFSET_X, y + OFFSET_Y, "", color, font);

	type = GUI_ELEMENT_TYPE::GUI_INPUTBOX;

	line = App->render->DrawQuad({0, 0, 2, 10}, 0, 0, 0, 255, false);
	
	input_box_label->SetText("TEST");

	input_box_label->color = color;
	input_box_label->font = font;

	position.x = x;
	position.y = y;

	area = im_area;
}

void GUIInputBox::Update(float dt)
{
	/*if (App->input->GetKey(SDL_SCANCODE_U) == KEY_UP)
	{
		input_box_label->SetText("Changed");
	}*/

	int x, y;
	App->input->GetMousePosition(x, y);
	bool is_inside = x > position.x && x < position.x + area.w && y > position.y && y < position.y + area.h;

	// We want SDL Text Input only when we are inside the rect of the input box.
	if (is_inside)
	{
		SDL_StartTextInput();
		SDL_SetTextInputRect(&input_box_label->area);
		p2SString character = App->input->text_input;
		input_box_label->AddChar(character);
	}
	else
	{
		SDL_StopTextInput();
	}

	Move();
}

void GUIInputBox::DrawInputBox()
{
	line = App->render->DrawQuad({ 0, 0, 2, 10 }, 0, 0, 0, 255, false);
}

void GUIInputBox::GetInput()
{

}