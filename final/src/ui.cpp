#include "ui.h"
#include "TextureController.h"
#include "GameEngine.h"
#include "camera.h"

using std::to_string;

// Generic UI Element constructor
UIElement::UIElement(int x, int y, unsigned w, unsigned h, std::string font_name, std::string text) {
   ui_rect.x = x;
   ui_rect.y = y;
   ui_rect.w = w;
   ui_rect.h = h;
   this->font = font_name;
   this->text = text;
   text_tex = TextureController::get_instance()->render_text(font, text);
}

void UIElement::update() {

}

void UIElement::render() {
   auto tc = TextureController::get_instance();
   tc->display_text(text_tex, ui_rect);
}

void UIElement::reset() {

}

LifeCounter::LifeCounter(int x, int y, unsigned w, unsigned h, std::string font_name) : UIElement(x, y, w, h, font_name, "")
{
}
// Get current lives of player
void LifeCounter::update() {
   current_lives = GameEngine::get_instance()->get_player()->get_lives();
}

void LifeCounter::reset() {

}

// Render lives UI
void LifeCounter::render() {

   // Only redraw text if lives have changed
   if(current_lives != previous_lives) {
      std::string l_text = std::to_string(current_lives) + " lives";
      if(text_tex != nullptr) {
         SDL_DestroyTexture(text_tex);
      }
      text_tex = TextureController::get_instance()->render_text(font, l_text);
   }
   TextureController::get_instance()->display_text(text_tex, ui_rect);
   previous_lives = current_lives;
}

FrameRate::FrameRate(int x, int y, unsigned w, unsigned h, std::string font) {
   ui_rect.x = x;
   ui_rect.y = y;
   ui_rect.w = w;
   ui_rect.h = h;

   this->font = font;
   current_framerate = GameEngine::get_instance()->get_framerate();
   text_tex = TextureController::get_instance()->render_text(font, std::to_string(current_framerate));
}

// Get current framerate
void FrameRate::update() {
   current_framerate = GameEngine::get_instance()->get_framerate();
}

// Display current frame rate
void FrameRate::render() {

   // Similar to lives UI, only redraw texture if framerate changes
   if(current_framerate != prev_framerate) {
      SDL_DestroyTexture(text_tex);
      text_tex = TextureController::get_instance()->render_text(font, std::to_string(current_framerate));
   }
   TextureController::get_instance()->display_text(text_tex, ui_rect);
   prev_framerate = current_framerate;
}

PauseScreen::PauseScreen(SDL_Color color, std::string font) {
   SDL_Rect pos = Camera::get_instance()->get_max();
   UIElement *title, *resume, *quit;
   col = color;

   title = new UIElement(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 4, 40, font, "Pause");
   resume = new UIElement(SCREEN_WIDTH / 4, SCREEN_HEIGHT * 2/3, SCREEN_WIDTH / 4, 40, font, "Press P to resume");

   options.push_back(title);
   options.push_back(resume);
   //delete title;
}

void PauseScreen::render() {
   TextureController::get_instance()->render_color(col);

   for(auto btn : options) {
      btn->render();
   }
   SDL_RenderPresent(GameEngine::get_instance()->get_renderer());
}

void PauseScreen::handle_input() {

   while(SDL_PollEvent(&this->input)) {
      switch(input.type) {
         case SDL_QUIT:
            GameEngine::get_instance()->unpause();
            GameEngine::get_instance()->stop_running();
            break;
         case SDL_KEYDOWN:
            switch(input.key.keysym.sym) {
               case SDLK_UP:
                  break;
               case SDLK_p:
                  GameEngine::get_instance()->unpause();
               default:
                  break;
            }
            break;
      }
   }
}

void PauseScreen::update() {

}

ImageScreen::ImageScreen(std::string img_id, std::string img_file) {
   this->img_id = img_id;
   TextureController::get_instance()->load(img_id, img_file);
}

void ImageScreen::render() {

   SDL_Rect dims = TextureController::get_instance()->get_img_dims(img_id);
   int x = (SCREEN_WIDTH / 2) - (dims.w / 2);
   int y = (SCREEN_HEIGHT / 2) - (dims.h / 2);
   SDL_RenderClear(GameEngine::get_instance()->get_renderer());
   TextureController::get_instance()->render(img_id, x, y, SDL_FLIP_NONE, 1.0f, 1.0f, 0, 1.0f);

   SDL_RenderPresent(GameEngine::get_instance()->get_renderer());
}

void ImageScreen::handle_input() {

   if(options.size() > 0) {
      // Do something
   }
}

void ImageScreen::update() {

}

StartScreen::StartScreen(std::string img_id, std::string img_file) {
   this->img_id = img_id;
   TextureController::get_instance()->load(img_id, img_file);

}

void StartScreen::update() {

}

void StartScreen::handle_input() {

   GameEngine *gp = GameEngine::get_instance();
   while(SDL_PollEvent(&this->input)) {
      switch(input.type) {
         case SDL_QUIT:
            gp->start_game();
            gp->stop_running();
            break;
         case SDL_KEYDOWN:
            switch(input.key.keysym.sym) {
               case SDLK_RETURN:
                  gp->start_game();
                  break;
               default:
                  break;
            }
            break;
      }
   }
}
