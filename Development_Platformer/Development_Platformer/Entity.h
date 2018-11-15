#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"

#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct Collider;

class Entity
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

public:
	fPoint position;
	int enemy_death_sound = NULL;

	int hit_points = 1; // Default HP; may change in every enemy
	int type;

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites, float speed = 1.0f);
	virtual void OnCollision(Collider* collider, int type) {};
};

#endif // __ENTITY_H__