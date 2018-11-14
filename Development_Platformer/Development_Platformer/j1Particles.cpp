#include <math.h>
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Particles.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1EntityManager.h"

#include "SDL/include/SDL_timer.h"

j1Particles::j1Particles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
	// Basic_shoot 0_up
	sword_right.anim.PushBack({ 6, 12, 35, 22 });
	sword_right.anim.loop = true;
	sword_right.speed.x = 150;
	sword_right.life = 2500;
	sword_right.type_particle = 0;

	sword_left.anim.PushBack({ 6, 45, 35, 22 });
	sword_left.anim.loop = true;
	sword_left.speed.x = -150;
	sword_left.life = 2500;
	sword_left.type_particle = 1;
}

j1Particles::~j1Particles()
{}

// Load assets
bool j1Particles::Start()
{
	LOG("Loading particles");
	sword_tex = App->tex->Load("textures/sword.png");
	App->win->GetWindowSize(width, height);
	return true;
}

// Unload assets
bool j1Particles::CleanUp()
{
	LOG("Unloading particles");
	App->tex->UnLoad(sword_tex);
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}
	return true;
}

// Update: draw background
bool j1Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];
		if (p == nullptr)
			continue;
		if (p->Update(dt) == false)
		{
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			switch (p->Type) {
			case p->SHOOT:
				App->render->Blit(sword_tex, p->position.x, p->position.y, &(p->anim.GetCurrentFrame()));
				break;
			}
			if (p->fx_played == false)
			{
				p->fx_played = true;
			}
		}
	}
	return true;
}

void j1Particles::AddParticle(const Particle& particle, int x, int y, COLLIDER_TYPE collider_type, Uint32 delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] == nullptr)
		{
			Particle* p = new Particle(particle);
			p->born = SDL_GetTicks() + delay;
			p->position.x = x;
			p->position.y = y;
			p->Type = particle.Type;
			if (collider_type != COLLIDER_NONE) {
				p->collider = App->collision->AddCollider(p->anim.GetCurrentFrame(), collider_type, this);
				p->collider->type = COLLIDER_PLAYER_SHOT; //if we don't put more particles
			}
			active[i] = p;
			break;
		}
	}
}
// every time a particle hits a wall it triggers an explosion particle
void j1Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (active[i] != nullptr && active[i]->collider == c1)
		{
			//code
			//App->particles->AddParticle(App->particles->explosion, active[i]->position.x, active[i]->position.y);
			//active[i]->collider->to_delete = true;
			delete active[i];
			active[i] = nullptr;
			break;
		}
	}
}

// -------------------------------------------------------------
// -------------------------------------------------------------
Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

Particle::~Particle()
{
	if (collider != nullptr) {
		collider->to_delete = true;
	}
}

bool Particle::Update(float dt)
{
	bool ret = true;
	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}
	else
		if (anim.Finished())
			ret = false;
	position.x += speed.x * dt;
	position.y += speed.y * dt;

	iPoint worldPos = App->map->WorldToMap(position.x, position.y);
	iPoint worldFinalPos = App->map->WorldToMap(position.x + 35 + speed.x*dt, position.y + 15);
	if (App->map->CheckCollisionX(worldPos.x, worldPos.y, worldFinalPos.y))//left
	{
		life = 0;
	}
	else if (App->map->CheckCollisionX(worldFinalPos.x, worldPos.y, worldFinalPos.y))//right
	{
		life = 0;
	}
	if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == 1) && life > 0)
	{
		App->entity_manager->ReturnPlayer()->position.x = position.x;
		App->entity_manager->ReturnPlayer()->position.y = position.y;
		life = 0;
	}
	/*if (collider != nullptr) {
	if (position.y < App->render->camera.y / App->win->GetScale() || position.x < App->render->camera.x / App->win->GetScale() || position.x > App->render->camera.x / App->win->GetScale() + width - collider->rect.w || position.y > App->render->camera.y / App->win->GetScale() + height - collider->rect.h)
	{
	ret = false;
	return ret;
	}
	}*/
	if (collider != nullptr) {
		collider->SetPos(position.x, position.y);
		if (collider->type == COLLIDER_PLAYER_SHOT)
			collider->SetPos(position.x, position.y);
	}
	return ret;
}