#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#define OBJ_IDLE 0
#define OBJ_MOVE_RIGHT 1
#define OBJ_MOVE_LEFT 2
#define OBJ_JUMPING 3

// Sprite sheet handling class
// There is one big limitation to this class: each state has to have the same number of frames
// Sprite class expects sprite sheet to be in the following order:
//    1st row: idle
//    2nd row: walking / running
//    3rd row: jumping

class Sprite {
   public:
      Sprite(unsigned height, unsigned width, unsigned frames, double duration);
      ~Sprite();

      void cleanup();
      void change_state(unsigned state);
      SDL_Rect update();
   private:
      SDL_Rect sprite_rect;
      unsigned w;
      unsigned h;
      unsigned frame = 0;
      unsigned frame_count;
      double frame_duration;
};
#endif
