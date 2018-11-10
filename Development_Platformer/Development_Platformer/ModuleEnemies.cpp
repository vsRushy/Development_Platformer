#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "ModuleEnemies.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "Enemy.h"
#include "Enemy_level01_ground.h"
#include "Enemy_level01_air.h"

#define SPAWN_MARGIN (/*60*/0 * App->win->GetScale())

ModuleEnemies::ModuleEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	enemy_level01_ground_tex = App->tex->Load("textures/Enemies/enemy_level01_ground.png");
	enemy_level01_air_tex = App->tex->Load("textures/Enemies/enemy_level01_air.png");

	return true;
}

bool ModuleEnemies::PreUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			SpawnEnemy(queue[i]);
			queue[i].type = ENEMY_TYPES::NO_TYPE;
			LOG("Spawning enemy at %d, %d", queue[i].x * App->win->GetScale(), queue[i].y * App->win->GetScale());
		}
	}

	return true;
}

// Called before render is available
bool ModuleEnemies::Update(float dt)
{
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();*/

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Update(dt);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr && enemies[i]->type == 1 /* && CHECK WHAT MAP WE ARE IN */) 
			enemies[i]->Draw(enemy_level01_ground_tex);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr && enemies[i]->type == 2 /* && CHECK WHAT MAP WE ARE IN */)
			enemies[i]->Draw(enemy_level01_air_tex);

	return true;
}

bool ModuleEnemies::PostUpdate()
{
	// check camera position to decide what to spawn
	/* ENEMIES WONT DESPAWN, THEY NEED TO BE KILLED :)
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->type == 1) {
				if (enemies[i]->position.x * App->win->GetScale() + (App->render->camera.x) * 0.2f < (App->render->camera.x) - SPAWN_MARGIN)
				{
					LOG("DeSpawning enemy at %d", enemies[i]->position.x * App->win->GetScale());
					delete enemies[i];
					enemies[i] = nullptr;
				}
			}
			else 
			if (enemies[i]->position.x * App->win->GetScale() < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * App->win->GetScale());
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
	*/

	return true;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(enemy_level01_ground_tex);
	App->tex->UnLoad(enemy_level01_air_tex);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::SpawnEnemy(const EnemyInfo& info)
{
	uint i = 0;

	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::ENEMY_LEVEL01_GROUND:
			enemies[i] = new Enemy_level01_ground(info.x, info.y);
			enemies[i]->type = 1;
			break;
		case ENEMY_TYPES::ENEMY_LEVEL01_AIR:
			enemies[i] = new Enemy_level01_air(info.x, info.y);
			enemies[i]->type = 2;
			break;
		default:
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			
		}
	}
}