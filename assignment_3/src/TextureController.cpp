#include "TextureController.h"

bool TextureController::load(std::string id, std::string filename, SDL_Renderer* ren) {
   SDL_Surface* surf = IMG_Load(filename.c_str());
   SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);

   if(!tex) {
      std::cerr << "Could not load image: " << SDL_GetError() << std::endl;
      return false;
   }

   this->textures[id] = tex;

   SDL_FreeSurface(surf);
   return true;
}

void drop(std::string id) {

}

void cleanup() {

}

void render(std::string id, SDL_Renderer* ren, int x, int y, uint32_t width, uint32_t height) {

}

