#include "parsing.h"
#include "tileset.h"
#include <tinyxml2/tinyxml2.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

MapParser *MapParser::map_instance = nullptr;
GameParser *GameParser::game_instance = nullptr;

Level* MapParser::load(std::string filename) {
   Level* loaded_level = new Level();
   tinyxml2::XMLElement *header, *tiles, *tileset;
   tinyxml2::XMLDocument map_file;
   map_file.LoadFile(filename.c_str());

   header = map_file.FirstChildElement("map");
   tileset = header->FirstChildElement("tileset");
   tiles = header->FirstChildElement("data");
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
Player* GameParser::get_player(SDL_Renderer* ren) {
   tinyxml2::XMLElement *player_data, *root;

   root = this->game_file->FirstChildElement("game");
   player_data = root->FirstChildElement("player");

   fs::path full_path = root_filepath;
   full_path /= player_data->Attribute("file");

   return new Player(ren, full_path.c_str());
}

// Return a vector of levels specified by level files in the game file
std::vector<Level*> GameParser::get_levels(SDL_Renderer* ren) {
   tinyxml2::XMLElement *level_data, *root;
   std::vector<Level*> levels;

   root = this->game_file->FirstChildElement("game");

   MapParser* mp = mp->get_instance();
   for(level_data = root->FirstChildElement("level"); level_data = level_data->NextSiblingElement(); level_data != nullptr) {
      std::string level_file = level_data->Attribute("file");
      Level* current_level = mp->load(level_file);
      levels.push_back(current_level);
   }

   return levels;
}
