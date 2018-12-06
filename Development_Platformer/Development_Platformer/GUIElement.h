#ifndef __GUIElement_H__
#define __GUIElement_H__

#include "p2Point.h"
#include "j1GUIManager.h"

#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class GUIElement
{
public:
	fPoint position;
	SDL_Rect area;
	GUI_ELEMENT_TYPE type = GUI_ELEMENT_TYPE::UNKNOWN;

public:
	GUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect area);
	virtual ~GUIElement();

	virtual void Update(float dt);
	virtual void Draw(SDL_Texture* sprites, float speed = 0.0f);
	virtual void DrawLabel() {}

	virtual void Move();
};

#endif // __GUIElement_H__