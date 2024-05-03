#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

static double sign(double v) {
  if (v < -0.01) return -1;
  if (v > 0.01) return 1;

  return 0;
}

struct Point {
  int x;
  int y;
};

Point getTileLocation(double x, double y) {
  Point point;
  point.x = (int)(x / 48);
  point.y = (int)(y / 48);
  return point;
}

bool hasBlock(Point point, std::vector<Block> grounds) {
  for (Block ground : grounds) {
    Point tile = getTileLocation(ground.x, ground.y);
    if (tile.x == point.x && tile.y == point.y) return true;
  }
  return false;
}

class Mario {
 public:
  bool isOnGround = true;
  double x = 20;
  double y = 432;
  double acceleration = 200;
  sf::Vector2f velocity;

  void Move(int input, float delta, const std::vector<Block>& grounds,
            int leftPos) {
    if (input == 1 && velocity.x > -0.5)
      velocity.x += acceleration * delta;
    else if (input == -1 && velocity.x < 0.5)
      velocity.x -= acceleration * delta;
    else
      velocity.x *= 0.95;

    velocity.x =
        sign(velocity.x) *
        std::min(std::abs(static_cast<double>(velocity.x)), max_x_velocity);

    if (x - 23 > leftPos) {
      x += velocity.x * delta;
    } else {
      x += 1;
    }

    if (hasBlock(getTileLocation(x + 24, y - 24), grounds)) {
      velocity.x = 0;
      x -= fmod(x, 48) - 24;
    }
    if (hasBlock(getTileLocation(x - 24, y - 24), grounds)) {
      velocity.x = 0;
      x += fmod(x, 48) - 23;
    }
  }

  void Jump(int input, float delta, const std::vector<Block>& grounds) {
    // if (y > 432) {
    //   velocity.y = 0;
    //   y = 432;
    // }
    velocity.y -= 3000 * delta;
    if (input == -1) {
      is_jumping = true;
      velocity.y = jump_strength;
      jump_duration = 0;
    }
    if (input == -1 && is_jumping) {
      velocity.y = jump_strength;
      jump_duration += delta;
    }
    if (jump_duration > .3 || input != -1) is_jumping = false;

    isOnGround = false;
    if (hasBlock(getTileLocation(x, y - 49), grounds)) {
      is_jumping = false;
      velocity.y = 0;
      velocity.y -= 3000 * delta;
    }
    y -= velocity.y * delta;
    if (hasBlock(getTileLocation(x, y + 1), grounds)) {
      isOnGround = true;
      velocity.y = 0;
      y -= fmod(y, 48);
    }

    if (fmod(x, 48) > 24)
      if (hasBlock(getTileLocation(x + 48, y + 1), grounds)) {
        isOnGround = true;
        velocity.y = 0;
        y -= fmod(y, 48);
      }
    if (fmod(x, 48) < 24)
      if (hasBlock(getTileLocation(x - 48, y + 1), grounds)) {
        isOnGround = true;
        velocity.y = 0;
        y -= fmod(y, 48);
      }
  }

 private:
  double jump_duration = 0;
  bool is_jumping = false;
  double jump_strength = 400.0;
  double max_x_velocity = 200.0;
};