#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

#include "SDL/include/SDL_render.h"

#define PLAYER_SIZE_X 16
#define PLAYER_SIZE_Y 22

struct Collider;
struct SDL_Texture;

// TODO: enumeration to know the player states.

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	// Load
	bool Load(pugi::xml_node&);
	// Save
	bool Save(pugi::xml_node&) const;
	
	void OnCollision(Collider*, Collider*);

public:
	SDL_Texture* graphics = nullptr;

	/* This pointer to SDL_Rect needs to be passed to the j1Scene.cpp in order
	to render the player correctly. It stores the player's animation rect */
	SDL_Rect* rect = nullptr; 

	fPoint position = { 0, 0 };
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	fPoint previous_position = { 0, 0 }; //initial position of the jump or fall: X0

	// Initial position depending on the map
	fPoint first_map_pos;
	fPoint second_map_pos;

	bool god_mode;

	bool time_finished = true;
	uint Bullet_time_init = 0;
	uint Bullet_delay;
	bool Reset_time_bullets = true;

private:
	Animation* current_animation = nullptr;
	Animation idle_anim;
	Animation walk_anim;

	Collider* player_collider = nullptr;
	iPoint collider_position;

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
};

#endif // __PLAYER_H__