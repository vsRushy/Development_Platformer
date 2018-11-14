#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__

#include "j1Module.h"
#include "Animation.h"
#include "Player.h"

#define MAX_ENTITIES 50

class Entity;
struct SDL_Texture;

enum class ENTITY_TYPE
{
	UNKNOWN = -1,
	PLAYER,
	ENEMY_01_AIR,
	ENEMY_01_GROUND
};

struct EntityInfo
{
	ENTITY_TYPE type = ENTITY_TYPE::UNKNOWN;
	int x, y;
};

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	virtual ~j1EntityManager();

	bool Awake(pugi::xml_node& conf);

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider* a, Collider* b);

	bool AddEntity(ENTITY_TYPE type, int x, int y);
	void SpawnEntity(const EntityInfo& info);

	Player* ReturnPlayer() const;

private:
	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];

	Player* player = nullptr;

	SDL_Texture* player_entity_tex = nullptr;
	SDL_Texture* enemy_level01_air_tex = nullptr;
	SDL_Texture* enemy_level01_ground_tex = nullptr;
};

#endif // __j1Enemies_H__