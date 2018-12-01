#ifndef __GUILabel_H__
#define __GUILabel_H__

#include "GUIElement.h"
#include "j1Fonts.h"
#include "p2SString.h"

class GUILabel : public GUIElement
{
public:
	GUILabel(int x, int y, p2SString text, uint size);
	~GUILabel() {};

	void Update(float dt);

	void SetText(p2SString text);

public:
	p2SString text;

private:
	j1Fonts font;
	int size = 0;
};

#endif // __GUILabel_H__