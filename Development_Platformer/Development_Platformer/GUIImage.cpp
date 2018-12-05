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
		float dist_x_posx = x + position.x;
		float dist_y_posy = y + position.y;
		LOG("%f", dist_x_posx);

		if (App->input->GetMouseButtonDown(3))
		{
			/*position.x = x - 3;
			position.y = y - 3;*/
			
			/*float dist_x_posx = x + position.x;
			float dist_y_posy = x + position.y;*/
			
			position.x = x - dist_x_posx;
			position.y = y - dist_y_posy;
		}
	}
}