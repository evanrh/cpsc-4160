#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameObject.h"
#include "Player.h"

// Viewing camera of the engine
// This is used to define a reference position to the engine as to what should be rendered
class Camera {
   public:
      static Camera* get_instance() { return cam_instance = (cam_instance != nullptr) ? cam_instance : new Camera(); }
      void update();
      SDL_Rect get_view() { return view_box; }
      SDL_Rect get_max() { return {0, 0, scene_width, scene_height}; }
      void set_target(GameObject* t) { target = t; }
      void set_limits(int w, int h);

   private:
      static Camera* cam_instance;
      int scene_width, scene_height;
      SDL_Rect view_box;
      GameObject* target;

      Camera();
};

#endif
