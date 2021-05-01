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

   // Initialized SDL
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

   // Initialize SDL for text
   if(TTF_Init() < 0) {
      std::cerr << "Could not load SDL_ttf lib" << std::endl;
   }

   // Create a renderer that uses hardware acceleration and is vertically synced to the screen
   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if(!renderer) {
      std::cerr << "Could not create renderer." << SDL_GetError() << std::endl;
   }


   inited = true;
   start = false;
}

void GameEngine::loop() {

   // Loop at start screen until player presses enter
   // This is handled by the screen itself
   while(!start) {
      Screen* s = screens["start"];
      s->handle_input();
      s->update();
      s->render();
   }

   // Loop until death or quit
   while(running) {
      if(not paused) {
         this->handle_input();
         this->update();
         this->render();
         this->frame_delay();
      }
      // Attempt to handle pausing
      else {
         screens["pause"]->handle_input();
         screens["pause"]->update();
         screens["pause"]->render();
         this->frame_delay();
      }
   }

   // This only runs if the player had a game over, not from a quit or otherwise
   if(gameover) {
      screens["gameover"]->render();
      SDL_Delay(5000);
   }
}

// Load the game setup file
void GameEngine::load_game(std::string game_filename) {
   if(!this->inited) {
      this->init();
   }

   GameParser* gp = gp->get_instance();

   gp->load_game(game_filename);
   player = gp->get_player();

   // Hardcoded player collision box
   player->set_collision_box(8, 15, 16, 17);

   // Get the list of levels, ui elements, and screens in the setup file
   Camera::get_instance()->set_target(player);
   levels = gp->get_levels();
   ui_elems = gp->get_ui_elems();
   screens = gp->get_screens();

   // Bad level spec
   if(levels.size() < 1) {
      std::cerr << "Please specify a level in the game setup!" << std::endl;
      this->running = false;
      return;
   }

   // Setup current in-play level and cleanup the parser
   current_level = levels[0];
   current_level_id = 0;
   current_level->set_camera_lims();
   gp->cleanup();

   int x = current_level->get_start_x();
   int y = current_level->get_start_y();
   static_cast<Player*>(player)->set_start_pos(x, y);
}

// Render all objects and ui elems to screen
void GameEngine::render() {
   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
   SDL_RenderClear(renderer);

   current_level->render();
   player->render();

   for(auto elem : ui_elems) {
      elem->render();
   }

   SDL_RenderPresent(renderer);
}

// Update state of objects and ui elements
void GameEngine::update() {
   player->update();
   current_level->update();
   Camera::get_instance()->update();

   for(auto elem : ui_elems) {
      elem->update();
   }

   // Check if player died
   if(static_cast<Player*>(player)->get_lives() == 0) {
      running = false;
      gameover = true;
   }
}

// Calculate the current framerate, and delay accordingly
void GameEngine::frame_delay() {
   static unsigned fpsCounter, duration;

   current_time = SDL_GetTicks();
   fpsCounter++;

   // Check for at least one second passing
   if(current_time >= (start_time + 1000)) {
      start_time = current_time;
      framerate = fpsCounter;
      fpsCounter = 0;
   }
   
   // Delay if frame rate is too high
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
               case SDLK_p:
                  this->pause();
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

// Cleanup SDL resources and player
void GameEngine::cleanup() {
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   TTF_Quit();

   TextureController::get_instance()->cleanup();
   player->cleanup();
   IMG_Quit();
   SDL_Quit();
}

// Set in-play level to the next one in the list
void GameEngine::next_level() {
   current_level_id++;

   if(current_level_id >= levels.size()) {
      running = false;
      return;
   }
   else {
      current_level = levels[current_level_id];
      current_level->set_camera_lims();
      int x = current_level->get_start_x();
      int y = current_level->get_start_y();
      static_cast<Player*>(player)->set_start_pos(x, y);
   }
}

// Collision detection and correction
// Kind of works, but not really
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
