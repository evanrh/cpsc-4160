#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#include <map>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using std::string;

class TextureController {
   public:
       static TextureController* get_instance() { return s_instance = (s_instance != nullptr) ? s_instance : new TextureController();}

       bool load(string id, string filename);
       bool load_font(string id, string filename);
       void drop(string id);
       void cleanup();
       void render(string id, int x, int y, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio);
       void render_frame(string id, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, float x_scale, float y_scale, float rotation, float speed_ratio);
       void render_ui(string id, int x, int y);
       void display_text(SDL_Texture *text, SDL_Rect dest);
       SDL_Texture* render_text(string font_id, string text);
       SDL_Rect get_img_dims(string id);
       void render_color(SDL_Color col);

   private:
      std::map<string, SDL_Texture*> textures;
      std::map<string, TTF_Font*> fonts;
      TextureController(){}
      static TextureController *s_instance;
};

#endif
