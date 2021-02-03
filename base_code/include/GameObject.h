#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class GameObject {
   public:
      GameObject();
      ~GameObject();

      void init();
      void cleanup();
   private:
      SDL_Renderer* obj_renderer;
      SDL_Rect* obj_rect;
      double x_vel;
      double y_vel;
};

#endif
