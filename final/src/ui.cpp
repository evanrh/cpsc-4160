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

}

void LifeCounter::reset() {
   GameEngine* ge = GameEngine::get_instance();

}

void LifeCounter::render() {
   unsigned lives = GameEngine::get_instance()->get_player()->get_lives();
   TextureController::get_instance()->render_text(font, std::to_string(lives), ui_rect);
}
