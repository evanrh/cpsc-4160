#ifndef UI_H
#define UI_H

#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameObject.h"

class UIElement {
   public:
      UIElement(int x, int y, unsigned w, unsigned h, std::string img_id, std::string img_file, std::string font);
      virtual void update();
      virtual void render();
      virtual void reset();

   protected:
      SDL_Rect ui_rect;
      SDL_Texture* text_tex;
      std::string img_id;
      std::string font;
};

class Screen {
   public:
      Screen(std::string img_id, std::string img_file);
   private:
      SDL_Rect scr_rect;
      std::string img_id;
      std::vector<UIElement> options;
};

class LifeCounter: public UIElement {
   public:
      using UIElement::UIElement;
      virtual void update();
      virtual void reset();
      virtual void render();
   private:
      unsigned current_lives = 0, previous_lives = 0;
};

#endif
