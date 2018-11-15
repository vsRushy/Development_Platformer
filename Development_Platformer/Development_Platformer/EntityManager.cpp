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
#include "Player.h"

EntityManager::EntityManager()
{
	name.create("entities");

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		entities[i] = nullptr;
}

// Destructor
EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& data)
{
	player.god_mode = data.child("god_mode").attribute("value").as_bool();
	player.gravity = data.child("gravity").attribute("value").as_float();
	player.velocity_x = data.child("velocity_x").attribute("value").as_float();
	player.velocity_y = data.child("velocity_x").attribute("value").as_float();
	
	return true;
}

bool EntityManager::Start()
{
	LOG("Loading entities textures...");
	player_tex = App->tex->Load("textures/characters.png");
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
	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == (int)ENTITY_TYPES::PLAYER)
			entities[i]->Draw(player_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == (int)ENTITY_TYPES::ENEMY_LEVEL01_GROUND)
			entities[i]->Draw(enemy_level01_ground_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == (int)ENTITY_TYPES::ENEMY_LEVEL01_AIR)
			entities[i]->Draw(enemy_level01_air_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr) entities[i]->Update(dt);

	return true;
}

bool EntityManager::PostUpdate()
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr && !entities[i]->is_alive)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(player_tex);
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
		case ENTITY_TYPES::PLAYER:
			playerData = new Player(info.x, info.y);
			entities[i] = playerData;
			entities[i]->type = 1;
			break;
		case ENTITY_TYPES::ENEMY_LEVEL01_GROUND:
			entities[i] = new Enemy_level01_ground(info.x, info.y);
			entities[i]->type = 2;
			break;
		case ENTITY_TYPES::ENEMY_LEVEL01_AIR:
			entities[i] = new Enemy_level01_air(info.x, info.y);
			entities[i]->type = 3;
			break;
		default:
			break;
		}
	}
}

bool EntityManager::Load(pugi::xml_node save)
{
	if (save.child("position") != NULL)
	{
		playerData->position.x = save.child("position").attribute("x").as_float();
		playerData->position.y = save.child("position").attribute("y").as_float();
	}

	return true;
}

bool EntityManager::Save(pugi::xml_node save) const
{
	if (save.child("position") == NULL)
	{
		save.append_child("position").append_attribute("x") = playerData->position.x;
		save.child("position").append_attribute("y") = playerData->position.y;
	}
	else {
		save.child("position").attribute("x") = playerData->position.x;
		save.child("position").attribute("y") = playerData->position.y;
	}

	return true;
}


PlayerInfo& EntityManager::GetPlayerInfo()
{
	return player;
}