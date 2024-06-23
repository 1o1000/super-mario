#ifndef MUSHROOM_H
#define MUSHROOM_H

#include <SFML/Graphics.hpp>

class Mushroom {
 public:
  double x;
  double y;
  sf::Sprite sprite;

  Mushroom(int x, int y);
  void Die();
  void Move(float dt);

 private:
  sf::Texture mushroom_texture;
  int fall = 0;
  int direction = 1;
  bool is_dead = false;
};

#endif