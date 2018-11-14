#include "j1App.h"
#include "Enemy_level01_ground.h"
#include "j1Collision.h"
#include "ModuleEnemies.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "p2Log.h"

#define GROUND_SIZE_01_X 17
#define GROUND_SIZE_01_Y 22

Enemy_level01_ground::Enemy_level01_ground(int x, int y) : Enemy(x, y)
{

	anim.PushBack({ 1, 2, GROUND_SIZE_01_X, GROUND_SIZE_01_Y });
	anim.loop = true;
	anim.speed = 0.5f;

	hit_points = 4;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, GROUND_SIZE_01_X, GROUND_SIZE_01_Y }, COLLIDER_TYPE::COLLIDER_ENEMY,
		(j1Module*)App->enemies);

	original_pos.x = x;
	original_pos.y = y;
}

void Enemy_level01_ground::Move()
{
	/*position.x = position.x;
	position.y = position.y;*/
}


void Enemy_level01_ground::Update(float dt)
{
	CreateRange();

	// Tile where the enemy is
	iPoint enemy_map_pos = App->map->WorldToMap((int)position.x, (int)position.y);

	// Player is in the range of the enemy?
	player_is_range = PlayerIsInRange();

	//Then allow the player to move
	if (player_is_range)
	{
		is_moving = true;
	}
	else
	{
		is_moving = false;
	}

	if (is_moving)
	{
		const p2DynArray<iPoint>* path;
		int c = App->pathfinding->CreatePath(enemy_map_pos, objective);
		if (c != -1)
		{
			path = App->pathfinding->GetLastPath();
			PathMovement(path, enemy_map_pos, dt);
		}
	}

	position.x += velocity.x;
	position.y += velocity.y;
}

void Enemy_level01_ground::CreateRange()
{
	uint counter = 0;
	for (int i = -4; i < 5; i++)
		for (int k = -4; k < 5; k++)
		{
			iPoint tmp_range = App->map->WorldToMap((int)position.x, (int)position.y);
			/* Representation of the enemy range
		
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

void Enemy_level01_ground::PathMovement(const p2DynArray<iPoint>* path, iPoint position, float dt)
{
	iPoint goal = iPoint(path->At(0)->x, path->At(0)->y); // we want the initial point of the path!
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	dt = 1.0f;
	if (goal.x < position.x)
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

	velocity.x = velocity_x;
	velocity.y = velocity_y;
}

bool Enemy_level01_ground::PlayerIsInRange()
{
	bool ret = false;

	iPoint player_pos = App->map->WorldToMap((int)App->player->position.x, (int)App->player->position.y);

	for (int i = 0; i < RANGE_SIZE; i++)
	{
		if (range[i] == player_pos)
		{
			//LOG("PLAYER DETECTEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
			//LOG("Objective [x]: %i [y]: %i", objective.x, objective.y);
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