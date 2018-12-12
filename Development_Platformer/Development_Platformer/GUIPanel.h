#ifndef __GUIPanel_H__
#define __GUIPanel_H__

#include "GUIElement.h"

class GUIPanel : public GUIElement
{
public:
	GUIPanel(int x, int y, SDL_Rect image_area, GUIElement* son);
	~GUIPanel() {};

	void Update(float dt);
};

#endif // __GUIPanel_H__