#ifndef COIN_H
#define COIN_H

#include <SFML/Graphics.hpp>

class Coin {
 public:
  double x;
  double y;
  sf::Sprite sprite;

  Coin(int x, int y);
  void Die();
  void Move(float dt);

 private:
  sf::Texture coin_texture;
  int fall = 0;
  int direction = 1;
  bool is_dead = false;
};

#endif