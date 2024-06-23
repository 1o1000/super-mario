#include "headers/Goomba.h"

#include "headers/Game.h"
#include "headers/Map.h"

extern Game game;

void Goomba::Die() {
  is_dead = true;
  sprite.setTexture(goombaDeath);
}

sf::Texture tex;

Goomba::Goomba(int id, int x, int y) {
  static sf::SoundBuffer buffer;

  goombaDeath.loadFromFile("textures/GoombaDeath.png");
  goombaWalk.emplace_back();
  goombaWalk.emplace_back();
  goombaWalk[0].loadFromFile("textures/GoombaWalk1.png");
  goombaWalk[1].loadFromFile("textures/GoombaWalk2.png");
  buffer.loadFromFile("sounds/GoombaDeath.wav");
  goombaDie.setBuffer(buffer);

  tex.loadFromFile("textures/GoombaWalk1.png");

  this->id = id;
  this->x = x;
  this->y = y;

  sprite.setTexture(tex);
  sprite.setScale(game.currentLevel.scale, game.currentLevel.scale);
  return;
}

void Goomba::Move(float dt) {
  if (is_dead) {
    fall += 1;
    y += 120 * dt * fall;
    sprite.setPosition(x, y);
    if (y > 620) will_delete = true;
    return;
  }
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

  if (checkGoomba(getTileLocation(x + game.currentLevel.tile_size / 2, y),
                  id) != -1)
    direction = -1;
  if (checkGoomba(getTileLocation(x - game.currentLevel.tile_size / 2, y),
                  id) != -1)
    direction = 1;

  animation.animate(sprite, goombaWalk, 0.4, dt);
  sprite.setPosition(x, y);
};

int checkGoomba(int x, int y) {
  for (auto goomba : game.currentLevel.goombas) {
    if (goomba.is_dead) continue;
    bool horizontal_check =
        (x > goomba.x && (x - goomba.x < game.currentLevel.tile_size ||
                          goomba.x - x > game.currentLevel.tile_size));
    bool vertical_check =
        getTileLocation(x, y).y == getTileLocation(goomba.x, goomba.y).y;
    if (horizontal_check && vertical_check) return goomba.id;
  }
  return -1;
}

int checkGoomba(Point point, int id) {
  for (auto& goomba : game.currentLevel.goombas) {
    if (goomba.is_dead) continue;
    Point tile = getTileLocation(goomba.x, goomba.y);
    if ((tile.x == point.x && tile.y == point.y) && goomba.id != id)
      return goomba.id;
  }
  return -1;
}