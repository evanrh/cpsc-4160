#ifndef TILESET_H
#define TILESET_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "GameObject.h"

class Tile: public GameObject {
   private:
      SDL_Rect collision_box;
};

class TileSet {
   public:
      TileSet(unsigned tw=32, unsigned th=32) : tile_width(tw), tile_height(th) {}
   private:
      unsigned tile_width;
      unsigned tile_height;
      std::vector<Tile> tiles;
};
#endif
