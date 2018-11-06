#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "j1Module.h"
#include "Enemy.h"

#define MAX_ENEMIES 350

enum ENEMY_TYPES
{
	NO_TYPE,
	ENEMY_LEVEL01_GROUND,
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

class ModuleEnemies : public j1Module
{
public:

	ModuleEnemies();
	virtual ~ModuleEnemies();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y);

private:

	void SpawnEnemy(const EnemyInfo& info);

public:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

	SDL_Texture* enemy_level01_ground_tex;
};

#endif // __ModuleEnemies_H__