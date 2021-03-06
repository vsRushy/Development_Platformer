#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

class Player;
class Enemy_level01_air;
class Enemy_level01_ground;
class Coins;

class GUIImage;
class GUIButton;
class GUILabel;
class GUIPanel;
class GUIInputBox;
class GUIThumb;
class GUISlider;

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

	bool start_game = false;

private:
	void CreateStartingGUI();
	void CreateLevel01Entities();
	void CreateLevel02Entities();
	void CreateLevel01Coins();
	void CreateLevel02Coins();
	void CreatePlayerGUI();
	void DeleteStartingGUI();
	void PauseMenuControl();
	void CheckVolumeInput();
	void PauseMenuWorkflow();

public:
	Player* player;
	Enemy_level01_air* enemy01air = nullptr;
	Enemy_level01_ground* enemy01ground = nullptr;
	Enemy_level01_air* enemy02air = nullptr;
	Enemy_level01_ground* enemy02ground = nullptr;
	Coins* coins[7];

private:
	SDL_Texture* bg = nullptr;
	SDL_Texture* coins_image = nullptr;

// GUI
public:
	GUIImage* logo_gui = nullptr;
	GUIImage* game_name_logo_gui = nullptr;
	GUIButton* credits_button_gui = nullptr;
	GUIButton* start_button_gui = nullptr;
	GUIButton* options_button_gui = nullptr;
	GUIButton* quit_button_gui = nullptr;
	GUIButton* volume_up_button_gui = nullptr;
	GUIButton* volume_down_button_gui = nullptr;
	GUISlider* start_volume_slider_gui = nullptr;
	GUILabel* introduce_name_label_gui = nullptr;
	GUIInputBox* inputbox_gui = nullptr;
	GUILabel* credits_label_license = nullptr;
	GUILabel* credits_label_names = nullptr;

	GUILabel* player_nickname_label = nullptr;
	GUIImage* player_line_gui = nullptr;

	GUILabel* player_coins = nullptr;

	// Pause GUI
	GUIButton* continue_button_gui = nullptr;
	GUIButton* pause_quit_button_gui = nullptr;
	GUIButton* pause_options_button_gui = nullptr;
	GUIButton* pause_volume_up_button_gui = nullptr;
	GUIButton* pause_volume_down_button_gui = nullptr;
	GUISlider* pause_volume_slider_gui = nullptr;

	p2SString player_name;
};

#endif // __j1SCENE_H__