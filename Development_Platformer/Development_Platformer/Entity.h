#ifndef __Entity_H__
#define __Entity_H__

#include "p2Point.h"
#include "Animation.h"
#include "j1EntityManager.h"

#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct Collider;

class Entity
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int hit_points = 1;

public:
	fPoint position;
	bool is_alive = true;
	ENTITY_TYPES type = ENTITY_TYPES::UNKNOWN;

public:
	Entity(ENTITY_TYPES type, int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites, float speed = 1.0f);
	virtual int GetHP() { return 0; };
	virtual void OnCollision(Collider* a, Collider* b) {};
};

#endif // __Entity_H__