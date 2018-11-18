#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "Player.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Particles.h"

Player::Player(int x, int y) : Entity(type, x, y)
{
	// Player idle animation
	idle_anim.PushBack({ 8, 42, 17, 22 });
	idle_anim.speed = 10.0f;
	idle_anim.loop = true;

	// Player walk animation
	walk_anim.PushBack({ 8, 42, 17, 22 });
	walk_anim.PushBack({ 40, 41, 17, 22 });
	walk_anim.PushBack({ 72, 42, 17, 22 });
	walk_anim.PushBack({ 104, 41, 17, 22 });
	walk_anim.speed = 1.0f;
	walk_anim.loop = true;

	hit_points = 1;

	god_mode = false;
	gravity = 2.0f;
	velocity_x = 150.0f;
	velocity_y = 2.0f;

	animation = &idle_anim;
	rect = &(animation->GetCurrentFrame());

	type = ENTITY_TYPES::PLAYER;

	collider = App->collision->AddCollider({ 0, 0, PLAYER_SIZE_X, PLAYER_SIZE_Y }, COLLIDER_TYPE::COLLIDER_PLAYER, (j1Module*)App->entities);

	original_pos.x = x;
	original_pos.y = y;
}

/* Here we define the player's logic. It is blitted to the screen in j1Scene.cpp. Although we can do it
   here, it has more sense to blit the player in the scene, because the player IS in the scene */
void Player::Update(float dt)
{
	float anim_speed = 7.0f;

	idle_anim.speed = anim_speed * dt;
	walk_anim.speed = anim_speed * dt;

	rect = &(animation->GetCurrentFrame());

	if (god_mode == false)
	{
		// Player Input (when pressing a key)
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			going_left = true;
			animation = &walk_anim;
			flip = SDL_FLIP_HORIZONTAL;
			if (player_facing)player_facing = false;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			going_right = true;
			animation = &walk_anim;
			flip = SDL_FLIP_NONE;
			if (!player_facing) player_facing = true;
		}
		else
		{
			animation = &idle_anim;
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
		
		if ((App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) && Bullet_delay >= 2000)
		{
			Bullet_delay = 0;
			Reset_time_bullets = true;
			App->audio->PlayFx(3);
			if(player_facing) App->particles->AddParticle(App->particles->sword_right, position.x, position.y, COLLIDER_PLAYER_SHOT);
			else  App->particles->AddParticle(App->particles->sword_left, position.x, position.y, COLLIDER_PLAYER_SHOT);
		}


		// Movement logic
		if (going_left)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x - velocity_x*dt - 1, position.y);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X, position.y + PLAYER_SIZE_Y - 1);

			if (!App->map->CheckCollisionX(worldPos.x, worldPos.y, worldFinalPos.y))
			{
				position.x -= velocity_x*dt;
			}
		}
		if (going_right)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x, position.y);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X + velocity_x*dt + 1, position.y + PLAYER_SIZE_Y - 1);

			if (!App->map->CheckCollisionX(worldFinalPos.x, worldPos.y, worldFinalPos.y))
			{
				position.x += velocity_x*dt;
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
				position.y -= velocity_y*dt;
			}
		}
		if (up_and_down && !dash)
		{
			// (x, y) point where the player is in the world
			iPoint worldPos = App->map->WorldToMap(position.x, position.y + 1);
			// (x, y) point where the player is in the world in the next frame
			iPoint worldNextPos = App->map->WorldToMap(position.x, previous_position.y + (initial_speed * (time + 8.0f*dt) + (gravity*(time + 8.0f*dt)*(time + 8.0f*dt)) * 0.5f) + 1);
			// (x + w, y + h) point where the player's ending coordinates are located in the world
			iPoint worldFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X - velocity_x*dt - 1, position.y + PLAYER_SIZE_Y);
			// (x + w, y + h) point where the player's ending coordinates are located in the world in the next frame
			iPoint worldNextFinalPos = App->map->WorldToMap(position.x + PLAYER_SIZE_X - velocity_x*dt - 1, previous_position.y + (initial_speed * (time + 8.0f*dt) + (gravity*(time + 8.0f*dt)*(time + 8.0f*dt)) * 0.5f) + PLAYER_SIZE_Y);

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
				if (!jump)time += 8.0f*dt;
				able_to_jump = false;
			}
			else if (!App->map->CheckCollisionY(worldFinalPos.y, worldPos.x, worldFinalPos.x) && App->map->CheckCollisionY(worldNextFinalPos.y, worldNextPos.x, worldNextFinalPos.x))
			{
				equation_is_possible = 1;
			}
			else if(App->map->CheckCollisionY(worldFinalPos.y, worldPos.x, worldFinalPos.x) || App->map->CheckCollisionY(worldNextFinalPos.y, worldNextPos.x, worldNextFinalPos.x))
			{
				Reset();
			}
		}

		//jump
		if (jump && !dash)
		{
			initial_speed = -20.0f;
			time += 8.0f*dt;
			if (jump_start) jump_start = false;
		}

		//dash
		if (!dash)
		{
			if (equation_is_possible == 0)
			{
				position.y = previous_position.y + (initial_speed * time + (gravity*time*time) * 0.5f);
			}
			else if (equation_is_possible == 1) ++position.y;
			else --position.y;
		}
		else {
			if (dashTime < 0.2f)
			{
				dashTime += 0.8f*dt;
				velocity_x = 500.0f;
			}
			else
			{
				dashTime = 0.0f;
				dash = false;
				velocity_x = 150.0f;
			}
		}
	}

	//god mode
	else if (god_mode == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			flip = SDL_FLIP_HORIZONTAL;
			animation = &walk_anim;
			position.x -= 125.0f * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			flip = SDL_FLIP_NONE;
			animation = &walk_anim;
			position.x += 125.0f * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			position.y -= 125.0f * dt;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			position.y += 125.0f * dt;
		}
		else
			animation = &idle_anim;
	}
}

void Player::OnCollision(Collider* a, Collider* b)
{
	
}

void Player::Reset() {
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
