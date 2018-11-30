#ifndef __GUIImage_H__
#define __GUIImage_H__

#include "GUIElement.h"

#include "p2Point.h"
#include "p2DynArray.h"

class GUIImage : public GUIElement
{
public:
	GUIImage(int x, int y, SDL_Rect image_area);
	~GUIImage() {};

	void Update(float dt);
};

#endif // __GUIImage_H__