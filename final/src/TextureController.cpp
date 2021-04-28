#include "TextureController.h"
#include "GameEngine.h"
#include "camera.h"
#include <cmath>

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

bool TextureController::load_font(std::string id, std::string filename) {
   int ptsize = 28;
   TTF_Font* f = TTF_OpenFont(filename.c_str(), ptsize);

   if(!f) {
      std::cerr << "Could not load font: " << TTF_GetError() << std::endl;
      return false;
   }
   this->fonts[id] = f;

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

   SDL_Rect dest = {x - cam_rect.x, y, 0, 0};
   dest.y = dest.y - cam_rect.y < 0 ? dest.y + cam_rect.y : dest.y + cam_rect.y;

   SDL_QueryTexture(tex, nullptr, nullptr, &src.w, &src.h);

   dest.w = src.w * x_scale;
   dest.h = src.h * y_scale;
   SDL_RenderCopyEx(GameEngine::get_instance()->get_renderer(), tex, &src, &dest, rotation, nullptr, flip);
}

void TextureController::render_frame(std::string id, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio) {
   SDL_Texture *tex = textures[id];

   SDL_Rect cam_rect = Camera::get_instance()->get_view();
   cam_rect.x *= speed_ratio;
   cam_rect.y *= speed_ratio;

   dest.x -= cam_rect.x;
   dest.y += cam_rect.y;

   dest.w *= x_scale;
   dest.h *= y_scale;

   SDL_RenderCopyEx(GameEngine::get_instance()->get_renderer(), tex, &src, &dest, rotation, nullptr, flip);
}

void TextureController::render_ui(std::string id, int x, int y) {

}

void TextureController::render_text(std::string font_id, std::string text, SDL_Rect dest) {
   SDL_Color black = {0, 0, 0};
   SDL_Surface *rendered = TTF_RenderText_Solid(this->fonts[font_id], text.c_str(), black);
   SDL_Texture *tex = SDL_CreateTextureFromSurface(GameEngine::get_instance()->get_renderer(), rendered);

   SDL_RenderCopy(GameEngine::get_instance()->get_renderer(), tex, nullptr, &dest);

   SDL_FreeSurface(rendered);
   SDL_DestroyTexture(tex);
}
