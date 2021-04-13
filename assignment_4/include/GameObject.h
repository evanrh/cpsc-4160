#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Sprite.h"
#include "globals.h"

class GameObject {
   public:
      GameObject(unsigned start_x, unsigned start_y, int w, int h, std::string img_id, std::string img_file);
      ~GameObject();

      void init();
      void cleanup();
      void update();
      virtual void render();
      void set_x_vel(double vel);
      void set_y_vel(double vel);
      void set_x_pos(unsigned pos);
      void set_y_pos(unsigned pos);
      void set_motion_state(unsigned state) {motion_state = state;}
      double get_x_vel() {return x_vel;}
      double get_y_vel() {return y_vel;}
      unsigned get_x_pos() {return obj_rect.x;};
      unsigned get_y_pos() {return obj_rect.y;};

      friend void collision_avoidance(GameObject &l, GameObject &r);

   protected:
      SDL_Rect obj_rect;
      SDL_Rect current_frame;
      std::string img_id;
      Sprite* obj_sprite;
      double x_vel;
      double y_vel;
      unsigned last_frame_time;
      unsigned motion_state;
      SDL_RendererFlip flip;
};

#endif
