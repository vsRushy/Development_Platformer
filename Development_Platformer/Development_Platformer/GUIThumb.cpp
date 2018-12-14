#include "j1App.h"
#include "j1Input.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIThumb.h"
#include "p2Log.h"

GUIThumb::GUIThumb(int x, int y, SDL_Rect image_area, SDL_Rect image_area_hover, SDL_Rect image_area_pressed, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_THUMB;
	initial_area = image_area;
	area = initial_area;

	img_area_hover = image_area_hover;
	img_area_pressed = image_area_pressed;
}

void GUIThumb::Update(float dt)
{
	Click();
	Move();
}

void GUIThumb::Click()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	bool is_inside = x > position.x && x < position.x + initial_area.w && y > position.y && y < position.y + initial_area.h;

	if (is_inside)
	{
		if (App->input->GetMouseButtonDown(1) == SDL_PRESSED)
		{
			is_clicked = true;
			area = img_area_pressed;
		}
		else if (App->input->GetMouseButtonDown(1) == SDL_RELEASED)
		{
			is_clicked = false;
			area = img_area_hover;
		}
	}
	else
	{
		is_clicked = false;
		area = initial_area;
	}
}
