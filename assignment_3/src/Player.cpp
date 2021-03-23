
#include "Player.h"
#include "globals.h"

Player::Player(int w, int h, std::string sprite_filename) : GameObject(0, 0, w, h, "player_sprite", sprite_filename) {

}

Player::~Player() {

}
