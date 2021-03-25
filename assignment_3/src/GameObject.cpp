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

   SDL_Rect max = Camera::get_instance()->get_max();

   if(obj_rect.x > (max.w - obj_rect.w + (SCREEN_WIDTH / 2))) { obj_rect.x = max.w - obj_rect.w + (SCREEN_WIDTH / 2); }
   else if( obj_rect.x < 0) { obj_rect.x = 0; }

   if(obj_rect.y > (max.h + obj_rect.h)) { 
      obj_rect.y = max.h +  obj_rect.h;
      y_vel = 0;
   }
   if(obj_rect.y < 0) {
      obj_rect.y = 0;
      y_vel = 0;
   }

   last_frame_time = current_frame_time;
}

void GameObject::render() {
   //std::cout << current_frame % 4 << std::endl;
   TextureController::get_instance()->render_frame(img_id, current_frame, obj_rect, flip, 1, 1, 0, 1);
   //SDL_RenderCopyEx(GameEngine::get_instance()->get_renderer(), obj_graphic, &current_frame, &obj_rect, 0.0, NULL, flip);
}

void GameObject::init() {

}

void GameObject::cleanup() {

}
