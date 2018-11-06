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
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Particles.h"

j1Player::j1Player()
{
	name.create("player");

	// Player idle animation
	idle_anim.PushBack({ 8, 42, 17, 22 });
	idle_anim.speed = 1.0f;
	idle_anim.loop = true;

	// Player walk animation
	walk_anim.PushBack({ 8, 42, 17, 22 });
	walk_anim.PushBack({ 40, 41, 17, 22 });
	walk_anim.PushBack({ 72, 42, 17, 22 });
	walk_anim.PushBack({ 104, 41, 17, 22 });
	walk_anim.speed = 0.1f;
	walk_anim.loop = true;
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& data)
{
	god_mode = data.child("god_mode").attribute("value").as_bool();
	gravity = data.child("gravity").attribute("value").as_float();
	velocity_x = data.child("velocity_x").attribute("value").as_float();
	velocity_y = data.child("velocity_x").attribute("value").as_float();

	return true;
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	graphics = App->tex->Load("textures/characters.png");

	LOG("Loading player sound effects");
	App->audio->LoadFx("jump.wav"); // id: 1
	App->audio->LoadFx("dash.wav"); // id: 2

	// Collider initial position
	collider_position.x = position.x;
	collider_position.y = position.y;

	// Set up the player's collider
	player_collider = App->collision->AddCollider({ collider_position.x, collider_position.y, PLAYER_SIZE_X, PLAYER_SIZE_Y }, COLLIDER_PLAYER, this);

	// Starting animation
	current_animation = &idle_anim;
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
	rect = &(current_animation->GetCurrentFrame());

	// Update collider position
	player_collider->SetPos(position.x, position.y);

	if (god_mode == false)
	{
		// Player Input (when pressing a key)
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			going_left = true;
			current_animation = &walk_anim;
			flip = SDL_FLIP_HORIZONTAL;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			going_right = true;
			current_animation = &walk_anim;
			flip = SDL_FLIP_NONE;
		}
		else
		{
			current_animation = &idle_anim;
		}

		/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			going_up = true;
		}*/

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && !dash && able_to_dash)
		{
			dash = true;
			able_to_dash = false;
			App->audio->PlayFx(2);
		}

		/*if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{

		}*/

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && !jump && able_to_jump)
		{
			App->audio->PlayFx(1);
			jump = true;
			jump_start = true;
			able_to_jump = false;
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
		/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			going_up = false;
		}*/

		/*if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			up_and_down = false;
		}*/

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			jump = false;
		}

		// sword throw
		if (Reset_time_bullets) {
			Bullet_time_init = SDL_GetTicks();
			Reset_time_bullets = false;
		}
		Bullet_delay = SDL_GetTicks() - Bullet_time_init;

		if ((App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) && Bullet_delay >= 100)
		{
			Bullet_delay = 0;
			Reset_time_bullets = true;
			App->particles->AddParticle(App->particles->sword, position.x, position.y, COLLIDER_PLAYER_SHOT);

		}

		// Movement logic
		if (going_left)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x - velocity_x - 1, position.y);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X, position.y + PLAYER_SIZE_Y - 1);

			if (!App->map->CheckCollisionX(worldPos.x, worldPos.y, worldFinalPos.y))
			{
				position.x -= velocity_x;
			}
		}
		if (going_right)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x, position.y);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X + velocity_x + 1, position.y + PLAYER_SIZE_Y - 1);

			if (!App->map->CheckCollisionX(worldFinalPos.x, worldPos.y, worldFinalPos.y))
			{
				position.x += velocity_x;
			}
		}
		if (going_up && !dash)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x, position.y + 1);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X - 1, position.y + PLAYER_SIZE_Y);

			if (!App->map->CheckCollisionY(worldPos.y, worldPos.x, worldFinalPos.x))
			{
				position.y -= velocity_y;
			}
		}
		if (up_and_down && !dash)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x, position.y + 1);
			// (x, y) point where the player is in the world in the next frame
			iPoint worldNextPos = App->map->WorldToMap(position.x, previous_position.y + initial_speed * (time + 0.1f) + (gravity*(time + 0.1f)*(time + 0.1f)) * 0.5f + 1);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X - velocity_x - 1, position.y + PLAYER_SIZE_Y);
			// (x + w, y + h) point where the player's ending coordinates are located in the world in the next frame
			iPoint worldNextFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X - velocity_x - 1, previous_position.y + initial_speed * (time + 0.1f) + (gravity*(time + 0.1f)*(time + 0.1f)) * 0.5f + PLAYER_SIZE_Y);

			//when colliding going up
			if (!App->map->CheckCollisionY(worldPos.y, worldPos.x, worldFinalPos.x) && App->map->CheckCollisionY(worldNextPos.y, worldNextPos.x, worldNextFinalPos.x))
			{
				equation_is_possible = 2;
			}
			else if (App->map->CheckCollisionY(worldPos.y, worldPos.x, worldFinalPos.x))
			{
				time = 0.0f;
				initial_speed = 2.0f;
				previous_position.y = position.y;
				equation_is_possible = 0;
				if (jump) jump = false;
				able_to_jump = false;
			}

			//when colliding going down
			if (!App->map->CheckCollisionY(worldFinalPos.y, worldPos.x, worldFinalPos.x) && !App->map->CheckCollisionY(worldNextFinalPos.y, worldNextPos.x, worldNextFinalPos.x))
			{
				if (!jump)time += 0.1f;
				able_to_jump = false;
			}
			else if (!App->map->CheckCollisionY(worldFinalPos.y, worldPos.x, worldFinalPos.x) && App->map->CheckCollisionY(worldNextFinalPos.y, worldNextPos.x, worldNextFinalPos.x))
			{
				equation_is_possible = 1;
			}
			else
			{
				time = 0.0f;
				previous_position.y = position.y;
				initial_speed = 0.0f;
				equation_is_possible = 0;
				if (!jump_start) {
					jump = false;
				}
				able_to_jump = true;
				able_to_dash = true;
			}
		}

		//jump
		if (jump && !dash)
		{
			initial_speed = -35.0f;
			time += 0.1f;
			if (jump_start) jump_start = false;
		}

		//dash
		if (!dash)
		{
			if (equation_is_possible == 0) position.y = previous_position.y + initial_speed * time + (gravity*time*time) * 0.5f;
			else if (equation_is_possible == 1) ++position.y;
			else --position.y;
		}
		else {
			if (dashTime < 1.5f)
			{
				dashTime += 0.1f;
				velocity_x = 10.0f;
			}
			else
			{
				dashTime = 0.0f;
				dash = false;
				velocity_x = 3.0f;
			}
		}
	}

	//god mode
	else if (god_mode == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			flip = SDL_FLIP_HORIZONTAL;
			current_animation = &walk_anim;
			position.x -= 3.0f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			flip = SDL_FLIP_NONE;
			current_animation = &walk_anim;
			position.x += 3.0f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			position.y -= 3.0f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			position.y += 3.0f;
		}
		else
			current_animation = &idle_anim;
	}

	return true;
}

bool j1Player::Load(pugi::xml_node& save)
{
	if (save.child("position") != NULL)
	{
		position.x = save.child("position").attribute("x").as_float();
		position.y = save.child("position").attribute("y").as_float();
	}

	return true;
}

bool j1Player::Save(pugi::xml_node& save) const
{
	if (save.child("position") == NULL)
	{
		save.append_child("position").append_attribute("x") = position.x;
		save.child("position").append_attribute("y") = position.y;
	}
	else {
		save.child("position").attribute("x") = position.x;
		save.child("position").attribute("y") = position.y;
	}

	return true;
}

void j1Player::OnCollision(Collider* a, Collider* b)
{

}