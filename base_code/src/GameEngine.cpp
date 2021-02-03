#include "GameEngine.h"

GameEngine::GameEngine() {
   player = new GameObject();
}

GameEngine::~GameEngine() {
   delete player;
}

void GameEngine::init() {
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      std::cout << "Error Initializing SDL: " << SDL_GetError() << std::endl;
   }
   window = SDL_CreateWindow("Base Game Window",
               SDL_WINDOWPOS_CENTERED,
               SDL_WINDOWPOS_CENTERED,
               SCREEN_WIDTH,
               SCREEN_HEIGHT,
               0
               );
   renderer = SDL_CreateRenderer(window, -1, 0);

}

void GameEngine::cleanup() {
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);

   player->cleanup();
   SDL_Quit();
}
