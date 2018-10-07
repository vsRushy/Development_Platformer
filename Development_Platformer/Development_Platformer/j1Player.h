#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

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
	Animation*  current_animation = nullptr;
	Animation idle;

	// Player movement
	float velocity_x;
	float max_accel_x = 0.5f;

	bool going_left = false;
	bool going_right = false;
};

#endif // __PLAYER_H__