#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Audio.h"


Enemy::Enemy(int x, int y) : position(x, y)
{
	enemy_death_sound = App->audio->LoadFx(".wav");
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites, float speed)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()), speed);
}

void Enemy::OnCollision(Collider* collider, int type)
{
	// ...
}