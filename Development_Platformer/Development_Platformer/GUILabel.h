#ifndef __GUILabel_H__
#define __GUILabel_H__

#include "GUIElement.h"

class GUILabel : public GUIElement
{
public:
	GUILabel(int x, int y, SDL_Rect image_area);
	~GUILabel() {};

	void Update(float dt);
};

#endif // __GUILabel_H__