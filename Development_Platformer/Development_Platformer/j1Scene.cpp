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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("map1.tmx");

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
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
		App->render->camera.y -= 3;
	else
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		App->render->camera.y += 3;
	else
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
		App->render->camera.x -= 3;
	else
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		App->render->camera.x += 3;
	else
	{
		App->render->camera.x = (int)(App->player->position.x - 242) * (-1) * App->win->GetScale();
		App->render->camera.y = (int)(App->player->position.y - 200) * (-1) * App->win->GetScale();
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
	App->render->Blit(App->player->graphics, App->player->position.x, App->player->position.y, 
		App->player->rect);

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

	return true;
}
