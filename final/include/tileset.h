#ifndef TILESET_H
#define TILESET_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "GameObject.h"

class Tile: public GameObject {
   public:
      Tile(unsigned x, unsigned y, unsigned w, unsigned h, std::string tileset_image, unsigned img_x, unsigned img_y, bool collidable);
      void render();
   private:
      unsigned img_x;
      unsigned img_y;
};

#endif
