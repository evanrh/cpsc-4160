
#include "Player.h"
#include "globals.h"

Player::Player(SDL_Renderer* ren) : GameObject(SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, ren, "./player.png") {

}

Player::~Player() {

}
