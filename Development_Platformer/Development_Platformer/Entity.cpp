#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "j1Render.h"

Entity::Entity(ENTITY_TYPES type, int x, int y) : type(type), position(x, y)
{
	
}

Entity::~Entity()
{
	if (collider != nullptr)
	{
		App->collision->EraseCollider(collider);
		collider = nullptr;
	}
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
