#ifndef __Enemy_level01_air__
#define __Enemy_level01_air__

#include "Entity.h"

#include "p2Point.h"
#include "p2DynArray.h"

#define RANGE_SIZE 80

class Enemy_level01_air : public Entity
{
private:
	iPoint original_pos;
	Animation anim;
	
	iPoint range[RANGE_SIZE];

public:
	Enemy_level01_air(int x, int y);

	void Update(float dt);
	void CreateRange();

	bool PlayerIsInRange();

	void PathMovement(const p2DynArray<iPoint>*, iPoint, float);

	void OnCollision(Collider* a, Collider* b);
};

#endif // __Enemy_level01_air__