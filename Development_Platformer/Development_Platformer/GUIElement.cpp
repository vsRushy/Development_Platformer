#include "j1App.h"
#include "GUIElement.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

GUIElement::GUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect area, GUIElement* parent) : type(type), position(x, y), area(area), parent(parent)
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
		if (parent == nullptr)
		{
			if (App->input->GetMouseButtonDown(3))
			{
				position.x = x - area.w / 2;
				position.y = y - area.h / 2;
			}
		}
		else if (parent != nullptr)
		{
			if (App->input->GetMouseButtonDown(3))
			{
				position.x = x - area.w / 2;
				position.y = y - area.h / 2;
				parent->position.x = x - parent->area.w / 2;
				parent->position.y = y - parent->area.h / 2;
			}
		}
	}
}
