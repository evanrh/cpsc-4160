
#include "Player.h"
#include "globals.h"
#include "parsing.h"
#include "GameEngine.h"
#include "camera.h"

Player::Player(int w, int h, std::string sprite_filename) : GameObject(0, 0, w, h, "player_sprite", sprite_filename) {
   original_x = 0;
   original_y = 0;
   lives = 3;
}

Player::~Player() {

}

void Player::update() {
   unsigned current_frame_time = SDL_GetTicks();
   obj_sprite->change_state(motion_state);
   current_frame = obj_sprite->update();

   switch(motion_state) {
      case OBJ_IDLE: case OBJ_MOVE_RIGHT:
         flip = SDL_FLIP_NONE;
         break;
      case OBJ_MOVE_LEFT:
         flip = SDL_FLIP_HORIZONTAL;
         break;
      default:
         flip = SDL_FLIP_NONE;
   }

   Level* current_level = GameEngine::get_instance()->get_level();
   std::vector<GameObject*> tiles = current_level->get_tiles();

   obj_rect.x += x_vel * ((double)(current_frame_time - last_frame_time) / 1000);
   for(auto tile : tiles) {
      if(collision_avoidance(*this, *tile)) {
         x_vel = 0;
         obj_rect.x -= x_vel * ((double)(current_frame_time - last_frame_time) / 1000);

         if(tile->is_harmful()) {
            lives -= 1;
            this->reset();
            return;
         }

         break;
      }

   }

   obj_rect.y += y_vel * (double)(current_frame_time - last_frame_time) / 10;
   for(auto tile : tiles) {
      if(collision_avoidance(*this, *tile)) {
         y_vel = 0;
         obj_rect.y -= y_vel * ((double)(current_frame_time - last_frame_time) / 10);

         if(tile->is_harmful()) {
            lives -= 1;
            this->reset();
            return;
         }
         else if(tile->is_win()) {
            GameEngine::get_instance()->next_level();
            this->reset();
         }

         break;

      }
   }


   SDL_Rect max = Camera::get_instance()->get_max();

   // Checks to keep the object in the bounds of the screen
   if(obj_rect.x > (max.w - obj_rect.w )) { obj_rect.x = 0; }
   else if( obj_rect.x < 0) { obj_rect.x = max.w - obj_rect.w; }

   if(obj_rect.y > (max.h - obj_rect.h)) { 
      obj_rect.y = max.h - obj_rect.h;
      y_vel = 0;
   }
   if(obj_rect.y < 0) {
      obj_rect.y = 0;
      y_vel = 0;
   }

   last_frame_time = current_frame_time;
}

// Reset Player position
void Player::reset() {
   obj_rect.x = original_x;
   obj_rect.y = original_y;
}

void Player::set_start_pos(int x, int y) {
   original_x = x;
   original_y = y;
}
