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

	fPoint position;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	// Initial position depending on the map
	fPoint first_map_pos = { 200.0f, 260.0f };
	fPoint second_map_pos = {300.0f, 300.0f };

private:
	Animation* current_animation = nullptr;
	Animation idle_anim;
	Animation walk_anim;

	Collider* player_collider = nullptr;
	iPoint collider_position;

	// Player movement
	float velocity_x = 2.0f;
	float velocity_y = 1.0f;

	bool going_left = false;
	bool going_right = false;
	bool going_up = false;
	bool going_down = true;

	//mrua equation X = X0 + v0*t + 1/2*a*t^2
	float gravity = 2.8f;// acceleration: a
	fPoint previous_position;//initial position of the jump or fall: X0
	float time = 0.0f;//t
	float initial_speed = 0.0f;//v0
	bool jump = false;// Jump input
	bool jump_start = false;//only true the moment you are on a platform and want to jump. Otherwise it wouldn't be possible to jump
	bool able_to_jump = false;// Is it possible to jump?
	short equation_is_possible = 0;//0 = equation is possible; 1 = equation not possible when going down; 2 = equation not possible when going up
};

#endif // __PLAYER_H__