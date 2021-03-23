#include <tinyxml2/tinyxml2.h>
#include <filesystem>
#include <fstream>

#include "parsing.h"
#include "tileset.h"
#include "layers.h"
#include "camera.h"
#include "GameEngine.h"

namespace fs = std::filesystem;

MapParser *MapParser::map_instance = nullptr;
GameParser *GameParser::game_instance = nullptr;

void Level::render() {
   for(auto bg : bg_layers) {
      bg.render();
   }
}

Level* MapParser::load(std::string filename) {
   int width, height;
   int tile_h, tile_w;
   Level* loaded_level = new Level();
   tinyxml2::XMLElement *header, *tiles, *tileset;
   tinyxml2::XMLDocument map_file;
   map_file.LoadFile(filename.c_str());

   header = map_file.FirstChildElement("map");
   width = std::stoi(header->Attribute("width"));
   height = std::stoi(header->Attribute("height"));
   tile_w = std::stoi(header->Attribute("tilewidth"));
   tile_h = std::stoi(header->Attribute("tileheight"));
   tileset = header->FirstChildElement("tileset");
   tiles = header->FirstChildElement("data");

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
   full_path /= player_data->Attribute("file");

   int w = std::stoi(player_data->Attribute("width"));
   int h = std::stoi(player_data->Attribute("height"));

   return new Player(w, h, full_path.c_str());
}

// Return a vector of levels specified by level files in the game file
std::vector<Level*> GameParser::get_levels(SDL_Renderer* ren) {
   tinyxml2::XMLElement *level_data, *root;
   std::vector<Level*> levels;

   fs::path level_path = root_filepath;
   fs::path lp_holder = level_path;

   root = this->game_file->FirstChildElement("game");

   MapParser* mp = mp->get_instance();
   for(level_data = root->FirstChildElement("level"); level_data != nullptr; level_data = level_data->NextSiblingElement()) {

      // Get relative of path of each level file
      std::string level_file = level_data->Attribute("file");
      level_path /= level_file;
      Level* current_level = mp->load(level_path.c_str());

      // Parse background layers from level
      float scroll_speed = 1.0f / (64);
      for(auto layer = level_data->FirstChildElement("layer"); layer != nullptr; layer = layer->NextSiblingElement()) {
         std::string tex_id = layer->Attribute("id");
         fs::path layer_path = root_filepath;
         layer_path /= layer->Attribute("file");

         BackgroundLayer bg(tex_id, layer_path.c_str(), 0, 0, scroll_speed, 0.5f, 0.7f);
         scroll_speed *= 2;
         current_level->add_background_layer(bg);

      }

      levels.push_back(current_level);
      level_path = lp_holder;
   }

   return levels;
}
