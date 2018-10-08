#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "j1Collision.h"

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
		position.y -= 0.5f;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && !block_y)
	{
		position.y += 0.5f;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && time_2 == 0)
	{
		jump = true;
		not_jumping = false;
	}

	// Player input (when releasing a key)
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		velocity_x = 0.0f;
		going_left = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		velocity_x = 0.0f;
		going_right = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		jump = false;
	}

	// Movement logic
	if (going_left)
	{
		velocity_x -= 0.1f;
		App->render->camera.x += 1;
		if (velocity_x < -max_accel_x)
			velocity_x = -max_accel_x;
	}
	else if (going_right)
	{
		App->render->camera.x -= 1;
		velocity_x += 0.1f;
		if (velocity_x > max_accel_x)
			velocity_x = max_accel_x;
	}

	if (jump && time == 0.0f) {		
		position_y_aux = position.y;
	}
	if (!not_jumping) {
		position.y = position_y_aux + initial_speed * time + (gravity*time*time) / 2;
		time += 0.1f;
	}
	if (!block_y) position.y += gravity;

	if (block_y && (position.x + PLAYER_COLLIDER_SIZE_X < coll_rect.x || position.x > coll_rect.x + coll_rect.w)) block_y = false;

	// Update player position
	previous_position = position;
	position.x += velocity_x;

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
	if (position.y + PLAYER_COLLIDER_SIZE_Y > b->rect.y && previous_position.y <= position.y) block_y = true;
	position.y = b->rect.y - PLAYER_COLLIDER_SIZE_Y;
	coll_rect = b->rect;
}