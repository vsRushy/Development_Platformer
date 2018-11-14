#ifndef __j1PARTICLES_H__
#define __j1PARTICLES_H__
#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1Collision.h"
#include "p2Log.h"
#define MAX_ACTIVE_PARTICLES 1
struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;
struct Particle
{
	int type_particle;
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	iPoint position;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;
	int tipe;
	Particle();
	Particle(const Particle& p);
	virtual ~Particle();
	bool Update(float dt);
	enum TYPE_PARTICLE {
		SHOOT = 0,
		SHOOT_LEFT = 0,
		EXPLOSION,
		EXPLOSION_ENEMY,
	} Type = TYPE_PARTICLE::SHOOT;
};
class j1Particles : public j1Module
{
public:
	j1Particles();
	virtual ~j1Particles();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type = COLLIDER_NONE, Uint32 delay = 0);
private:
	SDL_Texture* sword_tex = nullptr;
	Particle* active[MAX_ACTIVE_PARTICLES];
public:
	uint width, height;
	Particle sword_right;
	Particle sword_left;
};
#endif // __j1PARTICLES_H__ 