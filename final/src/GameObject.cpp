#include "GameObject.h"
#include "GameEngine.h"
#include "TextureController.h"
#include "camera.h"

GameObject::GameObject(unsigned start_x, unsigned start_y, int w, int h, std::string img_id, std::string img_file) {

   this->img_id = img_id;
   obj_rect.x = start_x;
   obj_rect.y = start_y;
   obj_rect.w = w;
   obj_rect.h = h;

   TextureController::get_instance()->load(img_id, img_file);

   obj_sprite = new Sprite(w, h, 4, 1000 / 10);
   x_vel = 0;
   y_vel = 0;

   flip = SDL_FLIP_NONE;
   collidable = false;
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

   obj_rect.x += x_vel * ((double)(current_frame_time - last_frame_time) / 1000);
   obj_rect.y += y_vel * (double)(current_frame_time - last_frame_time) / 10;


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

void GameObject::render() {
   TextureController::get_instance()->render_frame(img_id, current_frame, obj_rect, flip, 1, 1, 0, 1);
}

void GameObject::set_collision_box(unsigned x, unsigned y, unsigned w, unsigned h) {
   collidable = true;
   collision_box.x = x;
   collision_box.y = y;
   collision_box.w = w;
   collision_box.h = h;
}

SDL_Rect GameObject::get_collision_box() {
   return {collision_box.x + obj_rect.x, collision_box.y + obj_rect.y, collision_box.w, collision_box.h};
}
void GameObject::init() {

}

void GameObject::cleanup() {

}
