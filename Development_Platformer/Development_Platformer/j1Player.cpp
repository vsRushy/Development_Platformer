#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1Map.h"

j1Player::j1Player()
{
	name.create("player");

	// Player idle animation
	idle.PushBack({ 9, 42, 15, 22 });
	idle.speed = 1.0f;
	idle.loop = true;
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& data)
{
	return true;
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	graphics = App->tex->Load("textures/characters.png");

	LOG("Loading player sound effects");

	position.x = position.y = 10;

	// Collider initial position
	collider_position.x = position.x;
	collider_position.y = position.y;

	// Set up the player's collider
	player_collider = App->collision->AddCollider({ collider_position.x, collider_position.y, PLAYER_COLLIDER_SIZE_X, PLAYER_COLLIDER_SIZE_Y }, COLLIDER_PLAYER, this);

	// Starting animation
	current_animation = &idle;
	rect = &(current_animation->GetCurrentFrame());

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");
	App->tex->UnLoad(graphics);

	// Delete collider
	if (player_collider != nullptr)
		player_collider->to_delete = true;

	return true;
}

/* Here we define the player's logic. It is blitted to the screen in j1Scene.cpp. Although we can do it
   here, it has more sense to blit the player in the scene, because the player IS in the scene */
bool j1Player::Update(float dt)
{
	current_animation = &idle;
	rect = &(current_animation->GetCurrentFrame());

	// Update collider position
	player_collider->SetPos(position.x, position.y);

	// Player Input (when pressing a key)
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		going_left = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		going_right = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		going_up = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		going_down = true;
	}

	// Player input (when releasing a key)
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		going_left = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		going_right = false;
	}
	// Player input (when releasing a key)
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
	{
		going_up = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		going_down = false;
	}

	// Movement logic
	if (going_left)
	{
		// (x, y) point where the player is in the world
		iPoint worldPos = App->map->WorldToMap(position.x - 1, position.y);
		// (x + w, y + h) point where the player's ending coordinates are located in the world
		iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_COLLIDER_SIZE_X, position.y + PLAYER_COLLIDER_SIZE_Y - 1);

		
		if (!App->map->CheckCollisionX(worldPos.x, worldPos.y, worldFinalPos.y))
		{
			position.x -= velocity_x;
		}
	}
	if (going_right)
	{
		// (x, y) point where the player is in the world
		iPoint worldPos = App->map->WorldToMap(position.x + 1, position.y);
		// (x + w, y + h) point where the player's ending coordinates are located in the world
		iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_COLLIDER_SIZE_X, position.y + PLAYER_COLLIDER_SIZE_Y - 1);

		
		if (!App->map->CheckCollisionX(worldFinalPos.x, worldPos.y, worldFinalPos.y))
		{
			position.x += velocity_x;
		}
	}
	if (going_up)
	{
		// (x, y) point where the player is in the world
		iPoint worldPos = App->map->WorldToMap(position.x, position.y - 1);
		// (x + w, y + h) point where the player's ending coordinates are located in the world
		iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_COLLIDER_SIZE_X - 1, position.y + PLAYER_COLLIDER_SIZE_Y);

		
		if (!App->map->CheckCollisionY(worldPos.y, worldPos.x, worldFinalPos.x))
		{
			position.y -= velocity_y;
		}
	}
	if (going_down)
	{
		// (x, y) point where the player is in the world
		iPoint worldPos = App->map->WorldToMap(position.x, position.y + 1);
		// (x + w, y + h) point where the player's ending coordinates are located in the world
		iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_COLLIDER_SIZE_X - 1, position.y + PLAYER_COLLIDER_SIZE_Y);


		if (!App->map->CheckCollisionY(worldFinalPos.y, worldPos.x, worldFinalPos.x))
		{
			position.y += velocity_y;
		}
	}

	/*
	// Update player position
	position.x += velocity_x;
	*/
	return true;
}

bool j1Player::Load(pugi::xml_node& data)
{
	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	return true;
}

void j1Player::OnCollision(Collider* a, Collider* b)
{

}