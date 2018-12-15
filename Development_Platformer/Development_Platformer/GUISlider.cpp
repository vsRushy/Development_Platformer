#include "j1App.h"
#include "j1GUIManager.h"
#include "GUIElement.h"
#include "GUISlider.h"
#include "GUIThumb.h"
#include "j1Input.h"

// considering all thumbs and silder have the same anchure, we define an offset to positionate correctly the thumb
#define THUMB_OFFSET 1

GUISlider::GUISlider(int x, int y, SDL_Rect image_area, SDL_Rect thumb_1, SDL_Rect thumb_2, SDL_Rect thumb_3, GUIElement* son) : GUIElement(type, x, y, area, son)
{
	type = GUI_ELEMENT_TYPE::GUI_SLIDER;
	area = image_area;

	position.x = x;
	position.y = y;

	son = slider_thumb = (GUIThumb*)App->gui->CreateGUIThumb(GUI_ELEMENT_TYPE::GUI_THUMB, x + THUMB_OFFSET, y - thumb_1.h / 2, thumb_1, thumb_2, thumb_3);
}

void GUISlider::Update(float dt)
{
	Move();

	int x, y;
	App->input->GetMousePosition(x, y);

	if (slider_thumb->is_clicked)
	{
		// Boundaries
		if (slider_thumb->position.y >= this->position.y - slider_thumb->area.h / 2 && 
			slider_thumb->position.y <= this->position.y + this->area.h - slider_thumb->area.h / 2)
		{
			slider_thumb->position.y = y - slider_thumb->area.h / 2;

			// Reset pos if in boundaries
			if (slider_thumb->position.y < this->position.y - slider_thumb->area.h / 2)
			{
				slider_thumb->position.y = this->position.y - slider_thumb->area.h / 2;
			}
			if (slider_thumb->position.y > this->position.y + this->area.h - slider_thumb->area.h / 2)
			{
				slider_thumb->position.y = this->position.y + this->area.h - slider_thumb->area.h / 2;
			}
		}
	}
}

float GUISlider::GetSliderValue() const
{
	return (slider_thumb->position.y - this->position.y + slider_thumb->area.h / 2) / (float)this->area.h;
}