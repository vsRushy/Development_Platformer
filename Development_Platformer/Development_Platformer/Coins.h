#ifndef __COINS__
#define __COINS__

#include "Entity.h"

class Coins : public Entity
{
private:
	Animation spin;
public:
	Coins(int x, int y);
	~Coins() {};

	void Update(float dt);

	void OnCollision(Collider* a, Collider* b);
};

#endif // __COINS__