
#include "Player.h"
#include "globals.h"

Player::Player(int w, int h, std::string sprite_filename) : GameObject(100, 100, w, h, "player_sprite", sprite_filename) {

}

Player::~Player() {

}
