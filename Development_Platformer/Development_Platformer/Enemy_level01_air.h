#ifndef __Enemy_level01_air__
#define __Enemy_level01_air__

#include "Enemy.h"

class Enemy_level01_air : public Enemy
{
private:
	int original_x = 0;
	int original_y = 0;
	Animation anim;

public:
	Enemy_level01_air(int x, int y);

	void Move();
};

#endif // __Enemy_level01_air__