#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "GameObject.h"
#include "Sprite.h"

class Player: public GameObject {
   public:
      Player(SDL_Renderer* ren, int w, int h, const char *sprite_filename);
      ~Player();
};
#endif
