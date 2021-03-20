#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureController {
   public:
       static TextureController* GetInstance() { return s_instance = (s_instance != nullptr) ? s_instance : new TextureController();}

       bool load(std::string id, std::string filename, SDL_Renderer* ren);
       void drop(std::string id);
       void cleanup();
       void render(std::string id, SDL_Renderer* ren, int x, int y, uint32_t width, uint32_t height);

   private:
      std::map<std::string, SDL_Texture*> textures;
      TextureController();
      static TextureController *s_instance;
};

#endif
