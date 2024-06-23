#include "headers/Coin.h"

#include "headers/Game.h"
#include "headers/Map.h"

extern Game game;

void Coin::Die() { is_dead = true; };

Coin::Coin(int x, int y) {
  this->x = x;
  this->y = y;
  coin_texture.loadFromFile("textures/Coin.png");
  sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);
  sprite.setTexture(coin_texture);
}

void Coin::Move(float dt) {
  if (is_dead) {
    fall += 1;
    y += 120 * dt * fall;
    sprite.setPosition(-500, -500);
    return;
  }

  sprite.setTexture(coin_texture);
  y += 32 * dt;

  if (game.currentLevel
          .HasBlock(getTileLocation(x, y + game.currentLevel.tile_size))
          .id != "")
    is_dead = true;

  sprite.setPosition(x, y);
}