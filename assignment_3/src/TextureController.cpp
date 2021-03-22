#include "TextureController.h"
#include "GameEngine.h"
#include "camera.h"

TextureController* TextureController::s_instance = nullptr;

bool TextureController::load(std::string id, std::string filename) {
   SDL_Renderer* ren = GameEngine::get_instance()->get_renderer();
   SDL_Surface* surf = IMG_Load(filename.c_str());
   SDL_Texture* tex = SDL_CreateTextureFromSurface(ren,surf);

   if(!tex) {
      std::cerr << "Could not load image: " << SDL_GetError() << std::endl;
      return false;
   }

   this->textures[id] = tex;

   SDL_FreeSurface(surf);
   return true;
}

void TextureController::drop(std::string id) {

}

void TextureController::cleanup() {

}

void TextureController::render(std::string id, int x, int y, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio) {
   SDL_Texture *tex = textures[id];

   SDL_Rect src = {0, 0, 0, 0};
   SDL_Rect cam_rect = Camera::get_instance()->get_view();
   cam_rect.x *= speed_ratio;
   cam_rect.y *= speed_ratio;
   SDL_Rect dest = {x - cam_rect.x, y - cam_rect.y, 0, 0};

   SDL_QueryTexture(tex, nullptr, nullptr, &src.w, &src.h);

   dest.w = src.w * x_scale;
   dest.h = src.h * y_scale;

   SDL_RenderCopyEx(GameEngine::get_instance()->get_renderer(), tex, &src, &dest, rotation, nullptr, flip);
}

