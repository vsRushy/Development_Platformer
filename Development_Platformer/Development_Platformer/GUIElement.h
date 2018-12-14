#ifndef __GUIElement_H__
#define __GUIElement_H__

#include "p2Point.h"
#include "j1GUIManager.h"

#include "SDL/include/SDL_rect.h"

#define DEBUG_DRAW_OFFSET 2

struct SDL_Texture;
class GUIElement;

class GUIElement
{
public:
	fPoint position;
	SDL_Rect area;
	GUI_ELEMENT_TYPE type = GUI_ELEMENT_TYPE::UNKNOWN;

public:
	GUIElement(GUI_ELEMENT_TYPE type, int x, int y, SDL_Rect area, GUIElement* son);
	virtual ~GUIElement();

	virtual void DrawDebug();

	virtual void Update(float dt);
	virtual void Draw(SDL_Texture* sprites, float speed = 0.0f);
	virtual void DrawLabel() {}
	virtual void DrawInputBox() {}

	virtual void Move();

private:
	GUIElement* son = nullptr;
};

#endif // __GUIElement_H__