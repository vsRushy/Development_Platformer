#ifndef __GUIImage_H__
#define __GUIImage_H__

#include "GUIElement.h"

class GUIImage : public GUIElement
{
public:
	GUIImage(int x, int y, SDL_Rect image_area, GUIElement* son);
	~GUIImage() {};

	void Update(float dt);

	bool has_been_clicked = false;
	bool can_change = true;
};

#endif // __GUIImage_H__