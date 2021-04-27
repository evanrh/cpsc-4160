#include "TextureController.h"
#include "layers.h"

BackgroundLayer::BackgroundLayer(std::string id, std::string filename, int x, int y, float scroll_speed, float x_scale, float y_scale) {
   TextureController *tc = TextureController::get_instance();

   tc->load(id, filename);
   
   this->texture_id = id;
   this->x_offset = x;
   this->y_offset = y;
   this->scroll = scroll_speed;
   this->x_scale = x_scale;
   this->y_scale = y_scale;
}

void BackgroundLayer::render() {
   TextureController::get_instance()->render(texture_id, x_offset, y_offset, SDL_FLIP_NONE, x_scale, y_scale, 0, scroll);
}
