#include "j1App.h"
#include "Enemy_level01_air.h"
#include "j1Collision.h"
#include "ModuleEnemies.h"
#include "j1Map.h"

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
	position.x = position.x;
	position.y = position.y;
}

void Enemy_level01_air::Update(float dt)
{
	first_point = App->map->WorldToMap(position.x, position.y);
	last_point = App->map->WorldToMap(position.x + AIR_SIZE_01, position.y + AIR_SIZE_01);
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
				**** ****
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

void Enemy_level01_air::PathMovement(const p2DynArray<iPoint>* path, iPoint position, float speed)
{
	iPoint goal = iPoint(path->At(0)->x, path->At(0)->y); // we want the initial point of the path!
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;

	if(goal.x < position.x)
	{ 
		velocity_x = -5.0f;
	}
	else if (goal.x > position.x)
	{
		velocity_x = 5.0f;
	}
	else if (goal.y < position.y)
	{
		velocity_y = -5.0f;
	}
	else if (goal.y > position.y)
	{
		velocity_y = 5.0f;
	}

	is_moving = true;
	velocity.x = velocity_x;
	velocity.y = velocity_y;
}