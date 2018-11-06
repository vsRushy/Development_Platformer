#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;
struct Mix_Chunk;

class Enemy
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;

public:
	fPoint position;
	int enemy_death_sound = NULL;

	int hit_points = 1; // Default HP; may change in every enemy
	int type;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites, float speed = 1.0f);
	virtual void OnCollision(Collider* collider, int type);
};

#endif // __ENEMY_H__