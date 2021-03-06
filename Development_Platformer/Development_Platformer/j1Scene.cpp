#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "j1EntityManager.h"
#include "Player.h"
#include "Enemy_level01_air.h"
#include "Enemy_level01_ground.h"
#include "j1Pathfinding.h"
#include "j1GUIManager.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "GUIPanel.h"
#include "GUIInputBox.h"
#include "GUIThumb.h"
#include "GUISlider.h"

#include "Brofiler\Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	map_selected = config.child("map_index").attribute("value").as_int();
	
	first_map = config.child("first_map").attribute("name").as_string();
	second_map = config.child("second_map").attribute("name").as_string();

	first_song = config.child("first_song").attribute("name").as_string();
	second_song = config.child("second_song").attribute("name").as_string();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (!start_game)
	{
		App->audio->PlayMusic(first_song.GetString());
		bg = App->tex->Load("textures/background_start.png");
		CreateStartingGUI();
	}

	if (start_game)
	{
		coins_image = App->tex->Load("textures/coins_interface.png");
		if (map_selected == 1)
		{
			App->map->Load(first_map.GetString());
			App->audio->PlayMusic(first_song.GetString());

			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);

			first_map_pos = App->map->data.ObjectPos("Player", "PlayerStartPos");

			CreateLevel01Entities();
			
			player->number_of_coins = 0;

			CreateLevel01Coins();

			player->position = first_map_pos;
			player->previous_position = player->position;
		}
		else if (map_selected == 2)
		{
			App->map->Load(second_map.GetString());
			App->audio->PlayMusic(second_song.GetString());

			second_map_pos = App->map->data.ObjectPos("Player", "PlayerStartPos");

			CreateLevel02Entities();

			player->position = second_map_pos;
			player->previous_position = player->position;

			CreateLevel02Coins();
		}

		CreatePlayerGUI();

		if(player_coins == nullptr)
			player_coins = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 74.0f, 9.0f, "0", { 0, 0, 0, 255 }, App->gui->default_font_used);

		debug_tex = App->tex->Load("textures/path_tile.png");
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::CadetBlue);

	bool ret = true;

	// In game gui
	if (player_coins != nullptr && player != nullptr)
	{
		player_coins->SetText((p2SString)player->num_coins.GetString());
	}

	// UI Check
	if (start_button_gui != nullptr && start_button_gui->has_been_clicked == !start_game)
	{
		start_game = true;
		// Delete starting active GUI
		DeleteStartingGUI();

		App->fade->FadeToBlack(this, this, 1.0f);
	}

	if (player != nullptr)
	{
		/*INPUT--------------------------------------------*/
		if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			App->LoadGame();
		}
		if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			App->SaveGame();
		}
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			LOG("HEYYY!!!!!!!! Player position is: x = %f, y = %f", player->position.x, player->position.y);
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			App->render->camera.y -= 450 * dt;
		else
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				App->render->camera.y += 450 * dt;
			else
				if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
					App->render->camera.x -= 450 * dt;
				else
					if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
						App->render->camera.x += 450 * dt;
					else
					{
						App->render->camera.x = (int)(player->position.x - 242) * (-1) * App->win->GetScale();
						App->render->camera.y = (int)(player->position.y - 200) * (-1) * App->win->GetScale();
					}


		// Start from the first level
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			if (map_selected == 1)
			{
				App->entities->DeleteAllEntities();
			}
			else if (map_selected == 2)
			{
				map_selected = 1;
				App->entities->DeleteAllEntities();
			}
			player->player_start = true;
			App->gui->DeleteGUIElement(player_coins);
			player_coins = nullptr;
			App->fade->FadeToBlack(this, this, 0.5f);
		}

		// Start from the current level
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			if (map_selected == 1)
			{
				App->entities->DeleteAllEntities();
				player->position = first_map_pos;
				player->previous_position = player->position;
			}
			else if (map_selected == 2)
			{
				App->entities->DeleteAllEntities();
				player->position = second_map_pos;
				player->previous_position = player->position;
			}
			player->player_start = true;
			App->gui->DeleteGUIElement(player_coins);
			player_coins = nullptr;
			App->fade->FadeToBlack(this, this, 0.5f);
		}

		// Set the window title like
		// "Map:%dx%d Tiles:%dx%d Tilesets:%d"                            // Uncomment the following if you want to see tileset info as window title
		/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count());

		App->win->SetTitle(title.GetString());*/

		/* Will the player reach the end position to go to level 2? */
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || App->map->data.IsObjectTrigger("Player", "PlayerEndPos", player->position) && !App->fade->IsFading())
		{
			if (map_selected == 1)
				map_selected = 2;
			else if (map_selected == 2)
				map_selected = 1;

			App->entities->DeleteAllEntities();
			player->player_start = true;
			App->gui->DeleteGUIElement(player_coins);
			player_coins = nullptr;
			App->fade->FadeToBlack(this, this);
		}

		/* ENVABLE/DISABLE GOD MODE */
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			actual_god_mode = !actual_god_mode;
			player->god_mode = !player->god_mode;
		}

		/* Check if player falls into the death zone */
		if (App->map->data.IsObjectTrigger("DeathZone", "DeathZone_1", player->position) ||
			App->map->data.IsObjectTrigger("DeathZone", "DeathZone_2", player->position) ||
			App->map->data.IsObjectTrigger("DeathZone", "DeathZone_3", player->position) ||
			App->map->data.IsObjectTrigger("DeathZone", "DeathZone_4", player->position) ||
			App->map->data.IsObjectTrigger("DeathZone", "DeathZone_5", player->position) ||
			App->map->data.IsObjectTrigger("DeathZone", "DeathZone_6", player->position) ||
			App->map->data.IsObjectTrigger("DeathZone", "DeathZone_7", player->position))
		{

			if (map_selected == 1)
			{
				player->position = first_map_pos;
				player->previous_position = player->position;
			}

			else if (map_selected == 2)
			{
				player->position = second_map_pos;
				player->previous_position = player->position;
			}

			player->Reset();
		}

		/* DRAW------------ */
		// App->render->Blit(img, 0, 0);
		App->map->Draw();


		// Debug pathfinding ------------------------------
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);

		if (player->god_mode)
			App->render->Blit(debug_tex, p.x, p.y);
	}

	// Blit starting background
	if (!start_game)
	{
		App->render->Blit(bg, 0, 0, NULL);
	}
	else App->render->Blit(coins_image, 10, 5, NULL, 0.0F);

	// Go to web
	if (logo_gui != nullptr && logo_gui->has_been_clicked)
	{
		ShellExecuteA(NULL, "open", "https://vsrushy.github.io/Development_Platformer/", NULL, NULL, SW_SHOWNORMAL);
	}

	// Volume with keys +/-
	CheckVolumeInput();
	
	// Check inputs pause menu
	PauseMenuControl();

	if (pause_quit_button_gui != nullptr && pause_quit_button_gui->has_been_clicked)
	{
		ret = false;
	}

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (quit_button_gui != nullptr && quit_button_gui->has_been_clicked)
		ret = false;

	PauseMenuWorkflow();
	
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->Unload();
	App->tex->UnLoad(debug_tex);
	App->tex->UnLoad(bg);
	App->tex->UnLoad(coins_image);

	return true;
}

bool j1Scene::Load(pugi::xml_node& save)
{
	/*if (save.child("map_selected") != NULL)
	{
		// We want to load the map when we are not in the same map_selected index
		if(save.child("map_selected").attribute("value").as_int() != map_selected)
		{
			map_selected = save.child("map_selected").attribute("value").as_int();
			//App->fade->FadeToBlack(this, this, 2.0f);
		}
	}
	isLoading = true;*/

	return true;
}

bool j1Scene::Save(pugi::xml_node& save) const
{
	/*if (save.child("map_selected") == NULL)
	{
		save.append_child("map_selected").append_attribute("value") = map_selected;
	}
	else
	{
		save.append_child("map_selected").attribute("value") = map_selected;
	}*/

	return true;
}

void j1Scene::CreateStartingGUI()
{
	start_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 4.0f, 4.0f, { 0, 0, 47, 24 }, { 0, 24, 47, 24 }, { 0, 48, 47, 24 });
	options_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 4.0f, 32.0f, { 141, 0, 47, 24 }, { 141, 24, 47, 24 }, { 141, 48, 47, 24 });
	quit_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 4.0f, 60.0f, { 47, 0, 47, 24 }, { 47, 24, 47, 24 }, { 47, 48, 47, 24 });
	introduce_name_label_gui = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 150.0f, 220.0f, "Name:", { 0, 0, 0, 255 }, App->gui->default_font_used);
	logo_gui = (GUIImage*)App->gui->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 424.0f, 358.0f, { 0, 0, 84, 22 });
	game_name_logo_gui = (GUIImage*)App->gui->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 150.0f, 120.0f, { 0, 22, 264, 90 });
	inputbox_gui = (GUIInputBox*)App->gui->CreateGUIInputBox(GUI_ELEMENT_TYPE::GUI_INPUTBOX, 150.0f, 235.0f, { 0, 0, 0, 255 }, App->gui->default_font_used, { 0, 0, 100, 24 });
	credits_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 4.0f, 88.0f, { 232, 0, 47, 24 }, { 232, 24, 47, 24 }, { 232, 48, 47, 24 });
}

void j1Scene::CreateLevel01Coins()
{
	coins[0] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 450, 400);
	coins[1] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1119, 1053);
	coins[2] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 2238, 1213);
	coins[3] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1718, 776);
	coins[4] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1439, 670);
	coins[5] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 670, 766);
	coins[6] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 640, 396);
}

void j1Scene::CreateLevel02Coins()
{
	coins[0] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1088, 1280);
	coins[1] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1282, 1190);
	coins[2] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 2901, 1077);
	coins[3] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1536, 768);
	coins[4] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 1025, 350);
	coins[5] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 673, 223);
	coins[6] = (Coins*)App->entities->CreateEntity(ENTITY_TYPES::COINS, 2240, 416);
}

void j1Scene::CreatePlayerGUI()
{
	if (player_nickname_label == nullptr)
		player_nickname_label = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, player->position.x - PLAYER_SIZE_X + 17, player->position.y - PLAYER_SIZE_Y * 2,
			player_name.GetString(), { 0, 0, 0, 255 }, App->gui->default_font_used);

	if (player_name.Length() > 0)
	{
		if (player_line_gui == nullptr)
		{
			player_line_gui = (GUIImage*)App->gui->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 260.0f, 190.0f, { 85, 0, 26, 10 });
		}
	}
}

void j1Scene::CreateLevel01Entities()
{
	player = (Player*)App->entities->CreateEntity(ENTITY_TYPES::PLAYER, first_map_pos.x, first_map_pos.y);
	enemy01air = (Enemy_level01_air*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_AIR, 470, 200);
	enemy01ground = (Enemy_level01_ground*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_GROUND, 960, 779);
}

void j1Scene::CreateLevel02Entities()
{
	player = (Player*)App->entities->CreateEntity(ENTITY_TYPES::PLAYER, second_map_pos.x, second_map_pos.y);
	enemy02air = (Enemy_level01_air*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_AIR, 2274, 992);
	enemy02ground = (Enemy_level01_ground*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_GROUND, 1280, 1131);
}

void j1Scene::DeleteStartingGUI()
{
	if (start_button_gui != nullptr)
	{
		App->gui->DeleteGUIElement(start_button_gui);
		start_button_gui = nullptr;
	}
	if (options_button_gui != nullptr)
	{
		App->gui->DeleteGUIElement(options_button_gui);
		options_button_gui = nullptr;
	}
	if (quit_button_gui != nullptr)
	{
		App->gui->DeleteGUIElement(quit_button_gui);
		quit_button_gui = nullptr;
	}
	if (introduce_name_label_gui != nullptr)
	{
		App->gui->DeleteGUIElement(introduce_name_label_gui);
		introduce_name_label_gui = nullptr;
	}
	if (logo_gui != nullptr)
	{
		App->gui->DeleteGUIElement(logo_gui);
		logo_gui = nullptr;
	}
	if (game_name_logo_gui != nullptr)
	{
		App->gui->DeleteGUIElement(game_name_logo_gui);
		game_name_logo_gui = nullptr;
	}
	if (volume_up_button_gui != nullptr)
	{
		App->gui->DeleteGUIElement(volume_up_button_gui);
		volume_up_button_gui = nullptr;
	}
	if (volume_down_button_gui != nullptr)
	{
		App->gui->DeleteGUIElement(volume_down_button_gui);
		volume_down_button_gui = nullptr;
	}
	if (inputbox_gui != nullptr)
	{
		player_name = inputbox_gui->input_box_label->text.GetString();
		App->gui->DeleteGUIElement(inputbox_gui);
		inputbox_gui = nullptr;
	}
	if (credits_button_gui != nullptr)
	{
		App->gui->DeleteGUIElement(credits_button_gui);
		credits_button_gui = nullptr;
	}
	if (start_volume_slider_gui != nullptr)
	{
		App->gui->DeleteGUIElement(start_volume_slider_gui);
		start_volume_slider_gui = nullptr;
	}
	if (credits_label_license != nullptr)
	{
		App->gui->DeleteGUIElement(credits_label_license);
		credits_label_license = nullptr;
	}
	if (credits_label_names != nullptr)
	{
		App->gui->DeleteGUIElement(credits_label_names);
		credits_label_names = nullptr;
	}
}

void j1Scene::PauseMenuControl()
{
	if (continue_button_gui != nullptr && continue_button_gui->has_been_clicked)
	{
		if (App->game_pause)
		{
			App->game_pause = false;
			App->gui->DeleteGUIElement(continue_button_gui);
			App->gui->DeleteGUIElement(pause_quit_button_gui);
			App->gui->DeleteGUIElement(pause_options_button_gui);
			pause_quit_button_gui = nullptr;
			continue_button_gui = nullptr;
			pause_options_button_gui = nullptr;
		}
	}
	if (options_button_gui != nullptr && options_button_gui->has_been_clicked)
	{
		if (volume_up_button_gui == nullptr && volume_down_button_gui == nullptr)
		{
			volume_up_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 52.0f, 32.0f, { 188, 0, 22, 24 }, { 188, 24, 22, 24 }, { 188, 48, 22, 24 });
			volume_down_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 75.0f, 32.0f, { 210, 0, 22, 24 }, { 210, 24, 22, 24 }, { 210, 48, 22, 24 });
			start_volume_slider_gui = (GUISlider*)App->gui->CreateGUISlider(GUI_ELEMENT_TYPE::GUI_SLIDER, 100.0f, 20.0f, { 0, 0, 9, 51 }, { 0, 0, 7, 7 }, { 0, 7, 7, 7 }, { 0, 14, 7, 7 });
		}
	}
	else if (options_button_gui != nullptr && !(options_button_gui->has_been_clicked))
	{
		App->gui->DeleteGUIElement(volume_up_button_gui);
		App->gui->DeleteGUIElement(volume_down_button_gui);
		App->gui->DeleteGUIElement(start_volume_slider_gui);
		volume_up_button_gui = nullptr;
		volume_down_button_gui = nullptr;
		start_volume_slider_gui = nullptr;
	}
	if (credits_button_gui != nullptr && credits_button_gui->has_been_clicked)
	{
		if (credits_label_license == nullptr && credits_label_names == nullptr)
		{
			credits_label_license = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 10.0f, 310.0f, "MIT LICENSE.", { 0, 0, 0, 255 }, App->gui->default_font_used);
			credits_label_names = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 10.0f, 350.0f, "Copyright Gerard M. & Mart� T.", { 0, 0, 0, 255 }, App->gui->default_font_used);
		}
	}
	else if (credits_button_gui != nullptr && !(credits_button_gui->has_been_clicked))
	{
		App->gui->DeleteGUIElement(credits_label_license);
		App->gui->DeleteGUIElement(credits_label_names);
		credits_label_license = nullptr;
		credits_label_names = nullptr;
	}

	// Volume control 
	if (volume_up_button_gui != nullptr && volume_up_button_gui->has_been_clicked)
	{
		App->audio->ControlVolume(true);
	}
	if (volume_down_button_gui != nullptr && volume_down_button_gui->has_been_clicked)
	{
		App->audio->ControlVolume(false);
	}
	// Volume when pressing options
	if (pause_options_button_gui != nullptr)
	{
		if (pause_options_button_gui->has_been_clicked)
		{
			if (pause_volume_up_button_gui == nullptr)
				pause_volume_up_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 274, 165.0f, { 188, 0, 22, 24 }, { 188, 24, 22, 24 }, { 188, 48, 22, 24 });
			if (pause_volume_down_button_gui == nullptr)
				pause_volume_down_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 297, 165.0f, { 210, 0, 22, 24 }, { 210, 24, 22, 24 }, { 210, 48, 22, 24 });
			if (pause_volume_slider_gui == nullptr)
				pause_volume_slider_gui = (GUISlider*)App->gui->CreateGUISlider(GUI_ELEMENT_TYPE::GUI_SLIDER, 320.0f, 150.0f, { 0, 0, 9, 51 }, { 0, 0, 7, 7 }, { 0, 7, 7, 7 }, { 0, 14, 7, 7 });

		}
		if (!(pause_options_button_gui->has_been_clicked))
		{
			if (pause_volume_up_button_gui != nullptr && pause_volume_down_button_gui != nullptr)
			{
				App->gui->DeleteGUIElement(pause_volume_up_button_gui);
				App->gui->DeleteGUIElement(pause_volume_down_button_gui);
				App->gui->DeleteGUIElement(pause_volume_slider_gui);
				pause_volume_up_button_gui = nullptr;
				pause_volume_down_button_gui = nullptr;
				pause_volume_slider_gui = nullptr;
			}
		}

		if (pause_volume_up_button_gui != nullptr && pause_volume_up_button_gui->has_been_clicked)
			App->audio->ControlVolume(true);
		if (pause_volume_down_button_gui != nullptr && pause_volume_down_button_gui->has_been_clicked)
			App->audio->ControlVolume(false);
	}
	// Adjust slider volume
	if (start_volume_slider_gui != nullptr && start_volume_slider_gui->slider_thumb->is_clicked)
	{
		float volume_value = start_volume_slider_gui->GetSliderValue() * 100;
		App->audio->SetVolume(volume_value);
	}
	if (pause_volume_slider_gui != nullptr && pause_volume_slider_gui->slider_thumb->is_clicked)
	{
		float volume_value = pause_volume_slider_gui->GetSliderValue() * 100;
		App->audio->SetVolume(volume_value);
	}
}

void j1Scene::CheckVolumeInput()
{
	// Set volume
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->ControlVolume(true);
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->ControlVolume(false);
}

void j1Scene::PauseMenuWorkflow()
{
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && start_game)
	{
		App->game_pause = !App->game_pause;
		if (pause_quit_button_gui == nullptr)
		{
			pause_quit_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 226.0f, 200.0f, { 47, 0, 47, 24 }, { 47, 24, 47, 24 }, { 47, 48, 47, 24 });
		}
		if (pause_quit_button_gui != nullptr && App->game_pause == false)
		{
			App->gui->DeleteGUIElement(pause_quit_button_gui);
			pause_quit_button_gui = nullptr;
		}

		if (continue_button_gui == nullptr)
		{
			continue_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 226.0f, 130.0f, { 94, 0, 47, 24 }, { 94, 24, 47, 24 }, { 94, 48, 47, 24 });
		}
		if (continue_button_gui != nullptr && App->game_pause == false)
		{
			App->gui->DeleteGUIElement(continue_button_gui);
			continue_button_gui = nullptr;
		}

		if (pause_options_button_gui == nullptr)
		{
			pause_options_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 226.0f, 165.0f, { 141, 0, 47, 24 }, { 141, 24, 47, 24 }, { 141, 48, 47, 24 });
		}
		if (pause_options_button_gui != nullptr && App->game_pause == false)
		{
			App->gui->DeleteGUIElement(pause_options_button_gui);
			pause_options_button_gui = nullptr;
		}

		// -----
		if (pause_volume_down_button_gui != nullptr && App->game_pause == false)
		{
			App->gui->DeleteGUIElement(pause_volume_down_button_gui);
			pause_volume_down_button_gui = nullptr;
		}
		if (pause_volume_down_button_gui != nullptr && App->game_pause == false)
		{
			App->gui->DeleteGUIElement(pause_volume_down_button_gui);
			pause_volume_down_button_gui = nullptr;
		}
		if (pause_volume_slider_gui != nullptr && App->game_pause == false)
		{
			App->gui->DeleteGUIElement(pause_volume_slider_gui);
			pause_volume_slider_gui = nullptr;
		}
	}
}