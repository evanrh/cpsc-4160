#ifndef LAYERS_H
#define LAYERS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Layer {
   public:
      virtual void update() {}
      virtual void render() {}
};

class BackgroundLayer: public Layer {
   public:
      BackgroundLayer(std::string id, std::string filename, int x, int y, float scroll_speed=1.0f, float x_scale=1.0f, float y_scale=1.0f);
      void update() {}
      void render();
   private:
      std::string texture_id;
      int x_offset;
      int y_offset;
      float scroll;
      float x_scale;
      float y_scale;
      int width;
      int height;
      SDL_RendererFlip flip;
};

#endif
