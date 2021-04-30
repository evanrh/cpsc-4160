#include <tinyxml2/tinyxml2.h>
#include "parsing.h"
#include "GameEngine.h"
#include "camera.h"
#include "TextureController.h"

GameEngine* GameEngine::s_instance = nullptr;

GameEngine::GameEngine() {
}

GameEngine::~GameEngine() {
   delete player;
}

void GameEngine::init() {
   if(this->inited) {
      return;
   }

   if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      std::cout << "Error Initializing SDL: " << SDL_GetError() << std::endl;
   }

   SDL_WindowFlags w_flags = (SDL_WindowFlags) (SDL_WINDOW_ALLOW_HIGHDPI);
   window = SDL_CreateWindow("Base Game Window",
               SDL_WINDOWPOS_CENTERED,
               SDL_WINDOWPOS_CENTERED,
               SCREEN_WIDTH,
               SCREEN_HEIGHT,
               w_flags
               );
   IMG_Init(IMG_INIT_PNG);

   if(TTF_Init() < 0) {
      std::cerr << "Could not load SDL_ttf lib" << std::endl;
   }

   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if(!renderer) {
      std::cerr << "Could not create renderer." << SDL_GetError() << std::endl;
   }

   inited = true;
}

void GameEngine::loop() {

   while(running) {
      this->handle_input();
      this->update();
      this->render();
      this->framerate();
   }
}

void GameEngine::load_game(std::string game_filename) {
   if(!this->inited) {
      this->init();
   }

   GameParser* gp = gp->get_instance();

   gp->load_game(game_filename);
   player = gp->get_player();

   // Hardcoded player collision box
   player->set_collision_box(8, 15, 16, 17);

   Camera::get_instance()->set_target(player);
   levels = gp->get_levels();
   ui_elems = gp->get_ui_elems();

   if(levels.size() < 1) {
      std::cerr << "Please specify a level in the game setup!" << std::endl;
      this->running = false;
      return;
   }
   current_level = levels[0];
   current_level_id = 0;
   gp->cleanup();
}

void GameEngine::render() {
   SDL_RenderClear(renderer);

   current_level->render();
   player->render();

   for(auto elem : ui_elems) {
      elem->render();
   }

   SDL_RenderPresent(renderer);
}

void GameEngine::update() {
   player->update();
   current_level->update();
   this->collisions();
   Camera::get_instance()->update();

   for(auto elem : ui_elems) {
      elem->update();
   }

   if(static_cast<Player*>(player)->get_lives() == 0) {
      running = false;
   }
}

void GameEngine::framerate() {
   // Calculate the current framerate, and delay accordingly
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
   // This will take up to 7 inputs at a time. It seemed like a reasonable number
   unsigned num_inputs = 0;
   unsigned max_inputs = 7;

   // Below in the while loop is the player's state machine
   // Ideally, this will eventually be in the player class, as the engine does not need to know about the various states the player can be in
   // As for now, there are four possible states:
   //    idle
   //    moving left
   //    moving right
   //    jumping
   // Each of these states has a separate sprite animation, except for jumping currently.

   while(SDL_PollEvent(&this->input) and num_inputs < max_inputs) {
      switch(input.type) {
         case SDL_QUIT:
            running = false;
            break;
         case SDL_KEYDOWN:
            switch(input.key.keysym.sym) {
               case SDLK_RIGHT:
                  //player->set_x_pos(player->get_x_pos() + START_X_VEL);
                  player->set_x_vel(START_X_VEL);
                  player->set_motion_state(OBJ_MOVE_RIGHT);
                  break;
               case SDLK_LEFT:
                  //player->set_x_pos(player->get_x_pos() - START_X_VEL);
                  player->set_x_vel(-START_X_VEL);
                  player->set_motion_state(OBJ_MOVE_LEFT);
                  break;
               case SDLK_UP:
                  player->set_y_vel(player->get_y_vel() - START_Y_VEL);
                  player->set_motion_state(OBJ_JUMPING);
                  break;
               default:
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

   double y_vel = player->get_y_vel();
   double y_pos = player->get_y_pos();
   if(num_inputs == 0) {
      // This conditional is the gravity acting upon the player.
      if(y_pos < (SCREEN_HEIGHT - 64) or y_vel != 0) {
         player->set_y_vel(y_vel + .5);
      }
   }
}

void GameEngine::collisions() {

   // Check for collision between player and all GameObjects (will be level tiles at the moment)

   auto tiles = current_level->get_tiles();
   bool collided = false;

   /*
   for(auto tile : tiles) {
      if(collided) {
         collision_avoidance(*player, *tile);
      }
      else {
         collided = collision_avoidance(*player, *tile);
      }
   }
   */
}
void GameEngine::cleanup() {
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   TTF_Quit();

   TextureController::get_instance()->cleanup();
   player->cleanup();
   IMG_Quit();
   SDL_Quit();
}

bool collision_avoidance(GameObject &l, GameObject &r) {

   if(not l.collidable or not r.collidable) return false;

   SDL_Rect p = l.get_collision_box();
   SDL_Rect q = r.get_collision_box();
   SDL_Rect left, right, top, bot;

   if(p.x < q.x) {
      left = p;
      right = q;
   }
   else {
      left = q;
      right = p;
   }

   if(p.y < q.y) {
      top = p;
      bot = q;
   }
   else {
      top = q;
      bot = p;
   }

   if(!((left.x + left.w) < right.x)) {
      if(!((top.y + top.h) < bot.y)) {
         return true;
      }
   }
   return false;
   
}

void GameEngine::next_level() {
   current_level_id++;

   if(current_level_id >= levels.size()) {
      running = false;
      return;
   }
   else {
      current_level = levels[current_level_id];
   }
}
