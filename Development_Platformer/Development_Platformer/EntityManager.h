#ifndef __EntityManager_H__
#define __EntityManager_H__

#include "j1Module.h"
#include "Entity.h"

#define MAX_ENTITIES 50

enum class ENTITY_TYPES
{
	NO_TYPE,
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

	bool Load(pugi::xml_node save);
	bool Save(pugi::xml_node save) const;

private:

	void SpawnEntity(const EntityInfo& info);

public:

	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];

	SDL_Texture* enemy_level01_ground_tex;
	SDL_Texture* enemy_level01_air_tex;
};

#endif // __EntityManager_H__