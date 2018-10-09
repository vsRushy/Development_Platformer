#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

#define PLAYER_COLLIDER_SIZE_X 15
#define PLAYER_COLLIDER_SIZE_Y 22

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

private:
	Collider* col = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;

	Collider* player_collider = nullptr;
	iPoint collider_position;

	// Player movement
	float velocity_x = 1.0f;
	float velocity_y = 0.01f;

	bool going_left = false;
	bool going_right = false;
	bool going_up = false;
	bool going_down = true;

	//mrua equation
	float gravity = 2.8f;
	fPoint previous_position;
	float time = 0.0f;
	float initial_speed = 0.0f;
};

#endif // __PLAYER_H__