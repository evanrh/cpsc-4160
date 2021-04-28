#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "GameObject.h"
#include "Sprite.h"

class Player: public GameObject {
   public:
      Player(int w, int h, std::string sprite_filename);
      ~Player();
      virtual void update();
      void set_lives(unsigned l) { lives = l ;}
      unsigned get_lives() { return lives ;}
   private:
      unsigned lives;
};
#endif
