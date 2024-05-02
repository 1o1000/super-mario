#include <fmt/ranges.h>

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

static double sign(double v) {
  if (v < -0.01) return -1;
  if (v > 0.01) return 1;

  return 0;
}
static double lerp(double from, double to, double t) {
  return from + ((to - from) * t);
}

class Mario {
 public:
  double x = 20;
  double y = 500;
  double acceleration = 20;
  sf::Vector2f velocity;

  void Move(int input, float delta) {
    if (input == 1)
      velocity.x += acceleration;
    else if (input == -1)
      velocity.x -= acceleration;

    velocity.x =
        sign(velocity.x) *
        std::min(std::abs(static_cast<double>(velocity.x)), max_x_velocity);
    if (input == 0) velocity.x = lerp(velocity.x, 0, 15 * delta);

    x += velocity.x * delta;
  }

  void Jump(int input, float delta) {
    if (y > 500) {
      velocity.y = 0;
      y = 500;
    }
    if (y < 499.7) velocity.y -= 3000 * delta;
    if (input == -1 && y >= 500) {
      is_jumping = true;
      velocity.y = jump_strength;
      jump_duration = 0;
    }
    if (input == -1 && is_jumping) {
      velocity.y = jump_strength;
      jump_duration += delta;
    }
    if (jump_duration > .3 || input != -1) is_jumping = false;

    y -= velocity.y * delta;
  }

 private:
  double jump_duration = 0;
  bool is_jumping = false;
  double jump_strength = 400.0;
  double max_x_velocity = 200.0;
};