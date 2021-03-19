#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureController {
   public:
       static TextureController* GetInstance() { return s_instance = (s_instance != nullptr) ? s_instance : new TextureController();}

       bool load(std::string id, std::string filename);
       void drop(std::string id);
       void clean();
       void render(std::string id, int x, int y, int width, int height

   private:
      TextureController();
      static TextureController s_instance;
}
