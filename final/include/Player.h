#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "GameObject.h"
#include "Sprite.h"

// Special player class
// Subset of GameObject that has lives and an original position
class Player: public GameObject {
   public:
      Player(int w, int h, std::string sprite_filename);
      ~Player();
      virtual void update();
      void reset();
      void set_lives(unsigned l) { lives = l ;}
      void set_start_pos(int x, int y);
      unsigned get_lives() { return lives ;}
   private:
      unsigned lives;
      int original_x, original_y;
};
#endif
