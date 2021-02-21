#include "GameEngine.h"

GameEngine::GameEngine() {
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
   IMG_Init(IMG_INIT_PNG);
   renderer = SDL_CreateRenderer(window, -1, 0);
   if(!renderer) {
      std::cerr << "Could not create renderer." << std::endl;
   }
   player = new Player(renderer);

}

void GameEngine::loop() {

   while(running) {
      this->handle_input();
      this->update();
      this->render();
      this->framerate();
   }
}

void GameEngine::render() {
   SDL_RenderClear(renderer);
   player->render();
   SDL_RenderPresent(renderer);
   //SDL_SetRenderDrawColor(renderer, 0,0,0,255);
}

void GameEngine::update() {
   unsigned y_vel = player->get_y_vel();
   unsigned update_start_time = SDL_GetTicks();
   if(y_vel != 0) {
      player->set_y_vel(GRAVITY * ((update_start_time - y_start_time) / 1000) + y_vel);
      std::cout << player->get_y_vel() << std::endl;
   }
   else if(y_vel >= MAX_Y_VEL) {
      y_vel = MAX_Y_VEL;
   }
   player->update();
}

void GameEngine::framerate() {
   static unsigned fps, fpsCounter, duration;

   current_time = SDL_GetTicks();
   fpsCounter++;
   if(current_time >= (start_time + 1000)) {
      start_time = current_time;
      fps = fpsCounter;
      fpsCounter = 0;
   }
   
   duration = SDL_GetTicks() - current_time;
   if(duration < FRAME_DURATION) {
      SDL_Delay(FRAME_DURATION - duration);
   }
}

void GameEngine::handle_input() {
   unsigned num_inputs = 0;
   unsigned max_inputs = 7;
   while(SDL_PollEvent(&this->input) and num_inputs < max_inputs) {
      switch(input.type) {
         case SDL_QUIT:
            running = false;
            break;
         case SDL_KEYDOWN:
            switch(input.key.keysym.sym) {
               case SDLK_RIGHT:
                  player->set_x_vel(START_X_VEL);
                  player->set_motion_state(OBJ_MOVE_RIGHT);
                  break;
               case SDLK_LEFT:
                  player->set_x_vel(-1 * START_X_VEL);
                  player->set_motion_state(OBJ_MOVE_LEFT);
                  break;
               case SDLK_UP:
                  if(player->get_y_vel() == 0) {
                     y_start_time = SDL_GetTicks();
                     player->set_y_vel(-1 * START_Y_VEL);
                  }
                  break;
            }
            break;
         case SDL_KEYUP:
            switch(input.key.keysym.sym) {
               case SDLK_RIGHT: case SDLK_LEFT:
                  player->set_x_vel(0);
                  player->set_motion_state(OBJ_IDLE);
                  break;
            }
            break;
            
      }
      num_inputs++;
   }
}

void GameEngine::cleanup() {
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);

   player->cleanup();
   IMG_Quit();
   SDL_Quit();
}
