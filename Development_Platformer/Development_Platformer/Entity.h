#ifndef __Entity_H__
#define __Entity_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2DynArray.h"

#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct Collider;

class Entity
{
protected:
	Collider* collider = nullptr;
	Animation* animation = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

public:
	fPoint position;

	bool is_alive = true;
	int hit_points = 1;
	int type;

	// -------------
	iPoint objective;
	iPoint velocity;
	bool is_moving = true;
	bool player_is_range = false;
	virtual void CreateRange() {};
	virtual bool PlayerIsInRange() { return true; };
	virtual void PathMovement(const p2DynArray<iPoint>*, iPoint, float) {};

public:
	Entity(int x, int y);
	virtual ~Entity(); // needs to be virtual

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites, float speed = 1.0f);
	virtual void OnCollision(Collider* a, Collider* b) {};

	const Collider* GetCollider() const;
};

#endif // __Entity_H__