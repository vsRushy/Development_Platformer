#include "j1App.h"
#include "Enemy_level01_air.h"
#include "j1Collision.h"
#include "ModuleEnemies.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "p2Log.h"

#define AIR_SIZE_01 32

Enemy_level01_air::Enemy_level01_air(int x, int y) : Enemy(x, y)
{

	anim.PushBack({ 64, 82, AIR_SIZE_01, AIR_SIZE_01 });
	anim.loop = true;
	anim.speed = 0.5f;

	hit_points = 4;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, AIR_SIZE_01, AIR_SIZE_01 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);

	original_pos.x = x;
	original_pos.y = y;
}

void Enemy_level01_air::Move()
{
	/*position.x = position.x;
	position.y = position.y;*/
}

void Enemy_level01_air::Update(float dt)
{
	CreateRange();

	// Tile where the enemy is
	iPoint enemy_map_pos = App->map->WorldToMap(position.x, position.y);

	// Player is in the range of the enemy?
	player_is_range = PlayerIsInRange();

	//Then allow the player to move
	if (player_is_range)
	{
		is_moving = true;
	}

	if (is_moving)
	{
		const p2DynArray<iPoint>* path;
		if (App->pathfinding->CreatePath(enemy_map_pos, objective) != -1)
		{
			path = App->pathfinding->GetLastPath();
			PathMovement(path, enemy_map_pos, dt);
		}
	}

	position.x += velocity.x;
	position.y += velocity.y;
}

void Enemy_level01_air::CreateRange()
{
	uint counter = 0;
	for (int i = -4; i < 5; i++)
		for (int k = -4; k < 5; k++)
		{
			iPoint tmp_range = App->map->WorldToMap(position.x, position.y);
			/* Representation of the enemy range
				*********
				*********
			    *********
				*********
				****E****
				*********
				*********
				*********
				*********
			*/
			if (!(i == 0 && k == 0)) 
			{
				tmp_range.x += i;
				tmp_range.y += k;
				range[counter++] = tmp_range; // add it to the range var
			}
		}
}

void Enemy_level01_air::PathMovement(const p2DynArray<iPoint>* path, iPoint position, float dt)
{
	iPoint goal = iPoint(path->At(0)->x, path->At(0)->y); // we want the initial point of the path!
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	dt = 1.0f;
	if(goal.x < position.x)
	{ 
		velocity_x = -5.0f * dt;
	}
	else if (goal.x > position.x)
	{
		velocity_x = 5.0f * dt;
	}
	else if (goal.y < position.y)
	{
		velocity_y = -5.0f * dt;
	}
	else if (goal.y > position.y)
	{
		velocity_y = 5.0f * dt;
	}

	is_moving = true;
	velocity.x = velocity_x;
	velocity.y = velocity_y;
}

bool Enemy_level01_air::PlayerIsInRange()
{
	bool ret = false;

	iPoint player_pos = App->map->WorldToMap((int)App->player->position.x, (int)App->player->position.y);

	for (int i = 0; i < RANGE_SIZE; i++)
	{
		if (range[i] == player_pos)
		{
			LOG("PLAYER DETECTEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
			LOG("Objective [x]: %i [y]: %i", objective.x, objective.y);
			// Now we have the objective
			objective = range[i];
			if (objective == iPoint(App->map->WorldToMap(position.x, position.y)))
				ret = false;
			else
				ret = true;
		}
	}

	return ret;
}