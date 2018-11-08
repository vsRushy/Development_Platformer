#include "j1App.h"
#include "Enemy_level01_air.h"
#include "j1Collision.h"
#include "ModuleEnemies.h"

Enemy_level01_air::Enemy_level01_air(int x, int y) : Enemy(x, y)
{

	anim.PushBack({ 64, 82, 32, 32 });
	anim.loop = true;
	anim.speed = 0.5f;

	hit_points = 4;

	animation = &anim;

	collider = App->collision->AddCollider({ 0, 0, 32, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);

	original_x = x;
	original_y = y;
}

void Enemy_level01_air::Move()
{
	position.x = position.x;
	position.y = position.y;
}