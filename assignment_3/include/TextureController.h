#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureController {
   public:
       static TextureController* get_instance() { return s_instance = (s_instance != nullptr) ? s_instance : new TextureController();}

       bool load(std::string id, std::string filename);
       void drop(std::string id);
       void cleanup();
       void render(std::string id, int x, int y, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio);
       void render_frame(std::string id, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio);

   private:
      std::map<std::string, SDL_Texture*> textures;
      TextureController(){}
      static TextureController *s_instance;
};

#endif
