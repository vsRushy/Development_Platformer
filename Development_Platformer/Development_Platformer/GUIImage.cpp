#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUIImage.h"
#include "j1Input.h"
#include "p2Log.h"

GUIImage::GUIImage(int x, int y, SDL_Rect image_area) : GUIElement(type, x, y, area)
{
	type = GUI_ELEMENT_TYPE::GUI_IMAGE;
	area = image_area;
}

void GUIImage::Update(float dt)
{
	Move();
}

void GUIImage::Move()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	bool is_inside = x > position.x && x < position.x + area.w && y > position.y && y < position.y + area.h;

	if (is_inside)
	{
		if (App->input->GetMouseButtonDown(3))
		{
			position.x = x - area.w / 2;
			position.y = y - area.h / 2;
		}
	}
}