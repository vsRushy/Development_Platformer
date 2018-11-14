#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "Player.h"
#include "Enemy_level01_air.h"
#include "Enemy_level01_ground.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		entities[i] = nullptr;
}

// Destructor
j1EntityManager::~j1EntityManager()
{

}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	player_entity_tex = App->tex->Load("textures/characters.png");
	if (player_entity_tex != nullptr)
	{
		LOG("Player entity texture loaded correctly");
		ret = true;
	}
	else
	{
		LOG("Player entity texture cannot be loaded");
		ret = false;
	}

	enemy_level01_air_tex = App->tex->Load("textures/Enemies/enemy_level01_air.png");
	if (player_entity_tex != nullptr)
	{
		LOG("Enemy level 01 air entity texture loaded correctly");
		ret = true;
	}
	else
	{
		LOG("Enemy level 01 air entity texture cannot be loaded");
		ret = false;
	}

	enemy_level01_ground_tex = App->tex->Load("textures/Enemies/enemy_level01_ground.png");
	if (player_entity_tex != nullptr)
	{
		LOG("Enemy level 01 ground entity texture loaded correctly");
		ret = true;
	}
	else
	{
		LOG("Enemy level 01 ground entity texture cannot be loaded");
		ret = false;
	}

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPE::UNKNOWN)
		{
			SpawnEntity(queue[i]);
			queue[i].type = ENTITY_TYPE::UNKNOWN;
			LOG("Entity spawned at [x]: %i [y]: %i", queue[i].x * App->win->GetScale(), queue[i].y * App->win->GetScale());
		}
	}

	return true;
}

// Called before render is available
bool j1EntityManager::Update(float dt)
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr) entities[i]->Update(dt);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == 0)
			entities[i]->Draw(player_entity_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == 1)
			entities[i]->Draw(enemy_level01_air_tex);

	for (uint i = 0; i < MAX_ENTITIES; ++i)
		if (entities[i] != nullptr && entities[i]->type == 2)
			entities[i]->Draw(enemy_level01_ground_tex);

	return true;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr && !entities[i]->is_alive)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	return ret;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(player_entity_tex);
	App->tex->UnLoad(enemy_level01_air_tex);
	App->tex->UnLoad(enemy_level01_ground_tex);

	player = nullptr;

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

bool j1EntityManager::AddEntity(ENTITY_TYPE type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPE::UNKNOWN)
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

void j1EntityManager::SpawnEntity(const EntityInfo& info)
{
	uint i = 0;
	for (; entities[i] != nullptr && i < MAX_ENTITIES; ++i);

	if (i != MAX_ENTITIES)
	{
		switch (info.type)
		{
		case ENTITY_TYPE::PLAYER:
			player = new Player(info.x, info.y);
			entities[i] = player;
			entities[i]->type = 0;
			break;
		case ENTITY_TYPE::ENEMY_01_GROUND:
			entities[i] = new Enemy_level01_air(info.x, info.y);
			entities[i]->type = 1;
			break;
		case ENTITY_TYPE::ENEMY_01_AIR:
			entities[i] = new Enemy_level01_ground(info.x, info.y);
			entities[i]->type = 2;
			break;
		default:
			break;
		}
	}
}

void j1EntityManager::OnCollision(Collider* a, Collider* b)
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr && (entities[i]->GetCollider() == a)) {

			entities[i]->OnCollision(a, b);
			break; // break;
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& save)
{
	bool ret = true;

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (player != nullptr && entities[i] == player)
		{
			if (save.child("position") != NULL)
			{
				player->position.x = save.child("position").attribute("x").as_float();
				player->position.y = save.child("position").attribute("y").as_float();
			}
		}
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& save) const
{
	bool ret = true;
	
	if (save.child("position") == NULL)
	{
		save.append_child("position").append_attribute("x") = player->position.x;
		save.child("position").append_attribute("y") = player->position.y;
	}
	else {
		save.child("position").attribute("x") = player->position.x;
		save.child("position").attribute("y") = player->position.y;
	}

	return ret;
}

void j1EntityManager::AddAllEntities()
{
	
}