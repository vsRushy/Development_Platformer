#include "j1App.h"
#include "Coins.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"


Coins::Coins(int x, int y) : Entity(type, x, y)
{
	spin.PushBack({ 0, 0, 34, 33 });
	spin.PushBack({ 34, 0, 34, 33 });
	spin.PushBack({ 68, 0, 34, 33 });
	spin.PushBack({ 102, 0, 34, 33 });
	spin.PushBack({ 136, 0, 34, 33 });
	spin.PushBack({ 170, 0, 34, 33 });
	spin.speed = 0.3f;
	spin.loop = true;

	animation = &spin;

	type = ENTITY_TYPES::COINS;

	collider = App->collision->AddCollider({ 0, 0, 34, 33 }, COLLIDER_TYPE::COLLIDER_COIN, (j1Module*)App->entities);

}

void Coins::Update(float dt)
{

}

void Coins::OnCollision(Collider* a, Collider* b)
{

}