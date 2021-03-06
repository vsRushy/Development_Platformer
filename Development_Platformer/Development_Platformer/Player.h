#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"
#include "p2Point.h"

#include "SDL/include/SDL_render.h"

#define PLAYER_SIZE_X 16
#define PLAYER_SIZE_Y 22

struct Collider;
struct SDL_Texture;

class Player : public Entity
{
public:
	Player(int x, int y);
	~Player() {};

	void Update(float dt);
	
	void OnCollision(Collider*, Collider*);

public:
	/* This pointer to SDL_Rect needs to be passed to the j1Scene.cpp in order
	to render the player correctly. It stores the player's animation rect */
	SDL_Rect* rect = nullptr; 

	fPoint previous_position = { 0, 0 }; //initial position of the jump or fall: X0

	bool god_mode;

	bool time_finished = true;
	uint Bullet_time_init = 0;
	uint Bullet_delay;
	bool Reset_time_bullets = true;
	bool player_facing = true; //true = right; false = left;
	void Reset();
	bool player_start = true;
	uint number_of_coins = 0;
	p2SString num_coins;

private:
	Animation idle_anim;
	Animation walk_anim;

	iPoint original_pos;

	// Player movement
	float velocity_x;
	float velocity_y;

	bool going_left = false;
	bool going_right = false;
	bool going_up = false;
	bool up_and_down = true;

	//mrua equation X = X0 + v0*t + 1/2*a*t^2
	float gravity;// acceleration: a
	
	float time = 0.0f;//t
	float initial_speed = 0.0f;//v0
	bool jump = false;// Jump input
	bool jump_start = false;//only true the moment you are on a platform and want to jump. Otherwise it wouldn't be possible to jump
	bool able_to_jump = false;// Is it possible to jump?
	short equation_is_possible = 0;//0 = equation is possible; 1 = equation not possible when going down; 2 = equation not possible when going up

	//dash
	bool dash = false;
	float dashTime = 0.0f;
	bool able_to_dash = false;
	float wait_time = 0.0f;

};

#endif // __PLAYER_H__