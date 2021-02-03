#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "GameObject.h"

#define FPS 60
#define FRAME_DURATION 1000 / FPS
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class GameEngine {
   public:
      GameEngine();
      ~GameEngine();

      void init();
      void handle_events();
      void render();
      void cleanup();
   private:
      GameObject* player;
      SDL_Renderer* renderer;
      SDL_Window* window;

};

#endif
