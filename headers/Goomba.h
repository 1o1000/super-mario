#ifndef GOOMBA_H
#define GOOMBA_H
#include <fmt/ranges.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "headers/Animation.h"
#include "headers/Block.h"
#include "headers/Point.h"

class Goomba;
class Map;

Point getTileLocation(double x, double y);
int checkGoomba(Point point, int id);
int checkGoomba(int x, int y);

class Goomba {
 public:
  int id;
  bool is_dead = false;

  bool will_delete = false;

  sf::Sprite sprite;
  sf::Sound goombaDie;

  double x;
  double y;
  int direction = 1;
  int fall = 0;

  Goomba(int id, int x, int y);
  void Die();
  void Move(float dt);

 private:
  Animation animation;

  sf::Texture goombaDeath;
  std::vector<sf::Texture> goombaWalk;
};

#endif