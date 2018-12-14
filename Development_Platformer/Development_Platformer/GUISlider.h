#ifndef __GUISlider_H__
#define __GUISlider_H__

#include "GUIElement.h"

class GUISlider : public GUIElement
{
public:
	GUISlider(int x, int y, SDL_Rect image_area, GUIElement* son);
	~GUISlider() {};

	void Update(float dt);
};

#endif // __GUISlider_H__