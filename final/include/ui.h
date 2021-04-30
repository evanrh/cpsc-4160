#ifndef UI_H
#define UI_H

#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameObject.h"

// Abstract Base UI Element
class UIElement {
   public:
      UIElement() {}
      UIElement(int x, int y, unsigned w, unsigned h, std::string font_name, std::string text);
      virtual void update();
      virtual void render();
      virtual void reset();
      void set_text(std::string t) { text = t; }

      friend class Screen;

   protected:
      SDL_Rect ui_rect;
      SDL_Texture* text_tex;
      std::string font, text;
};

// Abstract Base Screen Element
class Screen {
   public:
      Screen() {}
      virtual void render() = 0;
      virtual void handle_input() = 0;
      virtual void update() = 0;
      void add_element(UIElement* e) { options.push_back(e); }
   protected:
      std::vector<UIElement*> options;
};

// Pause Screen
class PauseScreen: public Screen {
    public:
       PauseScreen(SDL_Color color, std::string font);
       void render();
       void handle_input();
       void update();
    private:
       SDL_Color col;
       SDL_Event input;
};

// Image Displaying Screen i.e start or death screen
class ImageScreen: public Screen {
    public:
        ImageScreen(std::string img_id, std::string img_file);
        void render();
        void handle_input();
        void update();
    private:
        std::string img_id;
};

// Lives UI Element
class LifeCounter: public UIElement {
   public:
      LifeCounter(int x, int y, unsigned w, unsigned h, std::string font_name);
      void update();
      void reset();
      void render();
   private:
      unsigned current_lives = 0, previous_lives = 0;
};

// Frame rate display element
class FrameRate: public UIElement {
   public:
      FrameRate(int x, int y, unsigned w, unsigned h, std::string font);
      void update();
      void render();
   private:
      unsigned current_framerate = 0, prev_framerate = 0;
};

// Button UI Element; typically goes in a screen
class Button: public UIElement {

};

#endif
