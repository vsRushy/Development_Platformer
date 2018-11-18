#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Audio.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy_level01_ground.h"
#include "Enemy_level01_air.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");
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
	player_tex = App->tex->Load("textures/characters.png");
	enemy_level01_ground_tex = App->tex->Load("textures/Enemies/enemy_level01_ground.png");
	enemy_level01_air_tex = App->tex->Load("textures/Enemies/enemy_level01_air.png");

	LOG("Loading player sound effects");
	App->audio->LoadFx("jump.wav"); // id: 1
	App->audio->LoadFx("dash.wav"); // id: 2
	App->audio->LoadFx("arrow.wav"); // id: 3

	return true;
}

bool j1EntityManager::PreUpdate()
{


	return true;
}

// Called before render is available
bool j1EntityManager::Update(float dt)
{
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr) entities[i]->Update(dt);

	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->type == ENTITY_TYPES::PLAYER)
			entities[i]->Draw(player_tex);

	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->type == ENTITY_TYPES::ENEMY_LEVEL01_GROUND)
			entities[i]->Draw(enemy_level01_ground_tex);

	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->type == ENTITY_TYPES::ENEMY_LEVEL01_AIR)
			entities[i]->Draw(enemy_level01_air_tex);

	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("Freeing all entities");

	App->tex->UnLoad(player_tex);
	App->tex->UnLoad(enemy_level01_ground_tex);
	App->tex->UnLoad(enemy_level01_air_tex);

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	return true;
}

Entity* j1EntityManager::CreateEntity(ENTITY_TYPES type, int x, int y)
{
	//static_assert(? , "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case ENTITY_TYPES::PLAYER:
		ret = new Player(x, y);
		break;
	case ENTITY_TYPES::ENEMY_LEVEL01_GROUND:
		ret = new Enemy_level01_ground(x, y);
		break;
	case ENTITY_TYPES::ENEMY_LEVEL01_AIR:
		ret = new Enemy_level01_air(x, y);
		break;
	default:
		break;
	}

	if (ret != nullptr)
		entities.PushBack(ret);

	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& save)
{
	bool ret = true;

	pugi::xml_node node;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (entities[i]->type == ENTITY_TYPES::PLAYER)
		{
			if (save.child("player") != NULL)
			{
				node = save.child("player");
			}
			if (node.child("position") != NULL)
			{
				entities[i]->position.x = node.child("position").attribute("x").as_float();
				entities[i]->position.y = node.child("position").attribute("y").as_float();
			}
		}
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& save) const
{
	bool ret = true;

	pugi::xml_node node;
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (entities[i]->type == ENTITY_TYPES::PLAYER)
		{
			if (save.child("player") == NULL)
			{
				node = save.append_child("player");
			}
			else
			{
				node = save.child("player");
			}
			if (node.child("position") == NULL)
			{
				node.append_child("position").append_attribute("x") = entities[i]->position.x;
				node.child("position").append_attribute("y") = entities[i]->position.y;
			}
			else
			{
				node.child("position").append_attribute("x") = entities[i]->position.x;
				node.child("position").append_attribute("y") = entities[i]->position.y;
			}
		}

		if (entities[i]->type == ENTITY_TYPES::ENEMY_LEVEL01_AIR)
		{
			if (save.child("enemy_level01_air") == NULL)
			{
				node = save.append_child("enemy_level01_air");
			}
			else
			{
				node = save.child("enemy_level01_air");
			}
			if (node.child("position") == NULL)
			{
				node.append_child("position").append_attribute("x") = entities[i]->position.x;
				node.child("position").append_attribute("y") = entities[i]->position.y;
			}
			else
			{
				node.child("position").append_attribute("x") = entities[i]->position.x;
				node.child("position").append_attribute("y") = entities[i]->position.y;
			}
		}

		if (entities[i]->type == ENTITY_TYPES::ENEMY_LEVEL01_GROUND)
		{
			if (save.child("enemy_level01_ground") == NULL)
			{
				node = save.append_child("enemy_level01_ground");
			}
			else
			{
				node = save.child("enemy_level01_ground");
			}
			if (node.child("position") == NULL)
			{
				node.append_child("position").append_attribute("x") = entities[i]->position.x;
				node.child("position").append_attribute("y") = entities[i]->position.y;
			}
			else
			{
				node.child("position").append_attribute("x") = entities[i]->position.x;
				node.child("position").append_attribute("y") = entities[i]->position.y;
			}
		}
	}

	return ret;
}

void j1EntityManager::OnCollision(Collider* a, Collider* b)
{
	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr && entities[i]->GetCollider() == a)
		{
			if (b->type == COLLIDER_TYPE::COLLIDER_PLAYER_SHOT)
			{
				delete entities[i];
				entities[i] = nullptr;
				break;
			}
			if (b->type == COLLIDER_TYPE::COLLIDER_PLAYER && a->type == COLLIDER_TYPE::COLLIDER_ENEMY) {
				
				if (App->scene->map_selected == 1) {
					App->scene->player->position = App->scene->first_map_pos;
				}
				else if (App->scene->map_selected == 2) {
					App->scene->player->position = App->scene->second_map_pos;
				}
				App->scene->player->Reset();
			}
		}
	}
}

void j1EntityManager::DeleteEntity(Entity* e)
{
	for (int i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] == e)
		{
			delete entities[i];
			entities[i] = nullptr;
			break;
		}
	}
}

void j1EntityManager::DeleteAllEntities()
{
	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
}