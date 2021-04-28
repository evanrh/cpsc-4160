#include <tinyxml2/tinyxml2.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>

#include "parsing.h"
#include "tileset.h"
#include "layers.h"
#include "camera.h"
#include "GameEngine.h"
#include "TextureController.h"
#include "ui.h"

namespace fs = std::filesystem;

MapParser *MapParser::map_instance = nullptr;
GameParser *GameParser::game_instance = nullptr;

void Level::render() {
   for(auto bg : bg_layers) {
      bg.render();
   }
   for(auto tile : tiles) {
      tile->render();
   }
}

void Level::add_tile(Tile *t) {
   tiles.push_back(t);
}

Level* MapParser::load(std::string filename) {
   int width, height;
   int tile_h, tile_w, columns, rows, tile_count;
   Level* loaded_level = new Level();
   tinyxml2::XMLElement *header, *tiles, *tileset;
   tinyxml2::XMLDocument map_file;
   map_file.LoadFile(filename.c_str());

   // Grab header and dimension information from XML file
   header = map_file.FirstChildElement("map");
   header->QueryIntAttribute("width", &width);
   header->QueryIntAttribute("height", &height);
   header->QueryIntAttribute("tilewidth", &tile_w);
   header->QueryIntAttribute("tileheight", &tile_h);
   tileset = header->FirstChildElement("tileset");

   // Get dimensions of tileset
   unsigned img_w = std::stoi(tileset->FirstChildElement("image")->Attribute("width"));
   unsigned img_h = std::stoi(tileset->FirstChildElement("image")->Attribute("height"));
   std::string tileset_image = tileset->FirstChildElement("image")->Attribute("source");
   tileset->QueryIntAttribute("columns", &columns);
   tileset->QueryIntAttribute("tilecount", &tile_count);
   auto layer = header->FirstChildElement("layer");
   tiles = layer->FirstChildElement("data");

   std::stringstream stream(tiles->GetText());
   std::string line;
   unsigned i = 0, j = 0;
   std::string img_id = "tileset";
   rows = tile_count / columns;

   // Loop through all rows of csv data to get all tile rows
   while(getline(stream, line)) {

      // Empty row
      if(line.length() == 0) {
         continue;
      }

      std::stringstream ids(line);
      std::string id_str;

      // Loop through each tile in a row
      while(getline(ids, id_str, ',')) {

         unsigned id = std::stoi(id_str);
         unsigned img_x, img_y;

         // An id of 0 means there is no tile specified
         if(id == 0) {
            j++;
            continue;
         }

         // Find the tile in the tilemap
         for(auto k = 0; k < rows; k++) {
            if(((k * columns) <= id) && ((k * columns + columns) >= id)) {
               img_y = k * tile_h;
               img_x = (id - (k * columns) - 1) * tile_w;
               break;
            }
         }

         // Create a new tile and add it to the level object
         Tile* current = new Tile(j * tile_w, i * tile_h, tile_w, tile_h, tileset_image, img_x, img_y);
         loaded_level->add_tile(current);

         j++;
      }

      j = 0;
      i++;
   }

   // Set level limits
   Camera::get_instance()->set_limits(width * tile_w, height * tile_h);
   return loaded_level;
}

// Imbue game parser with the file to be parsed
void GameParser::load_game(std::string filename) {
   tinyxml2::XMLElement *player_data, *level_data, *root;

   // Create game file pointer and load it into memory
   this->game_file = new tinyxml2::XMLDocument();
   game_file->LoadFile(filename.c_str());

   fs::path filepath = filename.c_str();
   this->root_filepath = filepath.parent_path();
}

// Remove the current game file from memory
void GameParser::cleanup() {
   delete this->game_file;
}

// Return a new player object created from the imbued game file
Player* GameParser::get_player() {
   tinyxml2::XMLElement *player_data, *root;

   root = this->game_file->FirstChildElement("game");
   player_data = root->FirstChildElement("player");

   fs::path full_path = root_filepath;
   full_path /= player_data->Attribute("source");

   int w = std::stoi(player_data->Attribute("width"));
   int h = std::stoi(player_data->Attribute("height"));

   return new Player(w, h, full_path.c_str());
}

// Return a vector of levels specified by level files in the game file
std::vector<Level*> GameParser::get_levels() {
   tinyxml2::XMLElement *level_data, *root;
   std::vector<Level*> levels;

   fs::path level_path = root_filepath;
   fs::path lp_holder = level_path;

   root = this->game_file->FirstChildElement("game");

   MapParser* mp = mp->get_instance();
   for(level_data = root->FirstChildElement("level"); level_data != nullptr; level_data = level_data->NextSiblingElement()) {

      // Get relative of path of each level file
      std::string level_file = level_data->Attribute("source");
      level_path /= level_file;
      Level* current_level = mp->load(level_path.c_str());

      // Parse background layers from level
      float scroll_speed = 1.0f / (64);
      for(auto layer = level_data->FirstChildElement("layer"); layer != nullptr; layer = layer->NextSiblingElement()) {
         std::string tex_id = layer->Attribute("id");
         fs::path layer_path = root_filepath;
         layer_path /= layer->Attribute("source");

         BackgroundLayer bg(tex_id, layer_path.c_str(), 0, 0, scroll_speed, 0.5f, 0.7f);
         scroll_speed *= 2;
         current_level->add_background_layer(bg);

      }

      levels.push_back(current_level);
      level_path = lp_holder;
   }

   return levels;
}

std::vector<UIElement*> GameParser::get_ui_elems() {
   tinyxml2::XMLElement *root, *ui_root, *curr_elem, *font;
   std::vector<UIElement*> elems;

   root = game_file->FirstChildElement("game");
   ui_root = root->FirstChildElement("ui");
   curr_elem = ui_root->FirstChildElement("element");
   font = ui_root->FirstChildElement("font");

   // Parse fonts
   while(font != nullptr) {
      std::string name, source;
      name = font->Attribute("name");
      source = font->Attribute("source");
      TextureController::get_instance()->load_font(name, source);
      font = font->NextSiblingElement("font");
   }

   // Parse UI elements
   while(curr_elem != nullptr) {
      int x, y;
      unsigned w, h;
      std::string name, font_name;
      UIElement* elem;
      
      name = curr_elem->Attribute("name");
      font_name = curr_elem->Attribute("font");
      curr_elem->QueryUnsignedAttribute("width", &w);
      curr_elem->QueryUnsignedAttribute("height", &h);
      
      // Determine which type of UI Element is being parsed
      if(name == "lives") {
         std::string loc = curr_elem->Attribute("loc");
         std::string src = curr_elem->Attribute("source");
         unsigned lives;
         auto ci = Camera::get_instance();

         // Locations of life counter
         // Can be: tl, tr, br, bl; t and b are for top and bottom, while l and r are for right and left
         if(loc == "tl") {
            x = 0;
            y = 0;
         }
         else if(loc == "tr") {
            x = SCREEN_WIDTH - w;
            y = 0;
         }
         else if(loc == "br") {
            x = SCREEN_WIDTH - w;
            y = SCREEN_HEIGHT - h;
         }
         else {
            x = 0;
            y = SCREEN_HEIGHT - h;
         }

         curr_elem->QueryUnsignedAttribute("start", &lives);
         GameEngine::get_instance()->get_player()->set_lives(lives);
         elem = new LifeCounter(x, y, w, h, name, src, font_name);
      }
      // Parse other types of UI elements
      else {

      }
      elems.push_back(elem);
      curr_elem = curr_elem->NextSiblingElement("element");
   }

   return elems;
}
