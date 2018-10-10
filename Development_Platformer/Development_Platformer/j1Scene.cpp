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

	first_map = config.child("first_map").attribute("name").as_string();
	second_map = config.child("second_map").attribute("name").as_string();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (map_selected == 1)
		App->map->Load(first_map.GetString());
	else if (map_selected == 2)
		App->map->Load(second_map.GetString());

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
	/*INPUT--------------------------------------------*/
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

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
			App->player->position = App->player->first_map_pos;
		}
		else
		{
			map_selected = 1;
			App->fade->FadeToBlack(this, this);
		}
	}

	// Start from the current level
	if (App->input->GetKey(SDL_SCANCODE_F2))
	{
		if (map_selected == 1)
		{
			App->player->position = App->player->first_map_pos;
		}
		else if (map_selected == 2)
		{
			App->player->position = App->player->second_map_pos;
		}
	}

	// Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"                            // Uncomment the following if you want to see tileset info as window title
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());*/


	/* DRAW------------ */
	// App->render->Blit(img, 0, 0);
	App->map->Draw();

	// Draw the player
	App->render->Blit(App->player->graphics, (int)App->player->position.x, App->player->position.y, 
		App->player->rect, 1.0f, 0.0, App->player->flip);

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

	return true;
}

bool j1Scene::Load(pugi::xml_node& save)
{
	if (save.child("map_selected") != NULL)
	{
		// We want to load the map when we are not in the same map_selected index
		if(save.child("map_selected").attribute("value").as_int() != map_selected/* && !App->fade->IsFading()*/)
		{
			map_selected = save.child("map_selected").attribute("value").as_int();
			App->fade->FadeToBlack(this, this);
		}
	}

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