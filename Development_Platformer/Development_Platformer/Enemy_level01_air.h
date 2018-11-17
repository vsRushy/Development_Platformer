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

	bool is_moving_to_player = false;
	bool is_moving_to_origin = false;
	bool player_is_range = false;
	
	iPoint objective;
	iPoint range[RANGE_SIZE];

	iPoint velocity;

public:
	Enemy_level01_air(int x, int y);
	~Enemy_level01_air() {};

	void Update(float dt);
	void CreateRange();

	bool PlayerIsInRange();
	int GetHP() { return hit_points; };

	void PathMovement(const p2DynArray<iPoint>*, iPoint, float);

	void OnCollision(Collider* a, Collider* b);
};

#endif // __Enemy_level01_air__