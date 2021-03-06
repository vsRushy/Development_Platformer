#include "j1App.h"
#include "j1Input.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIButton.h"
#include "j1Audio.h"
#include "p2Log.h"

GUIButton::GUIButton(int x, int y, SDL_Rect image_area, SDL_Rect image_area_hover, SDL_Rect image_area_pressed, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_BUTTON;
	initial_area = image_area;
	area = initial_area;

	img_area_hover = image_area_hover;
	img_area_pressed = image_area_pressed;
}

void GUIButton::Update(float dt)
{
	Click();
	Move();
}

void GUIButton::Click()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	bool is_inside = x > position.x && x < position.x + initial_area.w && y > position.y && y < position.y + initial_area.h;

	if (is_inside)
	{
		if (App->input->GetMouseButtonDown(1) == SDL_PRESSED && can_change)
		{
			App->audio->PlayFx(7);
			can_change = false;
			has_been_clicked = !has_been_clicked;
		}
		else if(App->input->GetMouseButtonDown(1) == SDL_RELEASED)
		{
			can_change = true;
		}

		if (has_been_clicked)
		{
			area = img_area_pressed;
		}
		if (can_change)
		{
			area = img_area_hover;
		}
	}
	else
	{
		area = initial_area;
	}
}
