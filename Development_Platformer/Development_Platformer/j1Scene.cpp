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
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "ModuleEnemies.h"
#include "Enemy_level01_air.h"
#include "j1Pathfinding.h"

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
	if (map_selected == 1)
	{
		App->map->Load(first_map.GetString());
		App->audio->PlayMusic(first_song.GetString());

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

		/* Add enemies :) */
		/*App->enemies->AddEnemy(ENEMY_TYPES::ENEMY_LEVEL01_GROUND, 295, 320);
		App->enemies->AddEnemy(ENEMY_TYPES::ENEMY_LEVEL01_AIR, 300, 300);*/
	}
	else if (map_selected == 2)
	{
		App->map->Load(second_map.GetString());
		App->audio->PlayMusic(second_song.GetString());
	}

	if (!isLoading)
	{
		if (map_selected == 1)
		{
			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);

			App->player->first_map_pos = App->map->data.ObjectPos("Player", "PlayerStartPos");
			App->player->position = App->player->first_map_pos;
			App->player->previous_position = App->player->position;
		}
		else if (map_selected == 2)
		{
			App->player->second_map_pos = App->map->data.ObjectPos("Player", "PlayerStartPos");
			App->player->position = App->player->second_map_pos;
			App->player->previous_position = App->player->position;
		}
	}

	debug_tex = App->tex->Load("textures/path_tile.png");

	isLoading = false;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
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
		App->render->camera.y -= 3;
	else
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->render->camera.y += 3;
	else
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->render->camera.x -= 3;
	else
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->render->camera.x += 3;
	else
	{
		App->render->camera.x = (int)(App->player->position.x - 242) * (-1) * App->win->GetScale();
		App->render->camera.y = (int)(App->player->position.y - 200) * (-1) * App->win->GetScale();
	}

	// Start from the first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (map_selected == 1)
		{
			App->fade->FadeToBlack(this, this, 0.1f);
			App->player->position = App->player->first_map_pos;
			App->player->previous_position = App->player->position;
		}
		else if(map_selected == 2)
		{
			map_selected = 1;
			App->fade->FadeToBlack(this, this, 0.1f);
			App->player->position = App->player->first_map_pos;
			App->player->previous_position = App->player->position;
		}
	}

	// Start from the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (map_selected == 1)
		{
			App->player->position = App->player->first_map_pos;
			App->player->previous_position = App->player->position;
		}
		else if (map_selected == 2)
		{
			App->player->position = App->player->second_map_pos;
			App->player->previous_position = App->player->position;
		}

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
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || App->map->data.IsObjectTrigger("Player", "PlayerEndPos", App->player->position) && !App->fade->IsFading())
	{
		if (map_selected == 1)
			map_selected = 2;
		else if(map_selected == 2)
			map_selected = 1;

		App->SaveGame();
		App->fade->FadeToBlack(this, this);
	}

	/* ENVABLE/DISABLE GOD MODE */
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (App->player->god_mode == false)
			App->player->god_mode = true;
		else if (App->player->god_mode == true)
			App->player->god_mode = false;
	}

	/* Check if player falls into the death zone */
	if (App->map->data.IsObjectTrigger("DeathZone", "DeathZone_1", App->player->position) ||
		App->map->data.IsObjectTrigger("DeathZone", "DeathZone_2", App->player->position) ||
		App->map->data.IsObjectTrigger("DeathZone", "DeathZone_3", App->player->position) ||
		App->map->data.IsObjectTrigger("DeathZone", "DeathZone_4", App->player->position) ||
		App->map->data.IsObjectTrigger("DeathZone", "DeathZone_5", App->player->position) ||
		App->map->data.IsObjectTrigger("DeathZone", "DeathZone_6", App->player->position) ||
		App->map->data.IsObjectTrigger("DeathZone", "DeathZone_7", App->player->position))
	{
		if (map_selected == 1)
		{
			App->player->position = App->player->first_map_pos;
			App->player->previous_position = App->player->position;
		}
	
		else if (map_selected == 2)
		{
			App->player->position = App->player->second_map_pos;
			App->player->previous_position = App->player->position;
		}
	}

	

	/* DRAW------------ */
	// App->render->Blit(img, 0, 0);
	App->map->Draw();

	// Draw the player
	App->render->Blit(App->player->graphics, (int)App->player->position.x, App->player->position.y, 
		App->player->rect, 1.0f, 0.0, App->player->flip);


	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	if(App->player->god_mode)
		App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	if (App->player->god_mode)
	{
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->Unload();
	App->tex->UnLoad(debug_tex);

	return true;
}

bool j1Scene::Load(pugi::xml_node& save)
{
	if (save.child("map_selected") != NULL)
	{
		// We want to load the map when we are not in the same map_selected index
		if(save.child("map_selected").attribute("value").as_int() != map_selected)
		{
			map_selected = save.child("map_selected").attribute("value").as_int();
			//App->fade->FadeToBlack(this, this, 2.0f);
		}
	}
	isLoading = true;

	return true;
}

bool j1Scene::Save(pugi::xml_node& save) const
{
	if (save.child("map_selected") == NULL)
	{
		save.append_child("map_selected").append_attribute("value") = map_selected;
	}
	else
	{
		save.append_child("map_selected").attribute("value") = map_selected;
	}

	return true;
}