#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Audio.h"

Entity::Entity(int x, int y) : position(x, y)
{
	entity_death_sound = App->audio->LoadFx(".wav");
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites, float speed)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()), speed, 0.0f, flip);
}

void Entity::OnCollision(Collider* a, Collider* b)
{}