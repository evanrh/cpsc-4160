#ifndef PARSING_H
#define PARSING_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <tinyxml2/tinyxml2.h>
#include <string>
#include <vector>
#include <map>
#include "tileset.h"
#include "Player.h"
#include "layers.h"
#include "ui.h"

// Level class representing a playable level
// It holds a tileset and a list of background layers if any
class Level {
   public:
      Level(unsigned w, unsigned h, unsigned tile_w, unsigned tile_h);
      void add_tile(Tile *t);
      void add_background_layer(BackgroundLayer& bg) { bg_layers.push_back(bg); }
      void set_tile_w(unsigned w) { tile_w = w; }
      void set_tile_h(unsigned h) { tile_h = h; }
      void set_camera_lims();
      void update() {}
      void render();
      std::vector<GameObject*> get_tiles() { return this->tiles; };

   private:
      unsigned width, height, tile_w, tile_h;
      std::vector<GameObject*> tiles;
      std::vector<BackgroundLayer> bg_layers; // Background layers. Rendered in back-to-front order
};

// Map parsing class
// This parses an input map file from Tiled and returns corresponding level objects
class MapParser {
   public:
      static MapParser* get_instance() { return map_instance = (map_instance != nullptr) ? map_instance : new MapParser();}

      Level* load(std::string filename);
   private:
      MapParser() {}
      static MapParser* map_instance;
};

// Game setup file parser
// This parses my special game setup format
class GameParser {
   public:
      static GameParser* get_instance() { return game_instance = (game_instance != nullptr) ? game_instance : new GameParser;}

      // Imbue parser with game setup file
      // This must be imbued before the other methods can be run
      void load_game(std::string filename);
      void cleanup();
      Player* get_player(void);
      std::vector<Level*> get_levels();
      std::vector<UIElement*> get_ui_elems();
      std::map<std::string, Screen*> get_screens();
   private:
      GameParser() {}
      static GameParser* game_instance;
      tinyxml2::XMLDocument* game_file;
      std::string root_filepath;
};
#endif
