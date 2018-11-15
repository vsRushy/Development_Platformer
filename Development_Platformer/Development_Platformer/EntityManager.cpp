#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "Entity.h"
#include "Enemy_level01_ground.h"
#include "Enemy_level01_air.h"

EntityManager::EntityManager()
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
		entities[i] = nullptr;
}

// Destructor
EntityManager::~EntityManager()
{
}

bool EntityManager::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	enemy_level01_ground_tex = App->tex->Load("textures/Enemies/enemy_level01_ground.png");
	enemy_level01_air_tex = App->tex->Load("textures/Enemies/enemy_level01_air.png");

	return true;
}

bool EntityManager::PreUpdate()
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::NO_TYPE)
		{
			SpawnEntity(queue[i]);
			queue[i].type = ENTITY_TYPES::NO_TYPE;
			LOG("Spawning entity at %d, %d", queue[i].x * App->win->GetScale(), queue[i].y * App->win->GetScale());
		}
	}

	return true;
}

// Called before render is available
bool EntityManager::Update(float dt)
{
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();*/

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr) entities[i]->Update(dt);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == (int)ENTITY_TYPES::ENEMY_LEVEL01_GROUND)
			entities[i]->Draw(enemy_level01_ground_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == (int)ENTITY_TYPES::ENEMY_LEVEL01_AIR)
			entities[i]->Draw(enemy_level01_air_tex);

	return true;
}

bool EntityManager::PostUpdate()
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
bool EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(enemy_level01_ground_tex);
	App->tex->UnLoad(enemy_level01_air_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	return true;
}

bool EntityManager::AddEntity(ENTITY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPES::NO_TYPE)
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

void EntityManager::SpawnEntity(const EntityInfo& info)
{
	uint i = 0;

	for (; entities[i] != nullptr && i < MAX_ENTITIES; ++i);

	if (i != MAX_ENTITIES)
	{
		switch (info.type)
		{
		case ENTITY_TYPES::ENEMY_LEVEL01_GROUND:
			entities[i] = new Enemy_level01_ground(info.x, info.y);
			entities[i]->type = 1;
			break;
		case ENTITY_TYPES::ENEMY_LEVEL01_AIR:
			entities[i] = new Enemy_level01_air(info.x, info.y);
			entities[i]->type = 2;
			break;
		default:
			break;
		}
	}
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr && entities[i]->GetCollider() == c1)
		{
			
		}
	}
}