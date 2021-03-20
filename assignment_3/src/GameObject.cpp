#include "GameObject.h"

GameObject::GameObject(unsigned start_x, unsigned start_y, SDL_Renderer* ren, const char* img_file) {
   obj_renderer = ren;

   obj_rect.x = start_x;
   obj_rect.y = start_y;
   obj_rect.w = 64;
   obj_rect.h = 64;

   SDL_Surface* surf = IMG_Load(img_file);
   obj_graphic = SDL_CreateTextureFromSurface(ren, surf);

   if(!obj_graphic) {
      std::cerr << "Could not load image: " << SDL_GetError() << std::endl;
   }

   SDL_FreeSurface(surf);
   obj_sprite = new Sprite(32, 32, 4, 1000 / 10);
   x_vel = 0;
   y_vel = 0;
}

GameObject::~GameObject() {
   delete obj_sprite;
}

void GameObject::set_x_vel(double vel) {
   x_vel = vel;
}

void GameObject::set_y_vel(double vel) {
   y_vel = vel;
}

void GameObject::set_x_pos(unsigned pos) {
   obj_rect.x = pos;
}

void GameObject::set_y_pos(unsigned pos) {
   obj_rect.y = pos;
}

void GameObject::update() {

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

   obj_rect.x += x_vel * ((current_frame_time - last_frame_time) / 1000);
   obj_rect.y += y_vel * (double)(current_frame_time - last_frame_time) / 10;

   // Checks to keep the object in the bounds of the screen
   if(obj_rect.x > SCREEN_WIDTH) { obj_rect.x = 0; }
   else if( obj_rect.x < 0) { obj_rect.x = SCREEN_WIDTH; }

   if(obj_rect.y > (SCREEN_HEIGHT - obj_rect.h)) { 
      obj_rect.y = SCREEN_HEIGHT - obj_rect.h;
      y_vel = 0;
   }
   if(obj_rect.y < 0) {
      obj_rect.y = 0;
      y_vel = 0;
   }

   last_frame_time = current_frame_time;
}

void GameObject::render() {
   SDL_RenderCopyEx(obj_renderer, obj_graphic, &current_frame, &obj_rect, 0.0, NULL, flip);
}
void GameObject::init() {

}

void GameObject::cleanup() {

}
