#include <cmath>
#include "TextureController.h"
#include "GameEngine.h"
#include "camera.h"

using std::string;

TextureController* TextureController::s_instance = nullptr;

// Load a texture into the map from the specified file, identified by id
bool TextureController::load(string id, string filename) {
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

// Similar to load, except loads a font with the specified id
bool TextureController::load_font(string id, string filename) {
   int ptsize = 24;
   TTF_Font* f = TTF_OpenFont(filename.c_str(), ptsize);

   if(!f) {
      std::cerr << "Could not load font: " << TTF_GetError() << std::endl;
      return false;
   }
   this->fonts[id] = f;

   return true;
}

// Unused method, but would be useful for keeping a lower memory footprint
void TextureController::drop(string id) {

}

// Cleanup memory of leftover resources
void TextureController::cleanup() {
   for(auto it : this->textures) {
      SDL_DestroyTexture(it.second);
   }
   for(auto it : this->fonts) {
      TTF_CloseFont(it.second);
   }
}

// Render texture of id at position (x,y) relative to the camera.
void TextureController::render(string id, int x, int y, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio) {
   SDL_Texture *tex = textures[id];

   SDL_Rect src = {0, 0, 0, 0};
   SDL_Rect cam_rect = Camera::get_instance()->get_view();
   cam_rect.x *= speed_ratio;

   // Create destination rect that is relative to the camera
   SDL_Rect dest = {x - cam_rect.x, y, 0, 0};
   dest.y = dest.y - cam_rect.y < 0 ? dest.y + cam_rect.y : dest.y + cam_rect.y;

   // Get width and height for src rect
   SDL_QueryTexture(tex, nullptr, nullptr, &src.w, &src.h);

   // Scale destination rect
   dest.w = src.w * x_scale;
   dest.h = src.h * y_scale;
   SDL_RenderCopyEx(GameEngine::get_instance()->get_renderer(), tex, &src, &dest, rotation, nullptr, flip);
}

// Render a frame from a sprite
// This differs from the render option, because the src rect is not just the whole image, it's a part of it
void TextureController::render_frame(string id, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio) {
   SDL_Texture *tex = textures[id];

   // Get camera position
   SDL_Rect cam_rect = Camera::get_instance()->get_view();
   cam_rect.x *= speed_ratio;
   cam_rect.y *= speed_ratio;

   // Move dest relative to the camera
   dest.x -= cam_rect.x;
   dest.y += cam_rect.y;

   // Scale dest
   dest.w *= x_scale;
   dest.h *= y_scale;

   SDL_RenderCopyEx(GameEngine::get_instance()->get_renderer(), tex, &src, &dest, rotation, nullptr, flip);
}

// Unused
void TextureController::render_ui(string id, int x, int y) {

}

// Actually display text to the screen
void TextureController::display_text(SDL_Texture* text, SDL_Rect dest) {
   SDL_RenderCopy(GameEngine::get_instance()->get_renderer(), text, nullptr, &dest);
}

// Render text from the specified font into a texture to be used
SDL_Texture* TextureController::render_text(string font_id, string text) {
   SDL_Color black = {0, 0, 0, 0};
   SDL_Color bg = {255, 255, 255, 150};
   SDL_Surface *rendered = TTF_RenderText_Shaded(this->fonts[font_id], text.c_str(), black, bg);
   SDL_Texture *tex = SDL_CreateTextureFromSurface(GameEngine::get_instance()->get_renderer(), rendered);
   SDL_FreeSurface(rendered);
   return tex;
}

// Render a solid color to the screen
void TextureController::render_color(SDL_Color col) {
   SDL_Rect dest = Camera::get_instance()->get_max();
   SDL_Renderer *ren = GameEngine::get_instance()->get_renderer();

   SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, col.a);

   SDL_RenderFillRect(ren, nullptr);
}

// Get dimensions of the texture specified by id
SDL_Rect TextureController::get_img_dims(string id) {
   SDL_Rect res;

   SDL_QueryTexture(textures[id], nullptr, nullptr, &res.w, &res.h);

   return res;
}
