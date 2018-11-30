#ifndef __GUIElement_H__
#define __GUIElement_H__

#include "p2Point.h"
#include "j1GUIManager.h"

struct SDL_Texture;

class GUIElement
{
public:
	fPoint position;
	GUI_ELEMENT_TYPE type = GUI_ELEMENT_TYPE::UNKNOWN;

public:
	GUIElement(GUI_ELEMENT_TYPE type, int x, int y);
	virtual ~GUIElement();

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites, float speed = 0.0f);
};

#endif // __GUIElement_H__