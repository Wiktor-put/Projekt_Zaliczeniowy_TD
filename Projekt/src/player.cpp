#include "player.h"
#include "Config.h"

Player::Player() {
    reset();
}

void Player::reset() {
    lives = Config::STARTING_LIVES;
    money = Config::STARTING_MONEY;
    score = 0;
}

bool Player::spendMoney(int cost) {
    if (!enoughMoney(cost)) return false;
    money -= cost;
    return true;
}

void Player::addMoney(int amount) {
    money += amount;
}

void Player::lostLives(int amount) {
    lives -= amount;
    if (lives < 0) lives = 0;
}
