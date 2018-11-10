#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

struct Object {

	p2SString name;
	uint id = 0u;
	uint x = 0u;
	uint y = 0u;
	uint width = 0u;
	uint height = 0u;
};

struct ObjectGroup
{
	p2SString name;
	p2List<Object*> objects;

	~ObjectGroup();
};

enum class LayerType
{
	DEFAULT,
	COLLISION,
	WALKABILITY,
	PARALLAX
};

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct MapLayer
{
	p2SString	name;
	int			width;
	int			height;
	uint*		data;
	LayerType type;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	~TileSet();
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
    // List of layers to the map!
	p2List<MapLayer*>   layers;
	p2List<MapLayer*>   collisionLayers;
	p2List<ObjectGroup*> objectGroups;

	// Functions related to object groups/objects
	fPoint ObjectPos(p2SString, p2SString);
	fPoint ObjectSize(p2SString, p2SString);
	bool IsObjectTrigger(p2SString, p2SString, fPoint);
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	// Function to unload the map in order to load another one.
	bool Unload();

    // Method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y) const;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	// Check map collisions
	bool CheckCollisionX(int, int, int);
	bool CheckCollisionY(int, int, int);

private:

	bool LoadMap();

	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
    // Method that loads a single layer
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	// Methods to load object groups
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadObjectGroupDetails(pugi::xml_node&, ObjectGroup*);  // Very similar to LoadTileSetDetails(). We just want the attribute (name) of this object group
	bool LoadObject(pugi::xml_node&, Object*);  // Very similar to LoadTileSetImage(). We just want the attributes of the object

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	float				parallax_speed;
};

#endif // __j1MAP_H__