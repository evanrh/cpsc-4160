#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Sprite.h"
#include "globals.h"

class GameObject {
   public:
      GameObject(unsigned start_x, unsigned start_y, SDL_Renderer* ren, const char* img_file);
      ~GameObject();

      void init();
      void cleanup();
      void update();
      void render();
      void set_x_vel(double vel) {x_vel = vel;}
      void set_y_vel(double vel) {y_vel = vel;}
      void set_motion_state(unsigned state) {motion_state = state;}
      double get_x_vel() {return x_vel;}
      double get_y_vel() {return y_vel;}

   private:
      SDL_Renderer* obj_renderer;
      SDL_Texture* obj_graphic;
      SDL_Rect obj_rect;
      SDL_Rect current_frame;
      Sprite* obj_sprite;
      double x_vel;
      double y_vel;
      unsigned last_frame_time;
      unsigned motion_state;
      SDL_RendererFlip flip;
};

#endif
