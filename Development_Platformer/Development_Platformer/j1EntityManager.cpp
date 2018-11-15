#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "Entity.h"
#include "Enemy_level01_ground.h"
#include "Enemy_level01_air.h"

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
	enemy_level01_ground_tex = App->tex->Load("textures/Enemies/enemy_level01_ground.png");
	enemy_level01_air_tex = App->tex->Load("textures/Enemies/enemy_level01_air.png");

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
	LOG("Freeing all enemies");

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

bool j1EntityManager::Load(pugi::xml_node&)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node&) const
{
	return true;
}