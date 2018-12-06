#include "j1App.h"
#include "GUIElement.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

GUIElement::GUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect area) : type(type), position(x, y), area(area)
{

}

GUIElement::~GUIElement()
{

}

void GUIElement::Draw(SDL_Texture* sprites, float speed)
{
	App->render->Blit(sprites, position.x, position.y, &area, speed);
}

void GUIElement::Update(float dt)
{
	Move();
}

void GUIElement::Move()
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
