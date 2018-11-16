#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Collision;
class j1FadeToBlack;
class j1EntityManager;
class j1Particles;
class j1PathFinding;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*           win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1EntityManager*    entities;
	j1Scene*			scene;
	j1Map*				map;
	j1Collision*        collision;
	j1FadeToBlack*      fade;
	j1Particles*        particles;
	j1PathFinding*      pathfinding;

private:

	p2List<j1Module*>	modules;

	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

	uint				frames;
	float				dt;

	uint64 frame_count = 0u;
	j1Timer timer;
	j1PerfTimer ptimer;

	uint32				framerate_cap = 0u;
	bool				cap = true;
	p2SString           is_capped;

	// Units in ms
	float time_since_startup_ms = 0.0f;
	int actual_frame_ms = 0;
	float f = 0.0f;
	float fps = 0.0f;
	float avg_fps = 0.0f;
};

extern j1App* App;

#endif // __j1APP_H__