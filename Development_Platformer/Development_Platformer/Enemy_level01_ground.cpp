#include "j1App.h"
#include "Enemy_level01_ground.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "j1Pathfinding.h"
#include "p2Log.h"

#define GROUND_SIZE_01_X 17
#define GROUND_SIZE_01_Y 22

Enemy_level01_ground::Enemy_level01_ground(int x, int y) : Entity(type, x, y)
{
	idle.PushBack({ 1, 2, GROUND_SIZE_01_X, GROUND_SIZE_01_Y });
	idle.loop = true;
	idle.speed = 0.5f;

	walk.PushBack({ 1, 120, 21, 21 });
	walk.PushBack({ 23, 120, 18, 21 });
	walk.PushBack({ 44, 120, 17, 21 });
	walk.PushBack({ 66, 120, 18, 21 });
	walk.PushBack({ 88, 120, 18, 21 });
	walk.PushBack({ 109, 120, 21, 21 });
	walk.PushBack({ 131, 120, 18, 21 });
	walk.PushBack({ 153, 120, 16, 21 });
	walk.PushBack({ 175, 120, 18, 21 });
	walk.PushBack({ 196, 120, 18, 21 });
	walk.loop = true;
	walk.speed = 0.5f;

	hit_points = 1;

	animation = &idle;

	type = ENTITY_TYPES::ENEMY_LEVEL01_GROUND;

	collider = App->collision->AddCollider({ 0, 0, GROUND_SIZE_01_X, GROUND_SIZE_01_Y }, COLLIDER_TYPE::COLLIDER_ENEMY,
		(j1Module*)App->entities);

	original_pos.x = x;
	original_pos.y = y;

	flip = SDL_FLIP_NONE;

	CreateRange();
}

void Enemy_level01_ground::Update(float dt)
{
	float anim_speed = 10.0f;
	idle.speed = anim_speed * dt;
	walk.speed = anim_speed * dt;

	// Starting enemy tile
	iPoint map_original_pos = App->map->WorldToMap(original_pos.x, original_pos.y);

	// Tile where the enemy is
	iPoint enemy_map_pos = App->map->WorldToMap((int)position.x, (int)position.y);

	// Player is in the range of the enemy?
	player_is_range = PlayerIsInRange();

	//Then allow the player to move
	if (player_is_range)
	{
		is_moving_to_player = true;
		is_moving_to_origin = false;
	}
	else
	{
		is_moving_to_player = false;
		if (enemy_map_pos != map_original_pos)
			is_moving_to_origin = true;
	}

	if (is_moving_to_player)
	{
		const p2DynArray<iPoint>* path;
		int c = App->pathfinding->CreatePath(enemy_map_pos, objective);
		if (c != -1)
		{
			path = App->pathfinding->GetLastPath();
			PathMovement(path, enemy_map_pos, dt);
		}
	}

	if (is_moving_to_origin && enemy_map_pos != map_original_pos)
	{
		const p2DynArray<iPoint>* path;
		int c = App->pathfinding->CreatePath(enemy_map_pos, map_original_pos);
		if (c != -1)
		{
			path = App->pathfinding->GetLastPath();
			PathMovement(path, enemy_map_pos, dt);
		}
	}

	if (enemy_map_pos == map_original_pos)
	{
		animation = &idle;
	}
}

void Enemy_level01_ground::CreateRange()
{
	uint counter = 0;
	for (int i = -4; i < 5; i++)
		for (int k = -4; k < 5; k++)
		{
			iPoint tmp_range = App->map->WorldToMap((int)position.x, (int)position.y);
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

void Enemy_level01_ground::PathMovement(const p2DynArray<iPoint>* path, iPoint position, float dt)
{
	iPoint goal = iPoint(path->At(0)->x, path->At(0)->y); // we want the initial point of the path!
	float velocity_x = 0.0f;

	if (goal.x < position.x)
	{
		flip = SDL_FLIP_NONE;
		animation = &walk;
		velocity_x = -100.0f;
	}
	else if (goal.x > position.x)
	{
		flip = SDL_FLIP_HORIZONTAL;
		animation = &walk;
		velocity_x = 100.0f;
	}

	velocity.x = velocity_x;

	this->position.x += velocity.x * dt;
}

bool Enemy_level01_ground::PlayerIsInRange()
{
	bool ret = false;

	iPoint player_pos = App->map->WorldToMap((int)App->scene->player->position.x, (int)App->scene->player->position.y);

	for (int i = 0; i < RANGE_SIZE; i++)
	{
		if (range[i] == player_pos)
		{
			//LOG("PLAYER DETECTEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");
			//LOG("Objective [x]: %i [y]: %i", objective.x, objective.y);
			// Now we have the objective
			objective = range[i];
			if (objective == iPoint(App->map->WorldToMap((int)position.x, (int)position.y)))
				ret = false;
			else
				ret = true;
		}
	}

	return ret;
}

void Enemy_level01_ground::OnCollision(Collider* a, Collider* b)
{
	
}
