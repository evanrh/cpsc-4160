
#include "Player.h"
#include "globals.h"

Player::Player(SDL_Renderer* ren, const char* sprite_filename) : GameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, ren, sprite_filename) {

}

Player::~Player() {

}
