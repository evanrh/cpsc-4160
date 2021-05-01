/*
    Evan Hastings
    CPSC 6160-001
    Spring 2021
    Assignment 1

*/
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <tinyxml2/tinyxml2.h>

#include "GameEngine.h"

using namespace std;

int main(int argc, char** argv) {

   if(argc < 2) {
      cerr << "Please specify a game setup file! (e.g. ./res/joust.xml)" << endl;
      return EXIT_FAILURE;
   }
   GameEngine* engine = GameEngine::get_instance();

   engine->load_game(argv[1]);
   
   engine->loop();
   engine->cleanup();

   return 0;
}
