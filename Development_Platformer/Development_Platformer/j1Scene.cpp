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
		bg = App->tex->Load("textures/background_start.png");
		start_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 4.0f, 4.0f, { 0, 0, 47, 24 }, { 0, 24, 47, 24 }, { 0, 48, 47, 24 });
		quit_button_gui = (GUIButton*)App->gui->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 4.0f, 32.0f, { 47, 0, 47, 24 }, { 47, 24, 47, 24 }, { 47, 48, 47, 24 });
		label_gui = (GUILabel*)App->gui->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, 163.0f, 97.0f, "Testing", { 255, 255, 255, 255 }, App->gui->default_font_used);
		logo_gui = (GUIImage*)App->gui->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 424.0f, 358.0f, { 0, 0, 84, 22 });
	}

	if (start_game)
	{
		

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

			/* Add entities :) */
			if(player == nullptr)
				player = (Player*)App->entities->CreateEntity(ENTITY_TYPES::PLAYER, first_map_pos.x, first_map_pos.y);
			if(enemy01air == nullptr)
				enemy01air = (Enemy_level01_air*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_AIR, 470, 200);
			if(enemy01ground == nullptr)
				enemy01ground = (Enemy_level01_ground*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_GROUND, 960, 779);

			player->position = first_map_pos;
			player->previous_position = player->position;
		}
		else if (map_selected == 2)
		{
			App->map->Load(second_map.GetString());
			App->audio->PlayMusic(second_song.GetString());

			second_map_pos = App->map->data.ObjectPos("Player", "PlayerStartPos");

			player = (Player*)App->entities->CreateEntity(ENTITY_TYPES::PLAYER, second_map_pos.x, second_map_pos.y);
			enemy02air = (Enemy_level01_air*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_AIR, 2274, 992);
			enemy02ground = (Enemy_level01_ground*)App->entities->CreateEntity(ENTITY_TYPES::ENEMY_LEVEL01_GROUND, 1280, 1131);

			player->position = second_map_pos;
			player->previous_position = player->position;
		}

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

	// UI Check
	if (start_button_gui != nullptr && start_button_gui->is_pressed == !start_game)
	{
		start_game = true;
		// Delete starting active GUI
		if (start_button_gui != nullptr)
		{
			App->gui->DeleteGUIElement(start_button_gui);
			start_button_gui = nullptr;
		}
		if (quit_button_gui != nullptr)
		{
			App->gui->DeleteGUIElement(quit_button_gui);
			quit_button_gui = nullptr;
		}
		if (label_gui != nullptr)
		{
			App->gui->DeleteGUIElement(label_gui);
			label_gui = nullptr;
		}
		if (logo_gui != nullptr)
		{
			App->gui->DeleteGUIElement(logo_gui);
			logo_gui = nullptr;
		}

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
				player->position = first_map_pos;
				player->previous_position = player->position;
			}
			else if (map_selected == 2)
			{
				map_selected = 1;
				App->entities->DeleteAllEntities();
				player->position = first_map_pos;
				player->previous_position = player->position;
			}
			player->player_start = true;
			App->fade->FadeToBlack(this, this, 0.1f);
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
			App->fade->FadeToBlack(this, this, 0.1f);
		}

		// Set volume
		if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
			App->audio->ControlVolume(true);
		if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
			App->audio->ControlVolume(false);

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

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || (quit_button_gui != nullptr && quit_button_gui->is_pressed))
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->Unload();
	App->tex->UnLoad(debug_tex);
	App->tex->UnLoad(bg);

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