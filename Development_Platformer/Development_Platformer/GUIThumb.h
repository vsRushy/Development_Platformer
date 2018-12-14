#ifndef __GUIThumb_H__
#define __GUIThumb_H__

#include "GUIElement.h"

class GUIThumb : public GUIElement
{
public:
	GUIThumb(int x, int y, SDL_Rect image_area_normal, SDL_Rect image_area_hover, SDL_Rect image_area_pressed, GUIElement* son);
	~GUIThumb() {};

	void Update(float dt);

	void Click();

	SDL_Rect initial_area;

	SDL_Rect img_area_hover;
	SDL_Rect img_area_pressed;

	bool is_clicked = false;
};

#endif // __GUIThumb_H__