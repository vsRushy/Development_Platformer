#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUILabel.h"
#include "j1Render.h"

#include "SDL_ttf/include/SDL_ttf.h"

GUILabel::GUILabel(int x, int y, p2SString text, SDL_Color color, _TTF_Font* font) : GUIElement(type, x, y, area)
{
	type = GUI_ELEMENT_TYPE::GUI_LABEL;

	this->text = text;
	this->color = color;
	this->font = font;

	tex = App->fonts->Print(this->text.GetString(), this->color, this->font);

	// Create rect
	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.GetString(), width, height, App->gui->default_font_used);
	area.w = width;
	area.h = height;
	area.x = 0;
	area.y = 0;
	
	position.x = x;
	position.y = y;
}

void GUILabel::Update(float dt)
{

}

void GUILabel::DrawLabel()
{
	App->render->Blit(tex, position.x, position.y, &area, 0.0f);
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