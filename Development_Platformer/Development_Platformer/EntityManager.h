#ifndef __EntityManager_H__
#define __EntityManager_H__

#include "j1Module.h"
#include "Entity.h"

#include "Player.h"
#include "Enemy_level01_air.h"
#include "Enemy_level01_ground.h"

#define MAX_ENTITIES 50

enum class ENTITY_TYPES
{
	NO_TYPE,
	PLAYER,
	ENEMY_LEVEL01_GROUND,
	ENEMY_LEVEL01_AIR,
	ENEMY_LEVEL02_GROUND,
	ENEMY_LEVEL02_AIR,
};

class Entity;

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
};

class EntityManager : public j1Module
{
public:

	EntityManager();
	~EntityManager();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool AddEntity(ENTITY_TYPES type, int x, int y);

	bool Awake(pugi::xml_node& data);

	bool Load(pugi::xml_node save);
	bool Save(pugi::xml_node save) const;

	PlayerInfo& GetPlayerInfo();

public:
	Entity * CreateEntity(ENTITY_TYPES type)
	{
		static_assert(Entity::ENTITY_TYPES::NO_TYPE == 5, "code needs update");
	}
	void SpawnEntity(const EntityInfo& info);

public:

	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];

	SDL_Texture* player_tex;
	SDL_Texture* enemy_level01_ground_tex;
	SDL_Texture* enemy_level01_air_tex;

	PlayerInfo player;
	Player* playerData;
};

#endif // __EntityManager_H__