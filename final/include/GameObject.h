#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Sprite.h"
#include "globals.h"

// A generic object that is in the game
// GameObject should be treated as an abstract, but it does provide definitions for all of the methods
class GameObject {
   public:
      GameObject(unsigned start_x, unsigned start_y, int w, int h, std::string img_id, std::string img_file);
      ~GameObject();

      void init();
      void cleanup();
      virtual void update();
      virtual void render();
      virtual void set_collision_box(unsigned x, unsigned y, unsigned w, unsigned h);
      void set_x_vel(double vel);
      void set_y_vel(double vel);
      void set_x_pos(unsigned pos);
      void set_y_pos(unsigned pos);
      void set_motion_state(unsigned state) { motion_state = state; }
      void set_harmful(bool v) { harmful = v; }
      void set_win(bool w) { win = w; }
      bool is_win() { return win; }
      bool is_harmful() {return harmful; }
      double get_x_vel() { return x_vel; }
      double get_y_vel() { return y_vel; }
      unsigned get_x_pos() { return obj_rect.x; }
      unsigned get_y_pos() { return obj_rect.y; }
      SDL_Rect get_collision_box();

      friend bool collision_avoidance(GameObject &l, GameObject &r);

   protected:
      SDL_Rect obj_rect;
      SDL_Rect current_frame;
      SDL_Rect collision_box;
      bool collidable, harmful, win;
      std::string img_id;
      Sprite* obj_sprite;
      double x_vel;
      double y_vel;
      unsigned last_frame_time;
      unsigned motion_state;
      SDL_RendererFlip flip;
};

#endif
