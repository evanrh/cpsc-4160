#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "GameObject.h"
#include "Player.h"
#include "parsing.h"
#include "ui.h"

class GameEngine {
   public:
      static GameEngine* get_instance() { return s_instance = (s_instance != nullptr) ? s_instance : new GameEngine() ;}

      void load_game(std::string game_filename);
      void handle_input();
      void render();
      void cleanup();
      void loop();
      void collisions();
      void update();
      void framerate();
      void next_level();
      void stop_game() { running = false; }
      void pause() { paused = true; }
      void unpause() { paused = false; }
      SDL_Renderer* get_renderer() { return renderer; }
      Level* get_level() { return current_level; }
      Player* get_player() { return static_cast<Player*>(player) ;}
      unsigned get_framerate() { return framerate; }
   private:
      static GameEngine *s_instance;
      GameObject* player;
      SDL_Renderer* renderer;
      SDL_Window* window;
      SDL_Event input;
      bool running = true, inited = false, paused = false;
      unsigned current_time, start_time, y_start_time, framerate;
      std::vector<Level*> levels;
      std::vector<UIElement*> ui_elems;
      unsigned current_level_id;
      std::map<std::string, Screen*> screens;
      Level* current_level;

      GameEngine();
      ~GameEngine();
      void init();
      void frame_delay();
};

#endif
