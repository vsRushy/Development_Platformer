#ifndef __GUISlider_H__
#define __GUISlider_H__

#include "GUIElement.h"

class GUIThumb;

class GUISlider : public GUIElement
{
public:
	GUISlider(int x, int y, SDL_Rect image_area, SDL_Rect thumb_1, SDL_Rect thumb_2, SDL_Rect thumb3, GUIElement* son);
	~GUISlider() 
	{
		if (slider_thumb != nullptr)
		{
			App->gui->DeleteGUIElement((GUIElement*)slider_thumb);
			slider_thumb = nullptr;
		}
	}

	void Update(float dt);
	float GetSliderValue() const;

	GUIThumb* slider_thumb = nullptr;
	bool is_inside_thumb = false;

private:
	float value = 0;
};

#endif // __GUISlider_H__