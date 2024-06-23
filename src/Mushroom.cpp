#include "headers/Mushroom.h"

#include "headers/Game.h"
#include "headers/Map.h"

extern Game game;

void Mushroom::Die() { is_dead = true; };

Mushroom& checkMushroom(double x, double y) {
  for (auto& mushroom : game.currentLevel.mushrooms) {
    bool horizontal_check =
        (x > mushroom.x && (x - mushroom.x < game.currentLevel.tile_size ||
                            mushroom.x - x > game.currentLevel.tile_size));
    bool vertical_check =
        getTileLocation(x, y).y == getTileLocation(mushroom.x, mushroom.y).y;
    if (horizontal_check && vertical_check) return mushroom;
  }
  static auto mushroom = Mushroom{-1, -1};
  return mushroom;
}

Mushroom::Mushroom(int x, int y) {
  this->x = x;
  this->y = y;
  mushroom_texture.loadFromFile("textures/Mushroom.png");
  sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);
  sprite.setTexture(mushroom_texture);
}

void Mushroom::Move(float dt) {
  if (is_dead) {
    fall += 1;
    y += 120 * dt * fall;
    sprite.setPosition(x, y);
    return;
  }

  sprite.setTexture(mushroom_texture);
  x += 32 * dt * direction;
  y += 32 * dt * fall;

  if (game.currentLevel
          .HasBlock(getTileLocation(x + game.currentLevel.tile_size + 1, y))
          .id != "")
    direction = -1;
  if (game.currentLevel.HasBlock(getTileLocation(x - 1, y)).id != "")
    direction = 1;
  if (game.currentLevel
          .HasBlock(getTileLocation(x, y + game.currentLevel.tile_size + 1))
          .id == "")
    fall += 1;
  if (game.currentLevel
          .HasBlock(getTileLocation(x, y + game.currentLevel.tile_size + 1))
          .id != "") {
    fall = 0;
    y -= fmod(y, game.currentLevel.tile_size);
  }

  sprite.setPosition(x, y);
}