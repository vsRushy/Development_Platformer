#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

class Player;
class Enemy_level01_air;
class Enemy_level01_ground;

class GUIImage;
class GUIButton;

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load
	bool Load(pugi::xml_node&);
	// Save
	bool Save(pugi::xml_node&) const;

private:
	p2SString first_map;
	p2SString second_map;

	p2SString first_song;
	p2SString second_song;

	SDL_Texture* debug_tex = nullptr;

public:
	// Level 1: 1, Level 2: 2.
	int map_selected;
	int last_map_selected = 1;

	fPoint first_map_pos;
	fPoint second_map_pos;

	bool actual_god_mode = false;

public:
	Player* player;
	Enemy_level01_air* enemy01air = nullptr;
	Enemy_level01_ground* enemy01ground = nullptr;
	Enemy_level01_air* enemy02air = nullptr;
	Enemy_level01_ground* enemy02ground = nullptr;

// GUI
public:
	GUIImage* image_gui = nullptr;
	GUIButton* button_gui = nullptr;
};

#endif // __j1SCENE_H__