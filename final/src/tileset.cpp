#include "tileset.h"
#include "TextureController.h"

Tile::Tile(unsigned x, unsigned y, unsigned w, unsigned h, std::string tileset_image, unsigned img_x, unsigned img_y, bool collidable) :
   img_x(img_x), img_y(img_y), GameObject(x, y, w, h, "tileset", tileset_image)
{
   this->collidable = collidable;
   if(this->collidable) {
      this->set_collision_box(0, 0, w, h);
   }
}

void Tile::render() {
   SDL_Rect current = {0, 0, obj_rect.w, obj_rect.h};
   current.x = img_x;
   current.y = img_y;
   TextureController::get_instance()->render_frame("tileset", current, obj_rect, flip, 1, 1, 0, 1);
}
