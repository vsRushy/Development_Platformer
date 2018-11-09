#ifndef __Enemy_level01_air__
#define __Enemy_level01_air__

#include "Enemy.h"

#include "p2Point.h"
#include "p2DynArray.h"

class Enemy_level01_air : public Enemy
{
private:
	iPoint original_pos;
	Animation anim;

	bool is_moving = false;
	bool player_is_range = false;
	
	iPoint objective;
	iPoint range[32];

	iPoint velocity;

	iPoint first_point;
	iPoint last_point;

public:
	Enemy_level01_air(int x, int y);

	void Move();
	void Update(float dt);

	void PathMovement(const p2DynArray<iPoint>*, iPoint, float);
};

#endif // __Enemy_level01_air__