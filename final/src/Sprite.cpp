#include "Sprite.h"

Sprite::Sprite(unsigned height, unsigned width, unsigned frames, double duration) {
   h = height;
   w = width;
   frame_count = frames;
   frame_duration = duration;

   sprite_rect.w = w;
   sprite_rect.h = h;
   sprite_rect.x = 0;
   sprite_rect.y = 0;
}

Sprite::~Sprite() {

}

SDL_Rect Sprite::update() {
   // Get frame number based off of sprite framerate
   int frame_time = (int) (SDL_GetTicks() / frame_duration) % frame_count;

   static int last_frame_time = 0;

   // Update time and sprite position
   last_frame_time = frame_time;
   sprite_rect.x = frame_time * w;

   
   return sprite_rect;
}

void Sprite::change_state(unsigned state) {
   
   // Change movement states based on input state
   switch(state) {
      case OBJ_IDLE:
         sprite_rect.y = 0;
         break;

      case OBJ_MOVE_RIGHT: case OBJ_MOVE_LEFT:
         sprite_rect.y = h;
         break;

      default:
         sprite_rect.y = 0;
   }
   sprite_rect.x = 0;

}
void Sprite::cleanup() {
   return;
}
