#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Direction.h"

class Mario {
 public:
  float x;
  float y;
  float speed;
  bool changedDirection;
  bool moving;
  Direction direction = Direction::Right;
  void move();
  void stop();
  void changeDirection();
  void jump();
};

void Mario::stop() { this->moving = false; }

void Mario::move() {
  if (this->changedDirection) {
    if (this->speed > 0)
      this->speed -= 0.00003;
    else if (this->speed <= 0) {
      this->changedDirection = false;
      this->speed = 0;
    }
  }
  if (this->speed < 0.05 && this->speed > -0.05) {
    if (this->direction == Direction::Left && this->moving)
      this->speed -= 0.00003;
    if (this->direction == Direction::Right && this->moving)
      this->speed += 0.00003;
  }
  this->x += speed;
}

void Mario::changeDirection() {
  this->changedDirection = true;
  if (this->direction = Direction::Right)
    this->direction = Direction::Left;
  else
    this->direction = Direction::Right;
}

void Mario::jump() {
  // TODO: Add Jump
}
