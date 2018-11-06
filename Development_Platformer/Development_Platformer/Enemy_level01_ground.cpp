#include "j1App.h"
#include "Enemy_level01_ground.h"
#include "j1Collision.h"
#include "ModuleEnemies.h"

Enemy_level01_ground::Enemy_level01_ground(int x, int y) : Enemy(x, y)
{

	anim.PushBack({ 25, 5, 25, 37 });
	anim.loop = true;
	anim.speed = 0.5f;

	hit_points = 4;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, 25, 37 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);

	original_x = x;
	original_y = y;
}

void Enemy_level01_ground::Move()
{
	position.x = position.x;
	position.y = position.y;
}