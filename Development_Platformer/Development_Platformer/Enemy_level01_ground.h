#ifndef __Enemy_level01_ground__
#define __Enemy_level01_ground__

#include "Enemy.h"

#include "p2Point.h"
#include "p2DynArray.h"

#define RANGE_SIZE 80

class Enemy_level01_ground : public Enemy
{
private:
	iPoint original_pos;
	Animation anim;

	bool is_moving = true;
	bool player_is_range = false;

	iPoint objective;
	iPoint range[RANGE_SIZE];

	iPoint velocity;

	iPoint first_point;
	iPoint last_point;

public:
	Enemy_level01_ground(int x, int y);

	void Move();
	void Update(float dt);
	void CreateRange();

	bool PlayerIsInRange();

	void PathMovement(const p2DynArray<iPoint>*, iPoint, float); // p2DynArray needs to be const to call Last Path
};

#endif // __Enemy_level01_ground__