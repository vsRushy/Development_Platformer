#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__

#include "j1Module.h"
#include "p2DynArray.h"

#include "SDL/include/SDL.h"

class Entity;
class Player;

enum class ENTITY_TYPES
{
	UNKNOWN,
	ENEMY_LEVEL01_GROUND,
	ENEMY_LEVEL01_AIR,
	PLAYER,
	ENEMY_LEVEL02_GROUND,
	ENEMY_LEVEL02_AIR,
	COINS,
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	virtual ~j1EntityManager();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public:
	Entity* CreateEntity(ENTITY_TYPES type, int x, int y);
	void DeleteEntity(Entity* e);
	void DeleteAllEntities();
	void OnCollision(Collider* a, Collider* b);

private:
	p2DynArray<Entity*> entities = NULL;

	SDL_Texture* player_tex;
	SDL_Texture* enemy_level01_ground_tex;
	SDL_Texture* enemy_level01_air_tex;
	SDL_Texture* Coins_tex;
};

#endif // __j1EntityManager_H__