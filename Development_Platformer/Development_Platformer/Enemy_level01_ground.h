#ifndef __Enemy_level01_ground__
#define __Enemy_level01_ground__

#include "Entity.h"

#include "p2Point.h"
#include "p2DynArray.h"

#define RANGE_SIZE 80

class Enemy_level01_ground : public Entity
{
private:
	iPoint original_pos;
	Animation idle;
	Animation walk;

	iPoint range[RANGE_SIZE];

public:
	Enemy_level01_ground(int x, int y);

	void Update(float dt);
	void CreateRange();

	bool PlayerIsInRange();

	void PathMovement(const p2DynArray<iPoint>*, iPoint, float);

	void OnCollision(Collider* a, Collider* b);
};

#endif // __Enemy_level01_ground__