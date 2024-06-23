#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include <string>

#include "headers/Animation.h"

class Block {
 public:
  std::string id;
  double x;
  double y;
  sf::Sprite sprite;
  Block(std::string id, int x, int y);
  void Kick();
  void Interact(float dt);

 private:
  Animation animation;
  bool spawned = false;
  bool kicked = false;
  bool reached = false;
  double animation_y = 0;
  sf::Texture question1;
  sf::Texture question2;
  sf::Texture question3;
  sf::Texture question4;
  std::vector<sf::Texture> questions;
};
#endif