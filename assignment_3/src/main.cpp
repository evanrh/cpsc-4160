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

int main(void) {

   GameEngine* engine = new GameEngine();

   engine->init();
   engine->loop();
   engine->cleanup();

   delete engine;

   return 0;
}
