#include "ui.h"
#include "TextureController.h"
#include "GameEngine.h"

UIElement::UIElement(int x, int y, unsigned w, unsigned h, std::string img_id, std::string img_file, std::string font) {
   ui_rect.x = x;
   ui_rect.y = y;
   ui_rect.w = w;
   ui_rect.h = h;
   this->img_id = img_id;
   this->font = font;
   TextureController::get_instance()->load(img_id, img_file);
   text_tex = nullptr;
}

void UIElement::update() {

}

void UIElement::render() {
   auto tc = TextureController::get_instance();
   tc->render(img_id, ui_rect.x, ui_rect.y, SDL_FLIP_NONE, 1.0f, 1.0f, 0, 1.0f);
}

void UIElement::reset() {

}

void LifeCounter::update() {
   current_lives = GameEngine::get_instance()->get_player()->get_lives();
}

void LifeCounter::reset() {
   GameEngine* ge = GameEngine::get_instance();

}

void LifeCounter::render() {
   if(current_lives != previous_lives) {
      std::string text = std::to_string(current_lives) + " lives";
      if(text_tex != nullptr) {
         SDL_DestroyTexture(text_tex);
      }
      text_tex = TextureController::get_instance()->render_text(font, text, ui_rect);
   }
   TextureController::get_instance()->display_text(text_tex, ui_rect);
   previous_lives = current_lives;
}
