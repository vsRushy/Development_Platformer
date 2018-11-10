#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	parallax_speed = config.child("parallax_speed").attribute("value").as_float();
	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

    // Draw all tilesets + Blit
	p2List_item<TileSet*>* tilesetItem = data.tilesets.start;
	p2List_item<MapLayer*>* layerItem = data.layers.start;
	p2List_item<MapLayer*>* collisionsLayerItem = data.collisionLayers.start;

	while (tilesetItem != NULL)
	{
		while (layerItem != NULL)
		{
			for (uint j = 0; j < layerItem->data->height; ++j)
			{
				for (uint i = 0; i < layerItem->data->width; ++i)
				{
					// get tile id
					int tileid = layerItem->data->Get(i, j);

					if (tileid != 0)
					{
						// get tile rect
						SDL_Rect printRect = tilesetItem->data->GetTileRect(tileid);
						iPoint printCoords = MapToWorld(i, j);
						
						if(layerItem->data->type == LayerType::PARALLAX)
						{ 
							App->render->Blit(tilesetItem->data->texture, printCoords.x, printCoords.y, &printRect, parallax_speed);
						}
						else if(layerItem->data->type == LayerType::DEFAULT &&
							printCoords.x < (App->render->camera.x * (-1) + App->render->camera.w) / App->win->GetScale() &&
							printCoords.x + tilesetItem->data->tile_width > App->render->camera.x * (-1) / App->win->GetScale() &&
							printCoords.y < (App->render->camera.y * (-1) + App->render->camera.h) / App->win->GetScale() &&
							printCoords.y + tilesetItem->data->tile_height > App->render->camera.y * (-1) / App->win->GetScale())
						{
							App->render->Blit(tilesetItem->data->texture, printCoords.x, printCoords.y, &printRect);
						}
					}
				}
			}

			layerItem = layerItem->next;
		}
		while (collisionsLayerItem != nullptr) {
			for (uint j = 0; j < collisionsLayerItem->data->height; j++) 
			{
				for (uint i = 0; i < collisionsLayerItem->data->width; i++) 
				{
					// get tile id
					int tileid = collisionsLayerItem->data->Get(i, j);

					if (tileid != 0) {
						iPoint pos = MapToWorld(i, j);
						SDL_Rect printRect = tilesetItem->data->GetTileRect(collisionsLayerItem->data->Get(i, j));

						if (collisionsLayerItem->data->type == LayerType::COLLISION && App->collision->IsDebug() &&
							pos.x < (App->render->camera.x * (-1) + App->render->camera.w) / App->win->GetScale() &&
							pos.x + tilesetItem->data->tile_width > App->render->camera.x * (-1) / App->win->GetScale() &&
							pos.y < (App->render->camera.y * (-1) + App->render->camera.h) / App->win->GetScale() &&
							pos.y + tilesetItem->data->tile_height > App->render->camera.y * (-1) / App->win->GetScale())
								App->render->Blit(tilesetItem->data->texture, pos.x, pos.y, &printRect);
					}
				}
			}

			collisionsLayerItem = collisionsLayerItem->next;
		}

		layerItem = data.layers.start;
		collisionsLayerItem = data.collisionLayers.start;
		tilesetItem = tilesetItem->next;
	}
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item_tileset;
	item_tileset = data.tilesets.start;

	while (item_tileset != NULL)
	{
		RELEASE(item_tileset->data);
		item_tileset = item_tileset->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item_layer;
	item_layer = data.layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.layers.clear();

	// Remove all collision layers
	p2List_item<MapLayer*>* item_collision_layer;
	item_collision_layer = data.collisionLayers.start;

	while (item_collision_layer != NULL)
	{
		RELEASE(item_collision_layer->data);
		item_collision_layer = item_collision_layer->next;
	}
	data.collisionLayers.clear();

	// Remove all object groups
	p2List_item<ObjectGroup*>* item_object_group;
	item_object_group = data.objectGroups.start;

	while (item_object_group != NULL)
	{
		p2List_item<Object*>* item_object;
		item_object = item_object_group->data->objects.start;

		while (item_object != NULL)
		{
			RELEASE(item_object->data);
			item_object = item_object->next;
		}
		item_object_group->data->objects.clear();
		
		RELEASE(item_object_group->data);
		item_object_group = item_object_group->next;
	}
	data.objectGroups.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}

		if (set->type == LayerType::COLLISION)
		{
			data.collisionLayers.add(set);
		}
		else
		{
			data.layers.add(set);
		}
	}

	// Iterate all game objects and load each of them
	// Load object group info ---------------------------------------
	pugi::xml_node objGroup;
	pugi::xml_node obj;
	for (objGroup = map_file.child("map").child("objectgroup"); objGroup && ret; objGroup = objGroup.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjectGroupDetails(objGroup, set);
		}

		for (obj = objGroup.child("object"); obj && ret; obj = obj.next_sibling("object"))
		{
			Object* o = new Object();
			if (ret == true)
			{
				ret = LoadObject(obj, o);
			}
			set->objects.add(o);
		}
		data.objectGroups.add(set);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		// Info about the loaded tilesets
		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// Info about the loaded layers
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		// Info about the loaded objects
		p2List_item<ObjectGroup*>* item_object_group = data.objectGroups.start;
		while (item_object_group != NULL)
		{
			// We want info about the object group...
			ObjectGroup* og = item_object_group->data;
			LOG("Object group ----");
			LOG("name: %s", og->name.GetString());
			// But also the objects inside this object group!
			p2List_item<Object*>* item_object = item_object_group->data->objects.start;
			while (item_object != NULL)
			{
				Object* o = item_object->data;
				LOG("Object ----");
				LOG("name: %s", o->name.GetString());
				LOG("id: %i", o->id);
				LOG("x: %i", o->x);
				LOG("y: %i", o->y);
				LOG("width: %i", o->width);
				LOG("height: %i", o->height);

				item_object = item_object->next;
			}
			item_object_group = item_object_group->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// Definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name.create(node.attribute("name").as_string());
	if (layer->name == "Parallax")
	{
		layer->type = LayerType::PARALLAX;
	}
	else if (layer->name == "Colliders")
	{
		layer->type = LayerType::COLLISION;
	}
	else
	{
		layer->type = LayerType::DEFAULT;
	}

	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();

	layer->data = new uint[layer->width * layer->height];

	memset(layer->data, 0, layer->width * layer->height * sizeof(uint));

	uint i = 0;
	for (pugi::xml_node aux = node.child("data").child("tile"); aux; aux = aux.next_sibling("tile"))
	{
		layer->data[i++] = aux.attribute("gid").as_uint();
	}

	return ret;
}

//----------------------------------
bool j1Map::LoadObjectGroupDetails(pugi::xml_node& node, ObjectGroup* objectGroup)
{
	objectGroup->name.create(node.attribute("name").as_string());

	return true;
}

bool j1Map::LoadObject(pugi::xml_node& node, Object* object)
{
	object->name.create(node.attribute("name").as_string());
	object->id = node.attribute("id").as_uint();
	object->x = node.attribute("x").as_uint();
	object->y = node.attribute("y").as_uint();
	object->width = node.attribute("width").as_uint();
	object->height = node.attribute("height").as_uint();

	return true;
}

TileSet::~TileSet()
{
	App->tex->UnLoad(texture);
}

ObjectGroup::~ObjectGroup()
{
	p2List_item<Object*>* item;
	item = objects.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
}

// COLLISIONS ------------------------------
bool j1Map::CheckCollisionX(int x, int upper_y, int lower_y)
{
	bool ret = false;

	if (!ret)
	{
		p2List_item<MapLayer*>* c_item = data.collisionLayers.start;
		for (int y = upper_y; y <= lower_y; y++)
		{
			if (c_item->data->Get(x, y) != 0)
			{
				ret = true;
			}
		}
	}

	return ret;
}

bool j1Map::CheckCollisionY(int y, int left_x, int right_x)
{
	bool ret = false;

	if (!ret)
	{
		p2List_item<MapLayer*>* collisions = data.collisionLayers.start;
		for (uint x = left_x; x <= right_x; x++)
		{
			if (collisions->data->Get(x, y) != 0)
				ret = true;
		}
	}
	
	return ret;
}

bool j1Map::Unload()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item_tileset;
	item_tileset = data.tilesets.start;

	while (item_tileset != NULL)
	{
		RELEASE(item_tileset->data);
		item_tileset = item_tileset->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item_layer;
	item_layer = data.layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.layers.clear();

	// Remove all collision layers
	p2List_item<MapLayer*>* item_collision_layer;
	item_collision_layer = data.collisionLayers.start;

	while (item_collision_layer != NULL)
	{
		RELEASE(item_collision_layer->data);
		item_collision_layer = item_collision_layer->next;
	}
	data.collisionLayers.clear();

	// Remove all object groups
	p2List_item<ObjectGroup*>* item_object_group;
	item_object_group = data.objectGroups.start;

	while (item_object_group != NULL)
	{
		p2List_item<Object*>* item_object;
		item_object = item_object_group->data->objects.start;

		while (item_object != NULL)
		{
			RELEASE(item_object->data);
			item_object = item_object->next;
		}
		item_object_group->data->objects.clear();

		RELEASE(item_object_group->data);
		item_object_group = item_object_group->next;
	}
	data.objectGroups.clear();

	return true;
}

// OBJECT FUNCTIONS --------------------
fPoint MapData::ObjectPos(p2SString obj_group_name, p2SString obj_name)
{
	fPoint pos;
	p2List_item<ObjectGroup*>* obj_group_item = objectGroups.start;
	bool stop = false;
	while (obj_group_item != NULL && !stop)
	{
		// We just want the objectgroup desired
		if (obj_group_item->data->name == obj_group_name)
		{
			p2List_item<Object*>* obj_item = obj_group_item->data->objects.start;
			while (obj_item != NULL && !stop)
			{
				if (obj_item->data->name == obj_name)
				{
					pos.x = obj_item->data->x;
					pos.y = obj_item->data->y;
					stop = true;
				}
				obj_item = obj_item->next;
			}
		}
		obj_group_item = obj_group_item->next;
	}
	return pos;
}

fPoint MapData::ObjectSize(p2SString obj_group_name, p2SString obj_name)
{
	fPoint size;
	p2List_item<ObjectGroup*>* obj_group_item = objectGroups.start;
	bool stop = false;
	while (obj_group_item != NULL && !stop)
	{
		// We just want the objectgroup desired
		if (obj_group_item->data->name == obj_group_name)
		{
			p2List_item<Object*>* obj_item = obj_group_item->data->objects.start;
			while (obj_item != NULL && !stop)
			{
				if (obj_item->data->name == obj_name)
				{
					size.x = obj_item->data->width;
					size.y = obj_item->data->height;
					stop = true;
				}
				obj_item = obj_item->next;
			}
		}
		obj_group_item = obj_group_item->next;
	}
	return size;
}

bool MapData::IsObjectTrigger(p2SString obj_group_name, p2SString obj_name, fPoint player_position)
{
	bool ret = false;

	fPoint object_position;
	fPoint object_size;

	object_position = ObjectPos(obj_group_name, obj_name);
	object_size = ObjectSize(obj_group_name, obj_name);

	if (object_position.x <= player_position.x && 
		object_position.x + object_size.x > player_position.x &&
		object_position.y <= player_position.y &&
		object_size.y + object_position.y > player_position.y)
			ret = true;

	return ret;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}