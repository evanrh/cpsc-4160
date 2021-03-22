#include <iostream>
#include "camera.h"
#include "GameObject.h"

Camera* Camera::cam_instance = nullptr;

Camera::Camera() { 
   view_box = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
   target = nullptr;
}
void Camera::update() {

   if(target != nullptr) {

      view_box.x = target->get_x_pos() - (SCREEN_WIDTH / 2);
      view_box.y = target->get_y_pos() - (SCREEN_HEIGHT / 2);

      if(view_box.x < 0) {
         view_box.x = 0;
      }
      else if(view_box.x > (scene_width - view_box.w)) {
         view_box.x = scene_width - view_box.w;
      }

      if(view_box.y < 0) {
         view_box.y = 0;
      }
      else if(view_box.y > (scene_height - view_box.h)) {
         view_box.y = scene_height - view_box.h;
      }
      std::cout << "Camera pos: (" << view_box.x << " " << view_box.y << ")" << std::endl;
   }
}
