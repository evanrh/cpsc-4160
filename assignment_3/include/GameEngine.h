#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "globals.h"
#include "GameObject.h"
#include "Player.h"
#include "parsing.h"


class GameEngine {
   public:
      static GameEngine* get_instance() { return s_instance = (s_instance != nullptr) ? s_instance : new GameEngine() ;}

      void load_game(std::string game_filename);
      void handle_input();
      void render();
      void cleanup();
      void loop();
      void update();
      void framerate();
      SDL_Renderer* get_renderer() { return renderer; }
   private:
      static GameEngine *s_instance;
      Player* player;
      SDL_Renderer* renderer;
      SDL_Window* window;
      SDL_Event input;
      bool running = true;
      bool inited = false;
      unsigned current_time;
      unsigned start_time;
      unsigned y_start_time;
      std::vector<Level*> levels;
      Level* current_level;

      GameEngine();
      ~GameEngine();
      void init();
};

#endif
